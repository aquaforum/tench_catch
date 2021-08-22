#include "autcpreceiver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <aulib.h>
#include <ausqlquery.h>

#include <QDir>
#include <QApplication>

AuTcpReceiver::AuTcpReceiver(AuBase &source, QObject *parent) : QObject(parent)
  ,myTcpServer(new QTcpServer(this))
  ,src(source)
  ,table(QStringLiteral(""))
  ,query(QStringLiteral(""))
  ,rawSqlIx(0)

{
    // далі налаштуємо TCP
    // qDebug() << "AuTcpReceiver listen: " <<
    myTcpServer->listen(QHostAddress::AnyIPv4,PORTN);
    connect(myTcpServer,&QTcpServer::newConnection,this,&AuTcpReceiver::slotNewConnection);

}

void AuTcpReceiver::addPlcDrive(QString plcName, AuPlcDrive* plcDrive)
{
    myPlcDrive[plcName]=plcDrive;
}

void AuTcpReceiver::slotNewConnection()
{
    QTcpSocket *pClientSocket = myTcpServer->nextPendingConnection();
    // qDebug() << pClientSocket;
    connect(pClientSocket,&QTcpSocket::readyRead,this,&AuTcpReceiver::slotRecvTcp);
    connect(pClientSocket,&QTcpSocket::disconnected,this,&AuTcpReceiver::slotDisconnect);

    pktLen[pClientSocket]=0; // приготуватися до прийому даних

}

void AuTcpReceiver::slotDisconnect()
{
    QTcpSocket *s=qobject_cast<QTcpSocket*>(sender());
    pktLen.remove(s);
    // qDebug() << s << " disconnected";
    s->deleteLater();

}

