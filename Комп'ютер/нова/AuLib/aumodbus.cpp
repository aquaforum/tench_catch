#include "aulibdefs.h"
#include "auplcdrive.h"
#include "aumodbus.h"
#include "autag.h"
#include "auplc.h"

#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>
#include <QFile>


// ці імена зв'язані з роботою модбаса
#define GETMCR 1
#define GETIX 2

#define PUTSCR 5
#define PUTMCR 15

#define GETMHR 3
#define PUTSHR 6
#define PUTMHR 16

AuModbus::AuModbus(AuBase &base, quint16 iX, qint16 addOffset, QObject *parrent) : AuPlcDrive(base,iX,parrent),
    nPort(502),
    unitId(1),
    cklCount(0),
    offset(addOffset) // !!!!!!!!!!!!!!!!!!!!!!!!!!

{
    linkOk=false;

    //qDebug() << "addOffset " << addOffset;

    // з'єднання сигналі і слотів переписано на новий стиль Qt5, старий стиль залишено закоментареним

    // теймер паузи між спробами встановити нове з’єднання
    connWait=new QTimer(this);
    connWait->setInterval(10000);
    //connect(connWait,SIGNAL(timeout()),this,SLOT(slotNewConnect()));
    connect(connWait,&QTimer::timeout,this,&AuModbus::slotNewConnect);

    // таймер для відліку таймайту з’єднання
    connTimeout=new QTimer(this);
    connTimeout->setInterval(10000);
    //connect(connTimeout,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    connect(connTimeout,&QTimer::timeout,this,&AuModbus::slotTimeout);


    // сокет для здійснення обміну даними
    pS=new QTcpSocket(this);
    //connect(pS,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(pS,&QTcpSocket::connected,this,&AuModbus::slotConnected);

    //connect(pS,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
    connect(pS,&QAbstractSocket::errorOccurred,this,&AuModbus::slotError);

    //connect(pS,SIGNAL(readyRead()),this,SLOT(slotRead()));
    connect(pS,&QTcpSocket::readyRead,this,&AuModbus::slotRead);

    //connect(pS,SIGNAL(disconnected()),this,SLOT(slotDisconnect()));
    connect(pS,&QTcpSocket::disconnected,this,&AuModbus::slotDisconnect);

    //qDebug() << myName;
    setObjectName(myName);

    loadList(base[iX].fileList());// завантажити список тегів і сформувати чергу запитів для отримання даних з контролера

    // підключаємось до бази для сигналізації про отримання свіжих даних з контролера
    connect(this,static_cast<void (AuModbus::*)()>(&AuModbus::updateData),&base,&AuBase::slotUpdateData);
    connect(this,static_cast<void (AuModbus::*)()>(&AuModbus::updateData),&base[iX],&AuPlc::setData);


    foreach (AuTag *tag, base[iX].tags()) { // підключаємось до тегів для відправки даних в контролер
        connect(tag,QOverload<qint32,QVector<qint16> &>::of(&AuTag::sendData),this,&AuModbus::setData);
    }

    base[iX].setActive(true); // встановити прапор активного обміну це потрібно для передачі шкал в мережу


    // qDebug() << "AuModbus" << plcData.size();


}

AuModbus::~AuModbus()
{
   pS->close();
}

void AuModbus::setData(qint32 address ,QVector<qint16> &data)
{
    // qDebug() << "AuModbus::setData" << address << data;
    QByteArray q; // тут готуєму дані для надсилання в контролер
    QDataStream qry(&q,QIODevice::WriteOnly); // а цим власне готуємо

    qry.setByteOrder(QDataStream::BigEndian); // встановити прорядок байт як у контролері
    if(address<0)
    {
        qry << qint16(0) << qint16(0); // TCP заголовок
        if(false) // data.size()==1)
        {
            qry << qint16(6);  // закінчення ТСР заголовку
            qry << unitId << qint8(PUTSCR) << qint16(-address-offset) << qint16(data[0]!=0?(0xFF00):0);
        }
        else
        {
            qint8 count=(data.size()/8)+(data.size()%8!=0?1:0); // розрахувати кількість байь
            qry << qint16(7+count); // закінчити заголовок TCP
            qry << unitId << qint8(PUTMCR) << qint16(-address-offset) << qint16(data.size()) << qint8(count) ;
            QByteArray val;
            val.fill(0,count);
            int i,j;
            for(int iX=0;iX<data.size();++iX)
            {
                i=iX/8;
                j=iX%8;
                val[i] = val[i] |  (data[iX]!=0?(1<<j):0);
            }
            //qDebug() << val;
           foreach(quint8 v, val)
           {
               qry << v;
           }
        }
        //qDebug() << q;
    }
    else // якщо слова
    {
        // сформувати запит до контролера
        qry << qint16(0) << qint16(0) << qint16((data.size()<<1)+7)  // TCP заголовок
           << unitId << qint8(PUTMHR)               // модбас заголовок
           << qint16(address-offset)             // адреса даних
              // оце бісове "-1" не дає мені спокою!!!!
              // треба перевірити чи ця хрінь працює на шнайдері і сіменсі
              //  і здається мені що воно таки працює на частотниках міцубісі

           << qint16(data.size())                     // довжина даних
           << qint8(data.size()<< 1);                 // кількість байт
        foreach(qint16 t,data)
        {
             qry << t; // завантажити дані
        }
    }

    query_queue.enqueue(q); // поставити в чергу на відправку в контролер

    // було б непогано для того щоб воно менше миготіло записати data в plcData
    // але нажаль address це адреса модбаса а не індекс в plcData

}


