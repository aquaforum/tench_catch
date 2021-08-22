#include "aulibdefs.h"
#include "auudptransiver.h"
#include <QUdpSocket>
#include <aubase.h>
#include <autag.h>
#include <auplcdrive.h>

#include <QByteArray>
#include <QDataStream>
#include <QNetworkDatagram>

#include <QDebug>
#include <QNetworkInterface>
#include <QRandomGenerator>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QTimer>

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QSettings>


/* цей клас мультикастить виключно дані, які безболісно можуть бути втрачені тобто plcRawData
 * але на основі його даних формується спсок адрес тих компів, хто власне мультикастить дані для того
 * щоб потім по ТРС передавати важливі дані.
 * формат посилки
 * name:QString - ім'я контролера
 * code:qint8 - код функції
 * 0 - передача сирих даних від контролера, масив plcRawData
 * наступним полем передається quint32:pktCnt лічильник пакунків
 * далі передається plcRawData
 *
 * ще воно ловить індивідуальну зміну шкал.
 *
*/

// !!! із UDP сокетами є онда глобальна проблема - пакети двояться
// і це стосується не тільки Qt а й socket api також.
// і це стосується не тільки лінукса а й вінди також.
// і я щось з цих хочу зробити але не знаю що....

AuUdpTransiver::AuUdpTransiver(AuBase &base, QObject *parent) : QObject(parent)
  ,sock(new QUdpSocket(this))
  ,sSock(new QUdpSocket(this))
  ,src(base)
  ,addr(ADDR)
  ,allPktCnt(0)
  ,allPktSend(0)
{
    // наступних два рядки потрібні для того щоб не було повороту пакунків назад
    QSettings set;

    sSock->bind(QHostAddress(QHostAddress::AnyIPv4),0);
    sSock->setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);

    sock->setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);

    sock->bind(QHostAddress::AnyIPv4,PORTN,QUdpSocket::ShareAddress);//QHostAddress::AnyIPv4

    // шукаємо інтерфейс, який будемо слухати
    QNetworkInterface iFm;
    //qDebug() << iFm.isValid();

    if(set.contains("iface"))
    {
        QString iName=set.value("iface","lo").toString();

        foreach(QNetworkInterface iFace,QNetworkInterface::allInterfaces())
        {
            if(iFace.name()==iName)
                iFm=iFace;
        }
    }
    //qDebug() << iFm.isValid();

    if(iFm.isValid())
    {
        sock->joinMulticastGroup(addr,iFm); // слухати з прив'язкою до інтерфейсу
        sSock->setMulticastInterface(iFm);
    }
    else
        sock->joinMulticastGroup(addr);

    connect(sock,&QUdpSocket::readyRead,this,&AuUdpTransiver::recvData);


    // спеціально для маршрутизаторів
    s2Sock=nullptr;
    QString s2Name=set.value("MulticastLoopbackOption","").toString();
    if(s2Name!="")
    {
        QNetworkInterface s2Fm;
        foreach(QNetworkInterface iFace,QNetworkInterface::allInterfaces())
        {
            if(iFace.name()==s2Name)
                s2Fm=iFace;
        }
        if(s2Fm.isValid())
        {
            s2Sock=new QUdpSocket(this);
            s2Sock->bind(QHostAddress(QHostAddress::AnyIPv4),0);
            s2Sock->setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
            s2Sock->setMulticastInterface(s2Fm);
            qDebug() << "Start two socket on " << s2Fm.name();
        }

    }


    foreach (quint16 iX, base.plcList().values()) { // ініціалізувати лічильник відправлених пакунків
        pktCnt[iX]=0;
    }
// зробити спробу відновити scadaAddr із файлв
    QFile f(QStringLiteral("%1/.auscada/scadaAddress").arg(QDir::homePath()));
    if(f.exists())
    {
        f.open(QIODevice::ReadOnly);
        QDataStream out(&f);
        out >> scadaAddr;
        f.close();
    }

    QTimer *statTmr=new QTimer(this);
    statTmr->setInterval(1000);
    statTmr->setTimerType(Qt::PreciseTimer);
    connect(statTmr,&QTimer::timeout,this,&AuUdpTransiver::printStat);
    statTmr->start();
}

AuUdpTransiver::~AuUdpTransiver()
{
    // зберегти scadaAddr щоб при перезапуску програми зробити спробу відновити графіки
    QFile f(QStringLiteral("%1/.auscada/scadaAddress").arg(QDir::homePath()));
    f.open(QIODevice::WriteOnly);
    QDataStream out(&f);
    out << scadaAddr;
    f.close();

}