void AuTcpReceiver::slotRecvTcp()
{
    QTcpSocket *s=qobject_cast<QTcpSocket*>(sender());
    // qDebug() << "packetReceived" << s->peerAddress().toString() <<" bytesAvailabl:"<< s->bytesAvailable() ;

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
        quint16 address;
        QVector<qint16> rData;
        quint16 pktCrc;
        qint8 code=-1;

        in >> plcName;
        in >> code;
        switch (code) {
        default:
            emit Alert(QStringLiteral("AuTcpReceiver unknown operation code:%1").arg(code));
            break;

        case UDP_PUSH: // приймач згачень окремих тегів, відповіді немає.
            in >> tagName; //
            in >> address;
            in >> rData;
            in >> pktCrc;
            try {
                for(int i=0;i<rData.size();++i) // скопіювати в оригінальний масив
                    src[plcName].data()[src[plcName][tagName].index()+i]=rData[i];
                // тут треба постукатись в модбас чи ще куди.....
                myPlcDrive[plcName]->setData(address,rData);
            }
            catch (int v) {
                switch (v) {
                case THROWN_NOPLC:
                    emit Alert(QStringLiteral("No plcName found in code UDP_PUSH of AuTcpReceiver: %1").arg(plcName));
                    break;
                case THROWN_NOTAG:
                    emit Alert(QStringLiteral("No tagName found in code UDP_PUSH of AuTcpReceiver: plc:%1 tag%2").arg(plcName).arg(tagName));
                    break;
                default:
                    emit Alert(QStringLiteral("Unknown error in code UDP_PUSH %1").arg(v));
                    break;
                }
            }
            s->close(); // завершити
            break;


        case UDP_ALLPARAM_SEND: // упаковка параметрів шкальованих тегів та відправка в мережу
            { // прочитати сіль
                int salt;
                in >> salt;
            }
            in >> pktCrc;
            // qDebug() << "UDP_ALLPARAM_SEND. Sender: " << dGram.senderAddress() << " pktCrc: " << pktCrc << " lCrc: " << lCrc;
        try {
            if(src[plcName].active()) // перевірити чи є в мене такий контролер
            {
                QHash<QString, QVector<double> > param;
                foreach(QString tagName,src[plcName].tagsList())
                {
                    if(src[plcName][tagName].scaleType()==tagScale::SET)
                    param[tagName] << src[plcName][tagName].zero()
                                   << src[plcName][tagName].full()
                                   << src[plcName][tagName].baseScale()
                                   << src[plcName][tagName].deciaml();
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out.setVersion(QTVER);

                out << plcName << quint8(UDP_ALLPARAM_RECV) << param ; // виплюнути в наступний кейс+сіль

                QByteArray buff=qCompress(data);
                quint32 len=buff.size(); // вирахув
                buff.prepend((char*)&len,TCP_HEADER_LEN);

                s->write(buff);
            }
            }
            catch (int v)
            {
                emit Alert(QStringLiteral("No plc found in code UDP_ALLPARAM_SEND of AuTcpReceiver: %1").arg(plcName));
            }
            s->close(); // завершити
            break;
        case UDP_SQL_QUERY: // ця фігня створить запит до локальної бази даних, хтось повинен передати відповідь назад в  мережу
        {
            QString table,query;
            qint32 iD;
            in >> table >> query  >> iD;
            // qDebug() << "UDP_SQL_QUERY " << plcName << table << query << iD;
            try {
                if(src[plcName].active()) // якщо є такий контролер перевірити щоб не було петлі якщо запит прийшов з мережі а контролера немає
                {
                    // обов'язкова частина процедури
                    AuSqlQuery *sqlQry= new AuSqlQuery(DBFILE);
                    connect(sqlQry,&AuSqlQuery::finished,this,&AuTcpReceiver::sqlResultSend);
                    // а це вже можна робити за потреби
                    connect(sqlQry,&AuSqlQuery::error,this,&AuTcpReceiver::sqlErrorSend);
                    // зберегти отримані дані
                    sqlQry->setPlcName(plcName);
                    sqlQry->setTable(table);
                    sqlQry->setId(iD);
                    sqlQry->setSocket(s);

                    sqlQry->exec(query); // запуск виконання запуту в паралельному потоці,
                }

            } catch (int ) { // якщо такого контролера взагалі немає то попадаємо сюти і загалі нічого не робимо
                emit Alert(QStringLiteral("No plc in UDP_SQL_QUERY: %1").arg(plcName));
            }
        }
            break;

        case TCP_FINDTABLE:
        {
            QString tblName;
            in >> tblName;
            QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME);
            if(dbs.isValid() && dbs.open())
            {
                if(dbs.tables().contains(tblName))
                {  // сформувати відповідь.
                    QByteArray data;
                    QDataStream out(&data,QIODevice::WriteOnly);
                    out.setVersion(QTVER);

                    qint16 active=0;
                    if(src.plcList().keys().contains(tblName)) // якщо в наборі є контролер для відповідної таблиці
                    {
                        active=src[tblName].active()?-1:0; // перевірити його статус
                    }
                    out << plcName << quint8(TCP_FINDTABLE) << tblName << qint16(active) ; // виплюнути в наступний кейс+сіль

                    QByteArray buff=qCompress(data);
                    quint32 len=buff.size(); // вирахув
                    buff.prepend((char*)&len,TCP_HEADER_LEN);

                    s->write(buff);
                    // qDebug() << plcName << " TCP_FINDTABLE " << tblName << s->peerAddress();
                }
                dbs.close();

            }

        }
            s->close();
            break;

        } // switch(code)
    } // for(;;)

}

/*


        // це для прикручування мережевого інтерфейсу до SQLIte`a
        // як на мене досить простий і лаконісний код вийшов
        case UDP_RAWQUERY:
            in >> query >> pktCrc;
            qDebug() << "UDP_RAWQUERY " << plcName << query << pktCrc;

            table=plcName;
            rawSqlIx=pktCrc;
            if(pktCrc!=lCrc) // якщо пакунок не дубль
                QTimer::singleShot(0,this,&AuUdpTransiver::slotRawSqlQuery); // це мало б запуститися зраза після завершення цього

            lastCrc[dGram.senderAddress()]=pktCrc; // для перевірки на дубль
            break;
        case UDP_RAWRESULT:
            in >> pktCrc;
            if(pktCrc!=lCrc)
                emit rawSqlResult(dGram);

            lastCrc[dGram.senderAddress()]=pktCrc;
            break;
        case UDP_RAWERROR:
            in >> pktCrc;
            if(pktCrc!=lCrc)
                emit rawSqlError(dGram);

            lastCrc[dGram.senderAddress()]=pktCrc;
            break;

*/

