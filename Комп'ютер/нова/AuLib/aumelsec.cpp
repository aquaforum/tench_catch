#include "aumelsec.h"

#include "aulibdefs.h"
#include "auplcdrive.h"
#include "autag.h"
#include "auplc.h"
#include "aubase.h"

#include <QString>
#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>
#include <QFile>


AuMelsec::AuMelsec(AuBase &base,quint16 index, QObject *parrent): AuPlcDrive(base,index,parrent),
    nPort(5002) ,nC(0),plcAddr(1) // кноструктор, треба уточнити
{
    linkOk=false;

    // з'єднання сигналі і слотів переписано на новий стиль Qt5, старий стиль залишено закоментареним

    // теймер паузи між спробами встановити нове з’єднання
    connWait=new QTimer(this);
    connWait->setInterval(10000);
    //connect(connWait,SIGNAL(timeout()),this,SLOT(slotNewConnect()));
    connect(connWait,&QTimer::timeout,this,&AuMelsec::slotNewConnect);

    // таймер для відліку таймайту з’єднання
    connTimeout=new QTimer(this);
    connTimeout->setInterval(10000);
    //connect(connTimeout,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    connect(connTimeout,&QTimer::timeout,this,&AuMelsec::slotTimeout);


    // сокет для здійснення обміну даними
    pS=new QTcpSocket(this);
    //connect(pS,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(pS,&QTcpSocket::connected,this,&AuMelsec::slotConnected);

    //connect(pS,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
    connect(pS,&QAbstractSocket::errorOccurred,this,&AuMelsec::slotError);

    //connect(pS,SIGNAL(readyRead()),this,SLOT(slotRead()));
    connect(pS,&QTcpSocket::readyRead,this,&AuMelsec::slotRead);

    //connect(pS,SIGNAL(disconnected()),this,SLOT(slotDisconnect()));
    connect(pS,&QTcpSocket::disconnected,this,&AuMelsec::slotDisconnect);

    //qDebug() << myName;
    setObjectName(myName);

    loadList(base[iX].fileList());// завантажити список тегів і сформувати чергу запитів для отримання даних з контролера

    // підключаємось до бази для сигналізації про отримання свіжих даних з контролера
    connect(this,static_cast<void (AuMelsec::*)()>(&AuMelsec::updateData),&base,&AuBase::slotUpdateData);
    connect(this,static_cast<void (AuMelsec::*)()>(&AuMelsec::updateData),&base[iX],&AuPlc::setData);


    foreach (AuTag *tag, base[iX].tags()) { // підключаємось до тегів для відправки даних в контролер
        connect(tag,QOverload<qint32,QVector<qint16> &>::of(&AuTag::sendData),this,&AuMelsec::setData);
    }

    base[iX].setActive(true); // встановити прапор активного обміну це потрібно для передачі шкал в мережу

    cmdpref["D"]=qint8(0xA8);
    cmdpref["X"]=qint8(0x9C);
    cmdpref["Y"]=qint8(0x9D);
    cmdpref["M"]=qint8(0x90);
    cmdpref["L"]=qint8(0x92);
    // десь тут ще потрібно сформувати пакунок на запити
}

AuMelsec::~AuMelsec() // поки-що тривіальний деструктор
{
    pS->close();
}

void AuMelsec::slotConnected () // приєдналися
{
    //connSend->start();
    connTimeout->start();
    nLen=0;
    qDebug() <<  "Connected to host" << myName;
    // slotSend(); // розпочати обмін
    pS->write(query_list[0]);
    // qDebug() << "query_list[0]" << query_list[0].size();
    nC=0;
    emit Alert(QString("Connected to PLC: %1:%2").arg(myName).arg(nPort));
    linkOk=true;
}

void AuMelsec::slotNewConnect()
{
    connWait->stop();
    pS->connectToHost(myName,nPort);
}

void AuMelsec::slotTimeout() // таймаут отримання даних від сервера
{
//   connSend->stop();
    connTimeout->stop();
    connWait->start();
    pS->close();
    emit Alert(QString("Connection to PLC lost: %1:%2").arg(myName).arg(nPort));
    // qDebug() << QString("Connection to PLC lost: %1:%2").arg(myName).arg(nPort);

}