void AuUdpTransiver::recvData()
{
    qint8 code=-1;
    quint64 cnt;
    // quint16 address; // це для зберігання отриманої адреси
    quint16 pktCrc,lCrc=0; // =0 щоб не сварилося на неініціалізовану змінну

    while (sock->hasPendingDatagrams()) { // якщо є необроблені датаграми
        ++allPktCnt;
        // треба відфільтрувати свої
        QNetworkDatagram dGram=sock->receiveDatagram();
        // qDebug() << dGram.senderAddress() << dGram.data().size();

#ifdef WIN32 // під відндовосм вищенаведений механізм запобігання петлям чомусь не працює
        if( QNetworkInterface::allAddresses().contains(dGram.senderAddress() ))
        {
            continue; // пропустити свої
        }
#endif

        // ініціалізувати масив з контрольними сумами пакунків
        if(lastCrc.keys().contains(dGram.senderAddress()))
            lCrc=lastCrc[dGram.senderAddress()];
        else
            lastCrc[dGram.senderAddress()]=0;

        QByteArray data=qUncompress(dGram.data());
        QDataStream in(&data,QIODevice::ReadOnly);
        in.setVersion(QTVER);

        QString plcName ,tagName ;
        in >> plcName;
        in >> code;

        //qDebug() << dGram.senderAddress().toString() << "plcName" << plcName << "code" << code;

        // може простіше перевірити наявність plcName за допомогою src.plcList().contains(plcName) ?
        // але з try catch наче красивіше виглядає тим більше що там і так є ця перевірка
        switch  (code) {
        default:
            emit Alert(QStringLiteral(" AuUdpTransiverunknown operation code:%1").arg(code));
            break;
        case UDP_RAW:
            in >> cnt;
            try { // ловити виключення THROWN_NOPLC
            // qDebug() << dGram.senderAddress() << plcName;
            if(! src[plcName].active()) // якщо самі не працюємо з цми к
            {
                in >> src[plcName].data();
                // перевірка контрольної суми пакунка із поперед
                in >> pktCrc;
                if(pktCrc!=lCrc) // перевірити контрольну суму якщо не співпала - обробити пакунки
                {
                    emit updateData(src[plcName].index());
                    // qDebug() << dGram.senderAddress().toString() << plcName<< "code " << code << "pktCrc " << pktCrc << "allPktCnt: " << allPktCnt ;
                    rcvCout[src[plcName].index()]++;
                }
                //else
                //    qDebug() << "UPD_RAW double pkt. Count: " << cnt <<  pktCrc << lCrc << " sender: " << dGram.senderAddress();

                lastCrc[dGram.senderAddress()]=pktCrc;

            }

            }
            catch (int v) {
                emit Alert(QStringLiteral("No plcName found in code UDP_RAW of AuUdpTransive: %1").arg(plcName));
            }
            break;

            case UDP_PARAM: // передача вибіркових параметрів тегів
                {
                    QHash<QString , QHash<quint8,double> > paramHash;
                    in >> paramHash;
                    //qDebug() << plcName << paramHash << dGram.senderAddress() << dGram.senderPort() ;
                    // а тепер тут треба розтасувати по типам
                    try{
                        // плюс саме такої кострукції що за відсутності plcName помилка спрацє тільки один раз
                        // але за відстуності одного tagName - всі інші теги до кінця будуть пропущені
                        // якщо ж try catch внести в цикл то будуть оброблені всі теги але за відсутності plcName
                        // Alert із catch стрельне багато разів.....

                    foreach(QString tagName,paramHash.keys())
                    {
                        //if(src[plcName].tagsList().contains(tagName)) // перевірити наявність тега
                        //{ віддано на відкуп try... catch
                            foreach(quint8 fType,paramHash[tagName].keys())
                            {
                                double val=paramHash[tagName][fType];
                                switch (fType) {
                                case TAG_ZERO:
                                    src[plcName][tagName].setZero(val);
                                    break;
                                case TAG_FULL:
                                    src[plcName][tagName].setFull(val);
                                    break;
                                case TAG_BASE:
                                    src[plcName][tagName].setBaseScale(val);
                                    break;
                                case TAG_DECIMAL:
                                    src[plcName][tagName].setDecimal(val);
                                    break;
                                default:
                                    break;

                                }
                            }
                        //}
                    }
                    }
                    catch (int v)
                    {
                        switch (v) {
                        case THROWN_NOPLC:
                            emit Alert(QStringLiteral("No plc found in code UDP_PARAM of AuUdpTransive: %1").arg (plcName));
                            break;
                        case THROWN_NOTAG:
                            emit Alert(QStringLiteral("No tag found in code UDP_PARAM of AuUdpTransiver. plc: %1 tag: %2").arg(plcName).arg(tagName));
                            break;
                        default:
                            emit Alert(QStringLiteral("Unknown thrown in code UDP_PARAM : %1").arg(v));
                            break;

                        }

                    }
                }
                break;

        }
        scadaAddr[plcName]=dGram.senderAddress(); // зберегти адресу для відправи даних назад

        }
}

void AuUdpTransiver::printStat()
{
    //qDebug() << "PPS: " << allPktCnt;
    emit Alert(QString("PPS revc: %1\n"
                       "PPS sent: %2").arg(allPktCnt).arg(allPktSend));

    allPktCnt=0;
    allPktSend=0;
}

// цей метод весь час передає дані в мережу, після того як отримає їх від контролера
void AuUdpTransiver::sendData(quint16 iX)
{
    try {
    QString plcName=src[iX].name();
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out.setVersion(QTVER);


    out << plcName << qint8(UDP_RAW) << pktCnt[iX]++ <<  src[iX].data(); // яким чином приймач знатиме що список тегів не змінився ?
    quint16 crc= qChecksum(data.data(),data.size());
    out << crc;
    //  qDebug() << pktCnt[iX];
    // передавати контрольну суму list файла мені якось не хочеться, хоча мабуть треба.....

    sSock->writeDatagram(qCompress(data),addr,PORTN);
    if(s2Sock!=nullptr)
        s2Sock->writeDatagram(qCompress(data),addr,PORTN);

    ++allPktSend;
    }
    catch(int v)
    {
        emit Alert(QStringLiteral("No plc found in AuUdpTransiver::sendData(iX) iX: %1").arg(iX));
    }

}

QHash<QString,QHostAddress>& AuUdpTransiver::scadaAddreses()
{
    return  scadaAddr;
}


