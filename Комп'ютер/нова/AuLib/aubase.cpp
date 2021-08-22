/* цей клас розбирає plc.list який має наступну структуру
 * поля розділені символом ; всі зайві пробіли з початку і вкінці обрізаються
 * список полів наступний
 * 0 - PLCNAME - fqdn контролера
 * 1 - TAGLIST  - файл, який містить список тегів
 * наступні поля можуть бути відсутні
 * 2 - OFFSET - прапор для протоколу модбус який задає зміщення адреси модбас, для і-8000 1 ( за замовчанням ) 0 - для шнайдера
 * 3 - PLCTEXT - текстова назва контролера
 * 4 - HISTPERIOD - період запису даних в історію, за замовчанням 5
 * 5 - HISTDBREAL - прапор запису історії в форматі REAL за замовчанням 0, дані пишуться в INT
 *
*/

#include "aulibdefs.h"
#include "aubase.h"
#include "auplc.h"
#include "auplcdrive.h"
#include "ausqlquery.h"
#include "auudptransiver.h"

#include "autcptransmitter.h"
#include "autcpreceiver.h"

#include <QHostAddress>

#include <QFile>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QDataStream>

#include <QByteArray>
//#include <QCryptographicHash>

#include <QDebug>
#include <QApplication>
#include <QTimer>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlError>

#include <QUdpSocket>
#include <QDataStream>
#include <QNetworkDatagram>

#include <QRandomGenerator>
#include <QList>
#include <QDateTime>
#include <QLocale>

#include "trcdataloader.h"

#include "auprivatedev.h"

// це для спрощення роботи з своїми елементами
#define src (*this)


AuBase::AuBase(QString confFile, QObject *parrent): QObject(parrent)
  , myTransiver(nullptr)
  , myTcpRx(nullptr)
  , myTcpTx(nullptr)
  , lastTime(0)
  , lastTable(QStringLiteral(""))
  , lastPlcName(QStringLiteral(""))
{
    QFile conf(confFile),file;
    conf.open(QIODevice::ReadOnly);
    while(!conf.atEnd())
    {
        QString line = conf.readLine().trimmed();
        if(line.size()>0) // там щось є треба розбирати
        {

            QStringList field= line.split(';');
            for(int i=0;i<field.size();++i)
                field[i]=field[i].trimmed();

            quint16 iX = qChecksum(field[PLCNAME].toLatin1().data(),static_cast<unsigned int>(field[PLCNAME].size()));
            names[field[PLCNAME]]=iX;

            plcS[iX]=new AuPlc(field[PLCNAME],field[TAGLIST],iX);

            if(field.size()>OFFSET)
                plcS[iX]->setOffset(field[OFFSET].toShort());

            plcS[iX]->setText(field.size()>PLCTEXT ?field[PLCTEXT]:field[PLCNAME]);

            if(field.size()>HISTPERIOD) // якщо присутнє поле із інтервалом запису історії,
                plcS[iX]->setInterval(field[HISTPERIOD].toInt()); // заповнити його

            if(field.size()>HISTDBREAL) // dbReal
                plcS[iX]->setDbReal(field[HISTDBREAL].toInt()!=0);

        }
    }


    conf.close();

    htrIx.clear();

}


AuBase::~AuBase()
{
    // тут треба вписати код для збереження даних із plcRawData на диск
    // дані зберігаються в домішній каталог/plcData
    //
    QFile file;
    QDir plcDir;
    if(!plcDir.exists(QDir::homePath()+"/.auscada")) // створити каталог якщо його немає
    {
        plcDir.mkdir(QDir::homePath()+"/.auscada");
    }



    foreach(quint16 iX, plcS.keys())
    {
        delete plcS[iX];
    }
}

/*
QHash<quint16 , QVector<qint16> > &AuBase::plcData()
{
    return plcRawData;
}
*/
void AuBase::setData(quint16 index)
{
    // ця функція викликається по сигналу з UdpTransivera
    // слугує для подальшої пересилки сигналів до AuPlc
    // qDebug() << sender() << index;
    if(plcS.keys().contains(index))
    {
        //qDebug() << sender()->objectName();
        plcS[index]->setData();
    }

    // emit updateData(index);
}


AuPlc &AuBase::operator[](quint16 iX)
{
    if(!plcS.contains(iX))
    {
        //qDebug()<< "AuPlc &AuBase::operator[](quint16 iX) Not plcS iX " << iX;
        //qApp->exit(); // у випадку відсутності даних  завершити роботу програми.
        throw  THROWN_NOPLC;

    }
    return *plcS[iX];
}

