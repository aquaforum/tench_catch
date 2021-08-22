#include "fabreportwriter.h"
#include "aulib.h"

#include <QApplication>
#include <QSettings>
#include <QDateTime>

#include <QTextStream>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QDir>
#include <QFile>
#include <QRegExp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QDebug>

#define DIFUZ   QStringLiteral("plcdifuz")
#define SATUR   QStringLiteral("plcsatur")
#define VIPARKA QStringLiteral("plcvipar")
#define ROU     QStringLiteral("plcrou")
#define GPI4    QStringLiteral("plcgpi4")

// тут перераховано всі поля для запису в звіт

#define REPTABLE QStringLiteral("fabreport")
FabReportWriter::FabReportWriter(AuBase &base, QObject *parent): QObject(parent)
 , src(base)
 , n(0),state(6)
{
    setObjectName("FabReportWriter");

    QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME,true);

    if(dbs.isOpen())
    {
        if(! dbs.tables().contains(REPTABLE)) // перевірти наявність таблиці
        {
            QSqlQuery qry(dbs);
            if(!qry.exec(QStringLiteral("CREATE TABLE %1(%2)").arg(REPTABLE)
                     .arg(QStringLiteral("tStart integer unsigned not null, "
                                         "tStop integer unsigned not null, "
                                         "SGstr REAL DEFAULT '0.0',"
                                         "SVvoda REAL DEFAULT '0.0',"
                                         "SVds REAL DEFAULT '0.0',"
                                         "S7Vds REAL DEFAULT '0.0',"
                                         "SVvm REAL DEFAULT '0.0',"
                                         "SVsvp REAL DEFAULT '0.0',"
                                         "SVsfv REAL DEFAULT '0.0',"
                                         "GRou REAL DEFAULT '0.0',"
                                         "GT1 REAL DEFAULT '0.0',"
                                         "GT2 REAL DEFAULT '0.0',"
                                         "SGcl REAL DEFAULT '0.0',"
                                         "SGst REAL DEFAULT '0.0',"
                                         "KFn REAL DEFAULT '0.0',"
                                         "SVvfd REAL DEFAULT '0.0',"
                                         "SVvtv REAL DEFAULT '0.0',"
                                         "SVvva REAL DEFAULT '0.0',"
                                         "W11 REAL DEFAULT '0.0', "
                                         "W00 REAL DEFAULT '0.0', "
                                         "W01 REAL DEFAULT '0.0', "
                                         "W02 REAL DEFAULT '0.0', "
                                         "W03 REAL DEFAULT '0.0', "
                                         "W04 REAL DEFAULT '0.0', "
                                         "W05 REAL DEFAULT '0.0', "
                                         "W06 REAL DEFAULT '0.0', "
                                         "W07 REAL DEFAULT '0.0', "
                                         "W08 REAL DEFAULT '0.0', "
                                         "W09 REAL DEFAULT '0.0', "
                                         "W10 REAL DEFAULT '0.0',"
                                         "H11 REAL DEFAULT '0.0', "
                                         "H00 REAL DEFAULT '0.0', "
                                         "H01 REAL DEFAULT '0.0', "
                                         "H02 REAL DEFAULT '0.0', "
                                         "H03 REAL DEFAULT '0.0', "
                                         "H04 REAL DEFAULT '0.0', "
                                         "H05 REAL DEFAULT '0.0', "
                                         "H06 REAL DEFAULT '0.0', "
                                         "H07 REAL DEFAULT '0.0', "
                                         "H08 REAL DEFAULT '0.0', "
                                         "H09 REAL DEFAULT '0.0', "
                                         "H10 REAL DEFAULT '0.0'"
                     ))))
            {
                qDebug() << qry.lastQuery() << qry.lastError();
            }

            if(!qry.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS FABREPIX ON %1 (tStart,tStop)").arg(REPTABLE)))
            {
                qDebug() << qry.lastQuery() << qry.lastError();
            }
            // dbs.close();
        }

    }

    connect(&src,&AuBase::sqlData,this,&FabReportWriter::sqlResult);

    for(int i=0;i<16;++i) // ініціалізувати нулями
        reportData.value[i]=0.0;

    stop=QDateTime::currentDateTime().toTime_t();
    start=stop-12*3600; // 12 годин

    qDebug() << "start " << start << " stop " << stop ;

    tagsList << QStringLiteral("plcdifuz,SUM(V_10)*0.001388888888889,SUM(V_14)*0.001388888888889,SUM(V_15)*0.001388888888889")
             << QStringLiteral("plcsatur,SUM(V_03)*0.001388888888889,SUM(V_04)*0.001388888888889,SUM(V_05)*0.001388888888889")
             << QStringLiteral("plcvipar,SUM(V_26)*0.001388888888889,SUM(V_27)*0.001388888888889")
             << QStringLiteral("plcrou,SUM(V0018)*0.001388888888889,SUM(V0019)*0.001388888888889,SUM(V0020)*0.001388888888889")
             << QStringLiteral("gpreport,SUM(Gcl),SUM(Gst)")
                ;
             // << QStringLiteral("plcdifuz,SUM(V_10)*0.001388888888889,SUM(V_14)*0.001388888888889,SUM(V_15)*0.001388888888889")  ;


    // надіслати запити пачкою і подивитися що з цього вийде


    {   // повторити запит до дифузії з іншим часовим інтервалом
        QDateTime cdt=QDateTime::currentDateTime();
        if(cdt.time().hour()>=8 && cdt.time().hour()<20) // якщо зараз денна зміна
        {
            cdt.setTime(QTime(7,0,0,0));
        }
        else
        {
            cdt.setDate(QDate(cdt.date().year(),cdt.date().month(),cdt.date().day()).addDays(cdt.time().hour()>19?0:-1));
            cdt.setTime(QTime(19,0,0,0));
        }
        dstop=cdt.toTime_t();
        dstart=stop-12*3600;

        QString tag=tagsList[0];
        int i=tag.indexOf(',');
        QString  table =tagsList[0].left(i);
        tag.remove(0,i+1);
    }

    QTimer::singleShot(1000,this,&FabReportWriter::slotSendQry);
    // захисний таймер на випадок якщо звідкись дані не прийдуть
    QTimer::singleShot(30000,this,&FabReportWriter::writeReport);


    // гворити раз в хвилину про своє істування
    QTimer *clock = new QTimer(this);
    clock->setInterval(60000);
    connect(clock,&QTimer::timeout,this,&FabReportWriter::slotClock);
    clock->start();

}