void AuMelsec::slotDisconnect() // відєднання зі сторони сервера
{
    //connSend->stop(); // зупинити таймер, коли від’єднано немає сенсу слати запити
    pS->close();
}

void AuMelsec::slotError(QAbstractSocket::SocketError)
{
    //connSend->stop();
    connTimeout->stop();
    connWait->start();
    //qDebug() << "Connection error";
    emit Alert(QString("Connection to PLC error: %1:%2. %3").arg(myName).arg(nPort).arg(pS->errorString()));
    pS->close();
}


// виявилося що не получається виконувавти асинхронні запити до контролера I-8000, це не дуже добре.
/*
void AuMelsec::slotSend()
{
#ifdef ASYNC
    //qDebug() << "Start -------------------------------------------------------------------------------";
    if(1>local_read[0])
   {
      pS->write(query_list[0]);
      local_read[0]=query_read[0];
   }
   local_read[0]--;
   nC=0;
#else
    // асинхронне виконання
    //qDebug() << "slotSend";
    for(int i=1;i<query_list.size();++i)
    {
        if(1>local_read[i])
        {
            //qDebug() << i;
            pS->write(query_list[i]);
            local_read[i]=query_read[i];
        }
        local_read[i]--;
    }
#endif
}
*/

void AuMelsec::slotRead()
{
    QDataStream in(pS);
    qint16 v16;
    quint8 v8;

    in.setByteOrder(QDataStream::LittleEndian); // встановити порядок байт

    // qDebug() << "slotRead()" <<  pS->bytesAvailable() ;

    for(;;)
    {

        if(nLen==0) // читати заголовок
        {
            if(pS->bytesAvailable()<9) // якщо тут мало байт
            {
                break;
            }
            // вичитати мусор, можливо воно щось означає...
            in >> v8;
            in >> v16;
            in >> v16;
            in >> v16;

            in >> v16; // вичитати довжину
            nLen=v16; // зкорегувати та зберегти довжину

            // qDebug() << "nLen" << nLen;

        }
        if(pS->bytesAvailable()<nLen)
        {
            break;
        }

        in >> v16; // знову мусор
        // отримано весь пакунок, розібрати на частини
        //qDebug() << "Start packet proccess Index" << Index << "nLen" << nLen << "as " << as << "fc" << fc;

        //тепер тут є чотири варіанти
        // 1 - отримано певну кількісит слів, котрі є регістрами D
        // 2 - отримано певну кількість тетрад (спакованих в байти) котрі є бітами
        // 3 - нічого не отримано, якщо перед цим була передача даних
        // 4 - отримано якесь повідомлення про помилку у відповідь на передачу даних.
        if(nC<query_list.size())  // якщо попереду був пакунок із query_list
        { // тоді треба зберегти отримані від контролера дані
            if(quint8(query_list[nC][18])==0xA8) // що то за дані якщо 0xA8 згачить D-регістри
            {
                for(int i=0;i<(nLen-2)/2;++i) // читати слова
                {
                    in >> v16;
                    plcData[Index[nC]+i]=v16;
                }
            }
            else  // інакще там біти
            {
                for(int i=0;i<nLen-2;++i)
                {
                    in >> v8;
                    plcData[Index[nC]+i*2]=v8&0x10?-1:0;  // якось так.
                    plcData[Index[nC]+i*2+1]=v8&0x1?-1:0;
                }
            }
        }
        else // інакще щоб там не було споржнити до кінця
        {
            while(!in.atEnd()) // чи це спрацює ? треба ретельно перевірити....
                in >> v8;
        }


#ifdef ASYNC
      //qDebug() << "nC " << nC  << "query_list.size()" << query_list.size() ;
        // відправити наступний запит
        ++nC;
        while(nC<query_list.size())
            {
                local_read[nC]--;
                if(1>local_read[nC])
                {
                    pS->write(query_list[nC]);
                    local_read[nC]=query_read[nC];
                    break;
                }
                ++nC;
            }

        if(nC==query_list.size()) // при виконанні кругу запитів на отримання даних смикнути про отримання даних це потенційно мало б зменшити проморгування індикаторів при введенні даних
        {
            emit updateData();
            emit updateData(plcData);
        }

        if(! (nC < query_list.size()))
        {
            //qDebug() << "Process query queue" ;
            if(query_queue.isEmpty()) // перевірити чергу при умові що інших запитів немає.
            {
                    pS->write(query_list[0]);
                    nC=0;

            }
            else
                pS->write(query_queue.dequeue()); // якщо не пуста, передати
        }
#endif
        nLen=0;
    }
    connTimeout->stop();
    connTimeout->start();

    // qDebug() << plcData;

}

