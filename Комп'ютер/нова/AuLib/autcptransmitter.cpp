/* Цей клас це клієнська частина для обміну важливими даним
 * 1) передача даних від клієнта в контролера, так, якщо його не слати по UDP то воно буде довше добиратися до інших мережевих клієнтів
 * 2) обмін шкалами
 * 3) обмін історією
 *
 * Механізм обміну даними простий, сокети не тримаються постійно відкритими оскільки передача цих даних досить рідкісна
 * Створюється сокет на стороні клієнта, надсилаються дані,
 * сервер їх отримую, обробляє, за потреби надсилає відповідь клієту і закриває сокет
 *
*/
#include "autcptransmitter.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QRandomGenerator>

#include <aulib.h>

AuTcpTransmitter::AuTcpTransmitter(QHash<QString, QHostAddress> & scadaAddreses, QObject *parent) : QObject(parent)
  , scadaAddr(scadaAddreses)
{
    setObjectName("AuTcpTransmitter");
}

void AuTcpTransmitter::sendPkt(const QString &plcName, QByteArray &data)
{

    if(scadaAddr.contains(plcName))
    {
        QTcpSocket *send=new QTcpSocket(this);

        connect(send,&QTcpSocket::connected,this,&AuTcpTransmitter::slotSendTcp); // коли з'єднання буде встановлено треба буде передати дані
        connect(send,&QTcpSocket::readyRead,this,&AuTcpTransmitter::slotRecvTcp); // це на випадок якщо будуть дані у відповідь
        connect(send,&QTcpSocket::disconnected,this,&AuTcpTransmitter::slotDisconnect);

        //qDebug() << scadaAddr[plcName];
        send->connectToHost(scadaAddr[plcName],PORTN);

        sockData[send]=qCompress(data); // додати дані
        quint32 len=sockData[send].size(); // вирахув
        sockData[send].prepend((char*)&len,TCP_HEADER_LEN);
        // qDebug() << "len:" << len << " pktSize:" << sockData[send].size();
        pktLen[send]=0;
    }

}

void AuTcpTransmitter::setData(QString tagName, quint16 address, QVector<qint16> &data)
{
    QString plcName=qobject_cast<AuTag*>(sender())->plcName();
    QByteArray d;
    QDataStream out(&d,QIODevice::WriteOnly);
    out.setVersion(QTVER);

    out << plcName << quint8(UDP_PUSH) << tagName << address << data;
    quint16 crc= qChecksum(d.data(),d.size());
    out << crc;

    sendPkt(plcName,d); // відправити дані

}

void AuTcpTransmitter::slotNewConnection()
{

}


void AuTcpTransmitter::slotDisconnect()
{
    QTcpSocket *s=qobject_cast<QTcpSocket*>(sender());
    pktLen.remove(s);
    // qDebug() << s << " disconnected";
    s->deleteLater();

}

void AuTcpTransmitter::slotSendTcp()
{

    QTcpSocket *s=qobject_cast<QTcpSocket*>(sender());
    // qDebug() << "write " << sockData[s].size();
    s->write(sockData[s]);
    sockData.remove(s);

}