FabReportWriter::~FabReportWriter()
{
    // QSqlDatabase::removeDatabase(AUDBNAME);
    qDebug() << QDateTime::currentDateTime() << "FabReportWriter deleted!!!!";
}

/*
void FabReportWriter::exit()
{
    deleteLater();
}
*/

void FabReportWriter::slotSendQry()
{
    if(n<tagsList.size())
    {
        QString tag=tagsList[n++];
        int i=tag.indexOf(',');
        QString  table =tag.left(i);
        tag.remove(0,i+1);
        qryIx << src.querySql(table,QStringLiteral("SELECT %1 FROM %2 where Dt BETWEEN %3 AND %4")
                              .arg(tag)
                              .arg(table)
                              .arg(start)
                              .arg(stop));
        qDebug() << QStringLiteral("SELECT %1 FROM %2 where Dt BETWEEN %3 AND %4")
                                  .arg(tag)
                                  .arg(table)
                                  .arg(start)
                                  .arg(stop);

        QTimer::singleShot(1000,this,&FabReportWriter::slotSendQry);

    }
    else
    {
        n=0;
        QString tag=tagsList[n++];
        int i=tag.indexOf(',');
        QString  table =tag.left(i);
        tag.remove(0,i+1);
        qryIx << src.querySql(table,QStringLiteral("SELECT %1 FROM %2 where Dt BETWEEN %3 AND %4")
                              .arg(tag)
                              .arg(table)
                              .arg(dstart)
                              .arg(dstop));

        qDebug() << QStringLiteral("SELECT %1 FROM %2 where Dt BETWEEN %3 AND %4")
                                  .arg(tag)
                                  .arg(table)
                                  .arg(dstart)
                                  .arg(dstop);

        QTimer::singleShot(2000,this,&FabReportWriter::writeReport);

    }


}