QVector<qint16> &AuModbus::data()
{
    return plcData;
}

/*
int AuModbus::loadList(QString list)
{

    return 0;
}


int AuModbus::start()
{
    return 0;
}

*/

void AuModbus::slotConnected () // приєдналися
{
    //connSend->start();
    connTimeout->start();
    nLen=0;
    // qDebug() <<  "Connected to host" << name;
    // slotSend(); // розпочати обмін
    pS->write(query_list[0]);
    nC=0;
    emit Alert(QString("Connected to PLC: %1:%2").arg(myName).arg(nPort));
    linkOk=true;
}

void AuModbus::slotNewConnect()
{
    connWait->stop();
    pS->connectToHost(myName,nPort);
}

void AuModbus::slotTimeout() // таймаут отримання даних від сервера
{
//   connSend->stop();
    connTimeout->stop();
    connWait->start();
    pS->close();
    emit Alert(QString("Connection to PLC lost: %1:%2").arg(myName).arg(nPort));
    qDebug() << QString("Connection to PLC lost: %1:%2").arg(myName).arg(nPort);
    linkOk=false;

}

void AuModbus::slotDisconnect() // відєднання зі сторони сервера
{
    //connSend->stop(); // зупинити таймер, коли від’єднано немає сенсу слати запити
    pS->close();
}

void AuModbus::slotError(QAbstractSocket::SocketError)
{
    //connSend->stop();
    connTimeout->stop();
    connWait->start();
    qDebug() << "Connection error" << objectName();
    emit Alert(QString("Connection to PLC error: %1:%2. %3").arg(myName).arg(nPort).arg(pS->errorString()));
    pS->close();
}