AuPlc &AuBase::operator[](QString plcName)
{
    if(! names.contains(plcName))
    {
        //qDebug() << "AuPlc &AuBase::operator[](QString plcName) Not Found:" << plcName;
        //qDebug() << names;
            //qApp->exit();
        throw  THROWN_NOPLC;
    }
    return *plcS[names[plcName]];

}


/*
qint32 AuBase::calcLenRawData(QString file)
{
    QFile list(file);
    list.open(QIODevice::ReadOnly);
    qint32 wordCount=0;

    if(list.isOpen())
    {
        while(!list.atEnd())
        {
            QString line=list.readLine().trimmed();
            QStringList tag=line.split(QRegExp("\\s+"));

            if(tag[2]=="Integer")
                ++wordCount;
            else if(tag[2]=="Bool")
                ++wordCount;
            else if(tag[2]=="Real")
                wordCount+=2;
            else if(tag[2]=="Timer")
                wordCount+=2;
            else if(tag[2]=="Long")
                wordCount+=2;
            else if(tag[2]=="EBOOL")
                ++wordCount;
            else
                ++wordCount;
        }
        list.close();
    }
    else {
        qDebug() << file << "file not found";
    }


    return wordCount;

}
*/


QHash<QString,quint16> &AuBase::plcList()
{
    return names;
}

void AuBase::slotUpdateData()
{
    emit updateData(names[sender()->objectName()]); // відправити сигнал про поновлення даних з контролера

}

// але воно нафіг не потрібне бо qobject_case<AuPlc*>(sender()) вирішує всю проблему
quint16 AuBase::index(AuPlc* tagList)
{
    return plcS.key(tagList);
}


quint32  AuBase::size()
{
    return plcS.size();
}

// ця функція отримує запит діставання даних з історії, якщо вони є локально - то зразу ж їх і дістає
qint32 AuBase::queryHtrData(QString plcName, QString tagList, quint32 startT, qint32 stopT, qint32 iD)
{
    QString sqlQuery=QStringLiteral("SELECT Dt,%1 FROM %2 WHERE Dt BETWEEN %3 AND %4 ORDER BY Dt").arg(tagList).arg(plcName).arg(startT).arg(stopT);
    //qDebug() << sqlQuery;

    int index =iD==0?QRandomGenerator::global()->generate():iD; // згенерувати індекс або зберегти, якщо він не нульовий (з мережі прийшов)

    htrIx << index; // зберегти для proccessQuery

    if(src.plcList().keys().contains(plcName))
    {
        //qDebug() << sqlQuery;

        if(src[plcName].active()) // перевірити де та історія знаходиться
        { // якщо локально - виконати запит прямо тут
            AuSqlQuery *sqlQry = new AuSqlQuery(DBFILE);
            connect(sqlQry,&AuSqlQuery::finishedId,this,&AuBase::processQuery);
            sqlQry->setId(index);
            sqlQry->exec(sqlQuery);

            // QTimer::singleShot(1,this,&AuBase::processLocalQuery);
        }
        else // відправити запит в мережу
        {
            try {
                if(myTcpTx!=nullptr)
                    myTcpTx->sendSqlQuery(plcName,plcName,sqlQuery,index);
                else
                    throw 0;
            } catch (...) {
                qDebug() << "No histori serve found";
            }
        }
        return index;

    }
    else
        return 0; // запит не може бути виконаний бо невідомий контролер
}



void AuBase::processQuery(qint32 id,const QList<QStringList>& resStr )
{
    // qDebug() << resStr;
    sqlStore.clear();
    sqlStore=resStr;

    if(htrIx.contains(id)) // якщо це запит на дані історії
    {                      // тоді їх треба обробити та надіслати за допомогою htrData
        int row=0,col=0;
        QByteArray res;
        row=resStr.size();
        if(row>0) // якщо там є дані
        {
            col=resStr[0].size();
            res.fill(0,row*col*sizeof(qint32));
            qint32 *ptr=(qint32*)res.data();

            QLocale loc1=QLocale::system();
            QLocale loc2=QLocale::c();

            for(int i=0;i<row;++i)
            for(int j=0;j<col;++j)
            {
                bool opRes;
                ptr[i*col+j]=loc1.toDouble( resStr[i][j],&opRes) ; // .toInt();
                if(!opRes)
                {
                    ptr[i*col+j]=loc2.toDouble( resStr[i][j],&opRes) ; // .toInt();
                }
                // qDebug() << resStr[i][j]<<  ptr[i*col+j] << opRes;
            }
        }
        emit htrData(id,res,row,col);
        htrIx.remove(htrIx.indexOf(id)); // обробили, вилучити

    }
    else // інакше це простий запит, вернути як є
    {
        emit sqlData(id,sqlStore);
    }
    //qDebug() << resStr;
}