void FabReportWriter::sqlResult(qint32 id,const QList<QStringList>& result)
{
    QStringList row = result[0];

        qDebug() << "state " << state-- << " id " << id << " switch " << qryIx.indexOf(id);
        qDebug() << result;

    SAFE(
    switch (qryIx.indexOf(id)) {
    case 0: // plcdifuz
        reportData.V.SGstr= row[0].toDouble()*src[DIFUZ][QStringLiteral("V_10")].k()+src[DIFUZ][QStringLiteral("V_10")].b(); // стружка по стрічкових вагах
        reportData.V.SVvoda=row[1].toDouble()*src[DIFUZ][QStringLiteral("V_14")].k()+src[DIFUZ][QStringLiteral("V_14")].b(); // витрата води
        reportData.V.SVds=  row[2].toDouble()*src[DIFUZ][QStringLiteral("V_15")].k()+src[DIFUZ][QStringLiteral("V_15")].b();// видтрада дифузійного соку

        break;
    case 1: // plcsatur
        reportData.V.SVvm = row[0].toDouble()*src[SATUR][QStringLiteral("V_03")].k()+src[SATUR][QStringLiteral("V_03")].b()
                           +row[1].toDouble()*src[SATUR][QStringLiteral("V_04")].k()+src[SATUR][QStringLiteral("V_04")].b()
                           +row[2].toDouble()*src[SATUR][QStringLiteral("V_05")].k()+src[SATUR][QStringLiteral("V_05")].b();
        break;
    case 2: // plcvipar
        reportData.V.SVsvp= row[0].toDouble()*src[VIPARKA][QStringLiteral("V_26")].k()+src[VIPARKA][QStringLiteral("V_26")].b(); // витрата соку на випарку
        reportData.V.SVsfv= row[1].toDouble()*src[VIPARKA][QStringLiteral("V_27")].k()+src[VIPARKA][QStringLiteral("V_27")].b(); // витрата сиропу з випарки
        break;
    case 3: // plcrou
        reportData.V.GRou = row[0].toDouble()*src[ROU][QStringLiteral("V0018")].k()+src[ROU][QStringLiteral("V0018")].b(); // витрапари пари на роу
        reportData.V.GT1  = row[1].toDouble()*src[ROU][QStringLiteral("V0019")].k()+src[ROU][QStringLiteral("V0019")].b(); // витрата пари на Т1
        reportData.V.GT2  = row[2].toDouble()*src[ROU][QStringLiteral("V0020")].k()+src[ROU][QStringLiteral("V0020")].b(); // витрата пари на Т2
        break;
    case 4: // gpreport
        reportData.V.SGcl = row[0].toDouble(); // витрата вугілля на газову
        reportData.V.SGst = row[1].toDouble(); // витрата каменю на газову
        break;
    case 5: // plcdifuz але з іншим інтервалом часу
        reportData.V.S7Vds= row[2].toDouble()*src[DIFUZ][QStringLiteral("V_15")].k()+src[DIFUZ][QStringLiteral("V_15")].b();// видтрада дифузійного соку.
        break;
    default: // ? відповідей трохи більше ніж запитів таке буде можливо коли в мереж буде наприклад два plcdifuz
        break;

    }
                )
}