void AuModbus::slotRead()
{
    QDataStream in(pS); // це використовується для читання даних з сокета
    qint16 v16; // ця змінна для самих даних
    qint8  as,fc; // це якісь заголовки
    quint8 bc; //

    in.setByteOrder(QDataStream::BigEndian); // встановити порядок байт

    for(;;)
    {
        if(nLen==0) // читати заголовок
        {
            if(pS->bytesAvailable()<6) // якщо тут мало байт
            {
                break;
            }
            // розбираємо заголовок Modbus TCP
            in >> Index; // id транзакції воно ж зміщення індекса в масиві даних
            in >> v16; // id протоколу
            in >> v16; // довжина пакунка
            nLen=v16;
        }
        if(pS->bytesAvailable()<nLen)
        {
            break;
        }
        // розбираємо пакунок Modbus

        linkOk=true; // якщо прийняли пакунок то скоріш за все зв'язок з контролером є

        // отримано весь пакунок, розібрати на частини
        // заголовок пакунка
        in >> as; // адреса ведомого
        in >> fc; // код функції
        //qDebug() << "Start packet proccess Index" << Index << "nLen" << nLen << "as " << as << "fc" << fc;
        switch(fc) // розібрати по коду функції
        {
            case GETMCR:
            case GETIX:
                in >> bc; // прочитати кількість байт
                // qDebug() << fc << bc  << Index;
                for(int i=0;i<bc;++i)
                {
                    qint8 v;
                    in >> v; // a тепер це треба правильно розпакувати..........
                    // qDebug() << v;
                    for(int j=0;j<8;++j)
                    {
                        int ix=i*8+j;
                        if(ix<dataLen[nC])
                        {
                            plcData[Index+ix]=qint16(v&0x01?-1:0); // все так от тільки чи влізе воно?
                        }
                        else
                        {
                            break;
                        }
                            v>>=1;
                    }
                }
                break;

            case GETMHR:
                in >> bc; // прочитати кількість байт
                bc >>= 1; // розрахувати кількість слів

                for(int i=0;i<bc;++i) // в циклі
                {
                        in >> v16; // прочитати слова
                        plcData[Index+i]=v16; // та записати в масив даних
                }

                break;

            case PUTSCR: // це відповідь на відправку даних в контролер, просто зчитати і нічого не робтити
            case PUTMCR:
            case PUTSHR:
            case PUTMHR:
                // qDebug() << "Ok fc "<< fc << "nLen " << nLen;
                for(int i=2;i<nLen;++i)
                    in >>bc;
                break;
            default: // якщо якась неочікувана функція то просто очистити весь буфер
                qDebug() << "Uncnown fc" << fc;
                for(int i=2;i<nLen;++i)
                    in >>bc;
        }

        // відправити наступний запит
        ++nC; // перейти до наступного запиту
        while(nC<query_list.size()) // ?? чому тут while а не if ?
            {
                local_read[nC]--; // порахувати період відправки зпитів
                if(1>local_read[nC]) // якщо 0 - відправити запит в контролер
                {
                    pS->write(query_list[nC]);
                    // qDebug() << query_list[nC];
                    local_read[nC]=query_read[nC];
                    break; // !!! тому що з цього циклу вихід тут
                }


                ++nC;
            }

        if(nC==query_list.size()) // при виконанні кругу запитів на отримання даних смикнути про отримання даних це потенційно мало б зменшити проморгування індикаторів при введенні даних
        {
            emit updateData();
            emit updateData(plcData);
        }
        // qDebug() << "nC " << nC <<" query_list.size() " << query_list.size();


        if(! (nC < query_list.size())) // перевірити чергу на надсилання даних в контролер
        {
            //qDebug() << "Process query queue" ;
            if(query_queue.isEmpty()) // перевірити чергу при умові що інших запитів немає.
            {
//                emit updateData();
//                emit updateData(plcData);
                ++cklCount;

                    pS->write(query_list[0]);
                    nC=0;

            }
            else
            {
                // qDebug() << query_queue;

                pS->write(query_queue.dequeue()); // якщо не пуста, передати

            }
        }
        nLen=0;
    }
    connTimeout->stop();
    connTimeout->start();
}