void AuTcpTransmitter::slotRecvTcp() // тут треба буде розбирати відповідь, коли вона буде
{
    QTcpSocket *s=qobject_cast<QTcpSocket*>(sender());
    // qDebug() << "read " << s->bytesAvailable();
    for(;;)
    {
        if(pktLen[s]==0) // якщо ще нічого не прочитано
        {
            if(s->bytesAvailable()<4) // якщо там є
                break; // нема даних. завершити for
            else // інакше вийняти довжину, є шанс що на великих об'ємах даних воно буде фрагментовано
            {
                int len=0;
                s->read((char*)&len,4); // прочитати довжину
                pktLen[s]=len; // порахувати скільки ще байт не прийнято
                // qDebug() << "pktLen[s]:" << pktLen[s] << len ;
            }
        }
        if(s->bytesAvailable()<pktLen[s])
            break;
        QByteArray data=qUncompress(s->read(pktLen[s])); // прочитати і розпакувати
        QDataStream in(&data,QIODevice::ReadOnly);
        in.setVersion(QTVER);

        QString plcName ,tagName ;
        // quint16 address;
        QVector<qint16> rData;
        // quint16 pktCrc;
        qint8 code=-1;

        in >> plcName;
        in >> code;
        switch (code) {
        default:
            emit Alert(QStringLiteral("AuTcpReceiver unknown operation code:%1").arg(code));
            break;
        case UDP_ALLPARAM_RECV: // тут все просто, надіслати тому, хто очікує ці дані
        {
            QHash<QString, QVector<double> > result;
            in >> result;
            emit scaleResult(plcName,result);
        }
            break;

        case UDP_SQL_RESULT:
        {
            QList<QStringList> res;
            qint32 iD;
            in >> iD >> res;
            // qDebug() << "UDP_SQL_RESULT" << iD;
            emit resultRemoteQuery(iD,res);

        }
             break;
        case UDP_SQL_ERROR:
        {
            qint32 id;
            QString errMsg;
            in >> id >> errMsg;
            emit reslutRemoteError(id,errMsg);
        }
            break;
        case TCP_FINDTABLE: //відповість тільки той, в кого є потрібна таблиця, відповідь повторює запит
        {
            QString tblName;
            qint16 active;
            in >> tblName ;
            if(!in.atEnd()) // правка вносилася на ходу тому для сумісності треба додати це
                in >> active;
            else
                active=0;
            //qDebug() << plcName << " TCP_FINDTABLE " << tblName << s->peerAddress();

            emit tableFinded(plcName,tblName,active);
        }
            break;
        }
    }

}

void AuTcpTransmitter::sendScaleQuery(const QString& plcName)
{
    QByteArray d;
    QDataStream out(&d,QIODevice::WriteOnly);
    out.setVersion(QTVER);

    out << plcName << quint8(UDP_ALLPARAM_SEND) << QRandomGenerator::global()->generate() ; // останнє - це сіль, для того щоб гуляла контрольна сума
    quint16 crc=qChecksum(d.data(),d.size());
    out << crc;

    sendPkt(plcName,d);
}

void AuTcpTransmitter::sendSqlQuery(const QString& plcName,const QString& table,const QString& query, int index)
{
    QByteArray d;
    QDataStream out(&d,QIODevice::WriteOnly);
    out.setVersion(QTVER);

    out << plcName << quint8(UDP_SQL_QUERY) << table << query << index ; // із сіллю

    sendPkt(plcName,d);
}

/* Алгоритм пошуку хоста, в бд якого є необхідна таблиця
 * шукаємо унікальні адреси  хостів в scadaAddr ( туди заносяться всі ті, хто хоч щось мультикастить, див. AuUdpTransiver::recvData() десь в кінці)
 * і надсилаємо всім запит TCP_FINDTABLE по протоколу TCP. тут би також було б гарно використати мультикаст але воно може і не спрацювати.
 * до того ж прийдеться налаштовувати роутери на пропуск мультикаста в обидва боки.
 * хто отримав ці запити перевіряє свої бази даних і якщо вказана таблиця присутні, відповідають тим же.
 * всі інші закривають з'єднання залишивши запит без відповіді.
 *
*/

void AuTcpTransmitter::findTable(const QString &tableName )
{
    // знайти унікальні адреси комп'ютерів
    //qDebug() << "scadaAddr\n" << scadaAddr;
    QHash<QHostAddress,QString> list;// це зворотній хеш
    foreach(QString p,scadaAddr.keys())
    {
        list[scadaAddr[p]]=p; // поміняв ключ і значення місцями
    }

    // qDebug() << "list\n" << list;

    foreach(QString plcName,list.values())
    {
        QByteArray d;
        QDataStream out(&d,QIODevice::WriteOnly);
        out.setVersion(QTVER);

        out << plcName << quint8(TCP_FINDTABLE) << tableName ; // із сіллю

        sendPkt(plcName,d);
    }


}