void FabReportWriter::writeReport()
{
    // випарка
    SAFE(
    reportData.V.SVvfd  = src[VIPARKA][QStringLiteral("V_41")]; // витрата води на дискові фільтри
    reportData.V.SVvtv  = src[VIPARKA][QStringLiteral("V_16")]; // витрата води на випарку
    reportData.V.SVvva  = src[VIPARKA][QStringLiteral("V_52")]; // витрата води на вакуум-апараи
        )
    SAFE(
    massRep.V.W11    = (src[DIFUZ][QStringLiteral("V0024")].valueReal()+src[DIFUZ][QStringLiteral("V0048")].valueReal())/100.0; // останній час попередньої зміни
    massRep.V.W00    = (src[DIFUZ][QStringLiteral("V0001")].valueReal()+src[DIFUZ][QStringLiteral("V0025")].valueReal())/100.0;
    massRep.V.W01    = (src[DIFUZ][QStringLiteral("V0002")].valueReal()+src[DIFUZ][QStringLiteral("V0026")].valueReal())/100.0;
    massRep.V.W02    = (src[DIFUZ][QStringLiteral("V0003")].valueReal()+src[DIFUZ][QStringLiteral("V0027")].valueReal())/100.0;
    massRep.V.W03    = (src[DIFUZ][QStringLiteral("V0004")].valueReal()+src[DIFUZ][QStringLiteral("V0028")].valueReal())/100.0;
    massRep.V.W04    = (src[DIFUZ][QStringLiteral("V0005")].valueReal()+src[DIFUZ][QStringLiteral("V0029")].valueReal())/100.0;
    massRep.V.W05    = (src[DIFUZ][QStringLiteral("V0006")].valueReal()+src[DIFUZ][QStringLiteral("V0030")].valueReal())/100.0;
    massRep.V.W06    = (src[DIFUZ][QStringLiteral("V0007")].valueReal()+src[DIFUZ][QStringLiteral("V0031")].valueReal())/100.0;
    massRep.V.W07    = (src[DIFUZ][QStringLiteral("V0008")].valueReal()+src[DIFUZ][QStringLiteral("V0032")].valueReal())/100.0;
    massRep.V.W08    = (src[DIFUZ][QStringLiteral("V0009")].valueReal()+src[DIFUZ][QStringLiteral("V0033")].valueReal())/100.0;
    massRep.V.W09    = (src[DIFUZ][QStringLiteral("V0010")].valueReal()+src[DIFUZ][QStringLiteral("V0034")].valueReal())/100.0;
    massRep.V.W10    = (src[DIFUZ][QStringLiteral("V0011")].valueReal()+src[DIFUZ][QStringLiteral("V0035")].valueReal())/100.0;
    )
// #warning Тут треба вставити код завантаження витрати газу з файлу
    gasRep.V.H11=-1;
    gasRep.V.H00=-1;
    gasRep.V.H01=-1;
    gasRep.V.H02=-1;
    gasRep.V.H03=-1;
    gasRep.V.H04=-1;
    gasRep.V.H05=-1;
    gasRep.V.H06=-1;
    gasRep.V.H07=-1;
    gasRep.V.H08=-1;
    gasRep.V.H09=-1;
    gasRep.V.H10=-1;
    // після того як дані з файлу буде завантажено файл потібно видалити

    reportData.V.KFn=0;

    QStringList val; // тут готуються дані для вствки в базу
    val << QString::number(QDateTime::currentDateTime().toTime_t()-12*3600)
        << QString::number(QDateTime::currentDateTime().toTime_t())
        << QString::number(reportData.V.SGstr,'f',1)
        << QString::number(reportData.V.SVvoda,'f',1)
        << QString::number(reportData.V.SVds,'f',1)
        << QString::number(reportData.V.S7Vds,'f',1)
        << QString::number(reportData.V.SVvm,'f',1)
        << QString::number(reportData.V.SVsvp,'f',1)
        << QString::number(reportData.V.SVsfv,'f',1)
        << QString::number(reportData.V.GRou,'f',1)
        << QString::number(reportData.V.GT1,'f',1)
        << QString::number(reportData.V.GT2,'f',1)
        << QString::number(reportData.V.SGcl,'f',1)
        << QString::number(reportData.V.SGst,'f',1)
        << QString::number(reportData.V.KFn,'f',1)
        << QString::number(reportData.V.SVvfd,'f',1)
        << QString::number(reportData.V.SVvtv,'f',1)
        << QString::number(reportData.V.SVvva,'f',1)
           ; // це все потенційно можна замінити на один цикл for, але з метою розширення так його краще видно
    // додати газ
    for(int i=0;i<12;++i)
    {
        val << QString::number(gasRep.value[i],'f',1);
    }
    // додати переробку
    for(int i=0;i<12;++i)
    {
        val << QString::number(massRep.value[i],'f',1);
    }

    // тут треба побудувати дуже довгий запит на вставку даних в таблицю fabreport
    QString insQry=QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)")
            .arg(REPTABLE)
            .arg(QStringLiteral("tStart, tStop, "
                                "SGstr, "
                                "SVvoda, "
                                "SVds, "
                                "S7Vds, "
                                "SVvm, "
                                "SVsvp, "
                                "SVsfv, "
                                "GRou, "
                                "GT1, "
                                "GT2, "
                                "SGcl, "
                                "SGst, "
                                "KFn, "
                                "SVvfd, "
                                "SVvtv, "
                                "SVvva, "
                                "H11, H00, H01, H02, H03, H04, H05, H06, H07, H08, H09, H10, "
                                "W11, W00, W01, W02, W03, W04, W05, W06, W07, W08, W09, W10"))
            .arg(val.join(',')); // список значень
    // qDebug() << insQry;

    // виконуємо запит напраму, бо якщо цей об'єкт запускається то потрібна таблиця тут точно є, він її в конструкторі створить.
    QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME,true);

    if(dbs.isValid() && dbs.isOpen()) // перевірити і відкрити
    {
        QSqlQuery qry(dbs);
        if(!qry.exec(insQry))
        {
            qDebug() << qry.lastError();
        }
        // dbs.close();
        src[VIPARKA][QStringLiteral("V_41")]=0.0; // витрата води на дискові фільтри
        src[VIPARKA][QStringLiteral("V_16")]=0.0; // витрата води на випарку
        src[VIPARKA][QStringLiteral("V_52")]=0.0; // витрата води на вакуум-апараи

    }
    else
    {
        qDebug() << dbs.lastError();
    }

    qDebug() << "FabReportWriter finished" << QDateTime::currentDateTime();
    emit finished();
}

void FabReportWriter::slotClock()
{
    qDebug() << this << "FabReportWriter slotClock: " << QDateTime::currentDateTime();

}