int AuMelsec::loadList(QString fileName)
{

    QFile f(fileName);
    int i;
    QString s;
    QStringList sl;
    QString type,type_old;
    int wc=0, wc_last=0; // лічильник слів

    qint16 next_addr=0,current_addr=0; //адреси

    qint16 current_len=0,packet_len=0; // поточна довжина

    qint16 current_rf=0,last_rf=0; // прапори читання
//    qint16 current_ft=0; // пити полів, для виявлення EBOOL

    QByteArray query;
    QDataStream qry(&query,QIODevice::WriteOnly);
    qry.setByteOrder(QDataStream::LittleEndian);

    QHash<QString,QString> tag_scale; // тут будуть теги, які шкалюються по іншому параметру

    QStringList ft;
    ft << "Integer" << "Bool" << "Real" << "Timer" << "Long" << "EBOOL" ;



    //qDebug() << "file " << fileName;

        // очистити все на випадок повторного завантаження
        // tags.clear();

        query_list.clear();
        query_read.clear();
        local_read.clear();

    if(f.open(QIODevice::ReadOnly))
    {
        for(i=0;!f.atEnd();++i)
        {
            s=QString::fromUtf8(f.readLine()).trimmed(); //читати
            //qDebug() << i << s;
            sl= s.split("\t"); // розбити на поля
            if(sl.size()>4) // якщо є всі поля
            {
                s= sl[0]; // назва тега
                //qDebug() << s;

                type_old=type;
                type=sl[1].left(1);
                if(type=="X" || type=="Y") // це кодується в 16-й системі
                    current_addr=sl[1].right(sl[1].size()-1).toInt(0,16);
                else
                    current_addr=sl[1].right(sl[1].size()-1).toInt(); // індекс, тут би для повного щася треба б було перевірити чи воно правильно перетворилося на число

                //tags[s] << wc             // 0-index
                //        << current_addr ; // 1- address

                current_rf=sl[3].toInt();
                wc_last=wc; // це потрібно для правильного формування поля id транзакції яке містить зміщення індексу в масиві даних
                            // метод не зовсім стандартний, на інших контролерах може і не буде працювати

                // розпізнати типи даних
                if(type=="D")
                {
                    if(sl[2]=="Integer"  )
                    {
                        ++wc;
                        current_len=1;
                    }
                    else if (sl[2]=="Real" || sl[2]=="Timer" || sl[2]=="Long" )
                    {
                        wc+=2;
                        current_len=2;
                    }
                    else
                    {
                        qDebug() << tr("Unknown data type") << sl[2] << sl;
                        ::exit(1);
                    }
                }
                else if(type=="M" || type=="L" || type=="X" || type=="Y") // це точно байт
                {
                    ++wc;
                    current_len=1;
                    // тут треба зімітувати дірку
                }
                else // невідомий тип даних
                {
                    qDebug() << tr("Unknown data type") << type << sl[2] << sl;
                    ::exit(1);
                }

                //current_ft=ft.indexOf(sl[2]);
                packet_len+=current_len;

                if( // packet_len>124 ||
                   current_addr>next_addr ||         // виявити дірки,
                   current_rf!=last_rf ||            // межі пакунків,
                   type != type_old )                 // зміна типу
                   // (current_ft==5 && last_ft!=5) ||  // кратність читання
                   // (current_ft!=5 && last_ft==5))    // чи зміну типу
                {
                    // Зберегти індекс в масиві даних
                    Index << wc_last;

                    if(query.size()) // якщо щось є,
                    {
                        query_list <<  query; // зберегти
                        dataLen << packet_len;
                        //qDebug() << packet_len-current_len; //
                        //qDebug() << query;
                    }

                    // підготуватися до нового запиту
                    qry.device()->seek(0);
                    query.clear();
                    // сформувати заголовок
                    packet_len=current_len;
                        // сформувати запит
                        qry << qint8(0x50) << qint8(0)  // subheader
                        << qint8(1)                     // netv No
                        << plcAddr                      // Addres PLC
                        << qint8(0xFF) << qint8(0x03) << qint8(0x0) // не знаю що це
                        << qint8(0x0c) << qint8(0x0)     // data length
                        << qint8(0x30) << qint8(0x0)    // timer
                        << qint8(0x01) << qint8(0x4)    // command
                        << (type=="D" ?qint8(0x0):qint8(1)) << qint8(0x0)     // subcommand
                        << current_addr << qint8(0x0)     // start
                                                        // адреса задається в трома байтами, тут старший завжди нуль, відповідно можна отримати тільки 65536 слів
                        << cmdpref[type] ;                  // Data type
                        //<< qint16(0x0);    // len
                                                                                          //^^^^^^^^^^^^^^^^^^^^^^ можливо для інших контролерів цей декримент непотрібен
                    //qDebug() << qint16(wc_last) << qint16(0) << qint16(6) << qint8(1) <<  qint8(sl[2]=="EBOOL"?GETMCR:GETMHR) << qint16(current_addr-1);
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
                //last_ft=current_ft;


            // цей код би винести в окремий клас



            }
        }

        if(query.size()) // зберегти останній запит.
        {
            query_list << query;
            dataLen << packet_len;
            //qDebug() << packet_len;
            query_read << current_rf;
        }

        f.close();


        return i;
    }
    else
    {
        return 0;
    }

}

void AuMelsec::setHostName(QString hostName)
{
    myName=hostName;
    setObjectName(hostName);
}

void AuMelsec::setPort(int Port)
{
    nPort=Port;
}

int AuMelsec::start()
{
    // тут би треба зробити якісь додаткові перевірки
    pS->connectToHost(myName,nPort);
    return 0;
}


void AuMelsec::setData(qint32 addr,QVector<qint16> &v)
{
   QByteArray q;
   QDataStream qry(&q,QIODevice::WriteOnly);

   qry.setByteOrder(QDataStream::LittleEndian);
   QVector<qint8> vp; // масив, у якому будуть підгтовані для передачі дані
   qint16 pLen,vLen=v.size();
   union
   {
       struct
       {
           quint8 type;
           quint8 symType;
           qint16 address;
       } ;
       qint32 val;
   }tAddr ;

   tAddr.val=addr;

   if(tAddr.type!=0xA8)  // якщо D-регістер
   {
       if(tAddr.type==0x9C) // якщ там X-регісти
           return; // далі можна не продовжувати
       if(v.size()%2) // кількість змінних має бути парною, інакше буде проблема
           v << 0; // дописати нуль, контролер його проігнорує
     for(int i=0;i<v.size();i+=2)
           vp << qint8( (v[i]?0x10:0 )| (v[i+1]?0x01:0)  );

     pLen=vp.size();
     // qDebug() << "vp" <<  vp;
   }
   else
   {
       pLen=v.size()*2;
   }

   //qDebug() << tag << QString("%1").arg(qint32(tags[tag][6])&0xff,2,16,QChar('0')) <<  v;


       qry << qint8(0x50) << qint8(0)                                 // subheader
       << qint8(1)                                                    // netv No
       << plcAddr                                                     // Addres PLC
       << qint8(0xFF) << qint8(0x03) << qint8(0x0)                    // не знаю що це
       << qint16(pLen+12)          // data length
       << qint8(0x30) << qint8(0x0)                                   // timer
       << qint8(0x01) << qint8(0x14)                                  // command
       << ( tAddr.type==0xA8 ?qint8(0x0):qint8(1)) << qint8(0x0)    // subcommand
       // << qint8(0x1) << qint8(0x0)    // subcommand

       << qint16(tAddr.address) << qint8(0x0)     // start
                                                 // адреса задається в трома байтами, тут старший завжди нуль, відповідно можна отримати тільки 65536 слів
       << qint8(tAddr.type)                           // Data type
       << vLen ;                     // Розмір масиву з даними.


        if(tAddr.type==0xA8)
        {
            foreach(qint16 t,v)
            {
             qry << t; // завантажити дані
            }
        }
        else
        {
            foreach(qint8 t,vp)
            {
             qry << t; // завантажити дані
            }
        }



       query_queue.enqueue(q); // поставити в чергу на відправку в контролер
}