/*
 * результатом роботи наступної функції є заповнення двох масивів
 * query_list - це запити до контролера
 * query_read - відповідний період виконання цих запитів
*/
int AuModbus::loadList(QString fileName)
{

    QFile f(fileName);
    int i;
    QString s;
    QStringList sl;

    int wc=0, wc_last=0; // лічильник слів

    qint16 next_addr=0,current_addr=0; //адреси

    qint16 current_len=0,packet_len=0; // поточна довжина

    qint16 current_rf=0,last_rf=0; // ReadFlag прапори кратності читання

    QByteArray query;// змінні для формування поточного запиту
    QDataStream qry(&query,QIODevice::WriteOnly);

    // QHash<QString,QString> tag_scale; // тут будуть теги, які шкалюються по іншому параметру

    QStringList ft;
    ft << "Integer" << "Bool" << "Real" << "Timer" << "Long" << "EBOOL" << "QX" << "IX" << "MX";
    //     0            1         2         3          4         5          6       7       8
    int current_ft=0,last_ft=0; // пити полів, для виявлення EBOOL , QX , IX

    qry.setByteOrder(QDataStream::BigEndian); // встановити порядок байт


    //qDebug() << "file " << fileName;

        // очистити все на випадок повторного завантаження
        //tags.clear();

        query_list.clear();
        query_read.clear();
        local_read.clear();

    if(f.open(QIODevice::ReadOnly))
    {
        for(i=0;!f.atEnd();++i)
        {
            s=QString::fromUtf8(f.readLine()).trimmed(); //читати рядок тегів із файлу
            sl= s.split("\t"); // розбити на поля
            if(sl.size()>3) // якщо є всі поля  треба індекс[1] тип даних[2] інтервал читання [3]
            {
                if(sl[2]=="MX")
                    current_addr=sl[1].split('.')[0].toInt()/2; // індекс, тут би для повного щася треба б було перевірити чи воно правильно перетворилося на число
                else
                    current_addr=sl[1].toInt(); // індекс, тут би для повного щася треба б було перевірити чи воно правильно перетворилося на число

                current_rf=sl[3].toInt(); // читати кратність читання
                wc_last=wc; // це потрібно для правильного формування поля id транзакції яке містить зміщення індексу в масиві даних
                            // метод не зовсім стандартний, на інших контролерах може і не буде працювати
                            // перевірено на I-8000 та старих версіях шнайдера, наче і на сіменсі спрацювало

                // розпізнати типи даних, порахувати слова
                if(sl[2]=="Integer" || sl[2]=="Bool" )
                {
                    ++wc;
                    current_len=1;
                }
                else if (sl[2]=="Real" || sl[2]=="Timer" || sl[2]=="Long" )
                {
                    wc+=2;
                    current_len=2;
                }
                else if(sl[2]=="EBOOL") // це спеціально для шнайдера
                {
                    ++wc;
                    current_len=1;
                    // тут треба зімітувати дірку
                }
                else if(sl[2]=="QX") // це спеціально для M241
                {
                    ++wc;
                    current_len=1;
                }
                else if(sl[2]=="IX") // це спеціально для M241
                {
                    ++wc;
                    current_len=1;
                }

                else if(sl[2]=="MX") // це спеціально для M241
                {
                    ++wc;
                    current_len=1;
                    // зайти всередину  і порахувати всі МХ-и
                    loadMx(f,current_addr);
                }

                else // невідомий тип даних
                {
                    qDebug() << tr("Unknown data type") << sl[2] << sl;
                    ::exit(1);
                }

                current_ft=ft.indexOf(sl[2]); // визначити потоний тип поля
                packet_len+=current_len; // порахувати поточну довжину пакунку

                if(packet_len>124 // довжина пакунку перестрибнула 124 слова
                        || current_addr>next_addr  // виявлено "дірку"
                        || current_rf!=last_rf  // виявлено зміну кратності читання

                        || (current_ft==5 && last_ft!=5)  // цих дві умови призначені для виявлення зміни типу змінної на EBOOL
                        || (current_ft!=5 && last_ft==5)

                        || (current_ft==6 && last_ft!=6)  // цих дві умови призначені для виявлення зміни типу змінної на QX
                        || (current_ft!=6 && last_ft==6)

                        || (current_ft==7 && last_ft!=7)  // цих дві умови призначені для виявлення зміни типу змінної на QX
                        || (current_ft!=7 && last_ft==7)
                        )
                {
                    if(query.size()) // якщо щось є,
                    {
                        query_list <<  query; // зберегти
                        dataLen << packet_len;
                    }

                    // підготуватися до нового запиту
                    qry.device()->seek(0);
                    query.clear();
                    // сформувати заголовок
                    packet_len=current_len;

                    qint8 code=GETMHR;
                    if(sl[2]=="EBOOL" || sl[2]=="QX")
                        code=GETMCR;
                    if(sl[2]=="IX")
                        code=GETIX;

                    qry << qint16(wc_last) << qint16(0) << qint16(6)  // TCP заголовок
                        << unitId  // адреса Modbus, зазвичай 1 але для ЧП міцубісі відрідняється
                        <<  code // код функції
                        << qint16(current_addr-offset);  //адреса для читання
                                                                                 //^^^^^^^^^^^^^^^^^^^^^^ можливо для інших контролерів цей декримент непотрібен
                    query_read << current_rf; //прапор read на пакунок
                    local_read << 0;
                }
                else // в іншому разі поновити дані про довжину.
                {
                    qry.device()->seek(query.size()-2);
                }
                qry << packet_len; //додати довжину пакунка
                next_addr=current_addr+current_len; // розрахувати новий наступний очікуваний адрес
                last_rf=current_rf;
                last_ft=current_ft;

            }
        }

        if(query.size()) // зберегти останній запит.
        {
            query_list << query;
            dataLen << packet_len;
            query_read << current_rf;
        }

        f.close();

        // qDebug() << query_list.size() << "\n" <<  query_list;

        return i;
    }
    else
    {
        return 0;
    }

}

void AuModbus::loadMx(QFile &file,qint16 addr )
{
    while(!file.atEnd())
    {
        int pos=file.pos();
        QString s=file.readLine();
        QStringList sl=s.split('\t');
        if(sl[2]!="MX")
        {
            file.seek(pos);
            break;
        }
        qint16 myAddr= sl[1].split('.')[0].toInt()/2;
        if(myAddr!=addr)
        {
            file.seek(pos);
            break;
        }
    }
    return;
}


void AuModbus::setHostName(QString hostName)
{
    myName=hostName;
    setObjectName(hostName);
}


int AuModbus::start()
{
    // тут би треба зробити якісь додаткові перевірки
    pS->connectToHost(myName,nPort);
    return 0;
}