void AuTcpReceiver::slotSendTcp()
{

}

/*
void AuTcpReceiver::slotHtrDataUdpSend(int iD, QByteArray dataRaw, int row, int col)
{
    if(iDs.keys().contains(iD)) // якщо робили цей запит
    {
        QByteArray data;
        QDataStream out(&data,QIODevice::WriteOnly);
        out.setVersion(QTVER);

        out << iDs[iD] << qint8(UDP_SQL_RESULT) << iD << row << col << dataRaw; // яким чином приймач знатиме що список тегів не змінився ?

        quint16 crc= qChecksum(data.data(),data.size()); //вчепити контрольну суму для запобіганю думлювання пакунків
        out << crc;

        // sSock->writeDatagram(qCompress(data),addr,PORTN);
        // qDebug() << "SQL pack size " << qCompress(data).size() << data.size();


        iDs.remove(iD);
    }
}
*/





/*
void AuTcpReceiver::slotRawSqlQuery()
{
    QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME,true); //знайти з’єдання logging
    if(dbs.isValid()) // якщо така база взагалі істує
    {
        if(dbs.open())
        {
            if(dbs.tables().contains(table)) // в даному випадку тут назва таблиці
            {
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out.setVersion(QTVER);
                out << table ;

                QSqlQuery qry(dbs);
                if(qry.exec(query))
                {
                    int fieldCount=qry.record().count();
                    QList<QStringList> res;
                    while(qry.next())
                    {
                        QStringList row;
                        for(int i=0;i<fieldCount;++i)
                            row << qry.value(i).toString();
                        res << row;
                    }
                    out << qint8(UDP_RAWRESULT) << rawSqlIx << res; // яким чином приймач знатиме що список тегів не змінився ?
                }
                else
                {
                    out << qint8(UDP_RAWERROR) << rawSqlIx << qry.lastError().text();
                }
                quint16 crc= qChecksum(data.data(),data.size()); //вчепити контрольну суму для запобіганю думлювання пакунків
                out << crc;
                //sSock->writeDatagram(qCompress(data),addr,PORTN);                    //qDebug() << "SQL pack size " << qCompress(data).size() << data.size();
            }
            dbs.close();
            rawSqlIx=0;
        }

    }
}
*/


// цей метод отримає дані з результатами SQL запиту від AuSqlQuert та відправить їх в мережу
void AuTcpReceiver::sqlResultSend(const QList<QStringList> &sqlResult)
{
    AuSqlQuery *res=qobject_cast<AuSqlQuery*>(sender());

    // qDebug() << sqlResult;

    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out.setVersion(QTVER);

    out << res->plcName() << quint8(UDP_SQL_RESULT) << res->id() <<  sqlResult; // виплюнути в наступний кейс+сіль

    QByteArray buff=qCompress(data);
    quint32 len=buff.size(); // вирахув
    buff.prepend((char*)&len,TCP_HEADER_LEN);

    res->socket()->write(buff);
    res->socket()->close();

}

void AuTcpReceiver::sqlErrorSend(const QString &sqlErrMsq)
{
    AuSqlQuery *res=qobject_cast<AuSqlQuery*>(sender());

    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out.setVersion(QTVER);

    out << res->plcName() << quint8(UDP_SQL_ERROR) << res->id() <<  sqlErrMsq; // виплюнути в наступний кейс+сіль

    QByteArray buff=qCompress(data);
    quint32 len=buff.size(); // вирахув
    buff.prepend((char*)&len,TCP_HEADER_LEN);

    res->socket()->write(buff);
    res->socket()->close();

}