qint32 AuBase::querySql(const QString &table,const QString &query)
{
    // спочатку провіримо чи є дана таблиця в локальній базі
    QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME,true); //знайти з’єдання logging
    int index =QRandomGenerator::global()->generate(); // згенерувати індекс або зберегти, якщо він не нульовий (з мережі прийшов)

    if(src.plcList().keys().contains(table) &&  src[table].active()) // це точно локально є такий контролер і він активний
    {
        AuSqlQuery *sqlQry=new AuSqlQuery(DBFILE);
        sqlQry->setId(index);
        connect(sqlQry,&AuSqlQuery::finishedId,this,&AuBase::processQuery);
        sqlQry->exec(query);

    }
    else if(src.plcList().keys().contains(table)==false && dbs.isValid() && dbs.open() && dbs.tables().contains(table)   // є таблиця, нема контролера, це всякі звіти
             ) // це такий збочений спосіб дізнатиця чи є запитана таблиця локально
    {
        AuSqlQuery *sqlQry=new AuSqlQuery(DBFILE);
        sqlQry->setId(index);
        connect(sqlQry,&AuSqlQuery::finishedId,this,&AuBase::processQuery);
        sqlQry->exec(query);
    }
    else // бо якщо вона в мережі тоді якимось магічним чином для неї потрібно вирахувати plcName, куди надсилати запит
    {
        time_t currentTime=QDateTime::currentDateTime().toTime_t();
        if(currentTime-lastTime<CACHE_TIME && lastTable==table) // дані в кеші актуальні
        { // можна зразу виконати запит
            myTcpTx->sendSqlQuery(lastPlcName,table,query,index);
        }
        else
        { // інакше його треба шукати
            if(myTcpTx!=nullptr) // якщо в нього взагалі є варіант шукати
                myTcpTx->findTable(table);
        }
        // зберегти запит в очікуванні результатів пошуку комп'ютера з необхідною таблицею
        remoteQuerys=query;
        remoteIdx=index;

    }
    return index;
}

void AuBase::scaleQuery(const QString &plcName)
{
    // тут би перевірити чи воно не локальне і його треба відправляти в мережу
    bool send=true;
    foreach(auto *p,activePlc)
    {
        if(p->objectName()==plcName)
            send=false;
    }

    if(send)
        myTcpTx->sendScaleQuery(plcName); // переслати це далі

}

/*
void  AuBase::inputScaleResult(const QString& plcName, const QHash<QString, QVector<double> > &result)
{
    emit scaleResult(plcName,result); // перетранслювати сигнал далі
}
*/

/* ця функція зв'яжеться з усіма хто їй відомий по TCP і спитає чи є в них потрібна таблиця
 * відповідь прийде від усіх, в кого вона є.
 * треба дивитися на прапор active
 *
*/

void AuBase::setTablePlcName(const QString& plcName, const QString& tblName, qint16 active)
{
    // qDebug() << plcName << tblName << active;
    // зберегти отримані дані в кеш

    // виконати запит
    if(active!=0 && plcName==tblName ) // це точно потрібний мені вузол для виймання даних з історії
    {
        lastTime=QDateTime::currentDateTime().toTime_t();
        lastTable=tblName;
        lastPlcName=plcName;
        myTcpTx->sendSqlQuery(plcName,tblName,remoteQuerys,remoteIdx);
    }
    else if (! this->plcList().keys().contains(tblName)) // а тут просто треба перевірити чи ця таблиця зпівпадає з іменем контролера чи ні
    { // якщо не співпадає значить цей вузол підходить

        lastTime=QDateTime::currentDateTime().toTime_t();
        lastTable=tblName;
        lastPlcName=plcName;

        myTcpTx->sendSqlQuery(plcName,tblName,remoteQuerys,remoteIdx);
    }

}


void AuBase::loadTrendChartData(TrendChart *trc, QString tagList)
{
    new TrcDataLoader(trc,tagList,*this,this); // запустити і забути
}

void AuBase::getAlert(const QString& msg) // ця фігня потрібна для пересилання повідомлень далі
{
    emit Alert(msg);
}
