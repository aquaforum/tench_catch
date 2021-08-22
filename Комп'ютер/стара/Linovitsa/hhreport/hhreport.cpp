#include "hhreport.h"
#include <IoNetClient.h>
#include <QtSql>
#include <QCoreApplication>
#include <QSettings>
#include <QDateTime>

#include <QTextStream>
#include <QDebug>

#include <QHttp>
#include <QFile>
#include <QRegExp>

#define DIFUZ (*(src[0]))

HhReport::HhReport(QVector<IoNetClient*> &source,QObject *parent) :
    QObject(parent),src(source),gasOk(false),updateCount(0)
{

    connect(src[0],SIGNAL(updateData()),this,SLOT(slotUpdateaData()));

    file=new QFile("/tmp/gasreport.html");
    file->open(QIODevice::WriteOnly);

    mass.fill(0.0,12);
    // gass.fill(0.0,12);

    QHttp *client = new QHttp(this);
    client->setHost("www.izodromgas.com.ua");
    connect(client,SIGNAL(done(bool)),this,SLOT(slotGasDataReady(bool)));

    //client->setUser("lin","lin1");
    //client->get("/nodes/211/reports/html/cv_arc?thread=1&start_time=2016-9-10%200:0&end_time=2016-9-11%200:0",file);

    QDateTime cDt=QDateTime::currentDateTime();

    QString startDate,stopDate;
    if(cDt.time().hour()<8)
    {
        startDate=cDt.addDays(-1).toString("dd/MM/yyyy")+"%2019:00";
        stopDate=cDt.toString("dd/MM/yyyy")+"%2007:00";
    }
    else
    {
        startDate=cDt.toString("dd/MM/yyyy")+"%2007:00";
        stopDate=cDt.toString("dd/MM/yyyy")+"%2019:00";
    }


//  "/home/print?TreeId=1&typenode=node&NodeId=2203&typearc=day&datefrom=17/09/2018%2000:00&dateto=26/09/2018%2000:00&thread=1"
//    client->get(QString("/nodes/211/reports/html/av_arc?thread=1&start_time=%1&end_time=%2")
//    client->get(QString("/home/print?TreeId=1&typenode=node&NodeId=2203&typearc=hour&datefrom=%1&dateto=%2&thread=1&f=true")
//                .arg(startDate).arg(stopDate),file);

    QString url=QString("/home/print?TreeId=1&typenode=node&NodeId=2203&typearc=hour&datefrom=%1&dateto=%2&thread=1&f=true").arg(startDate).arg(stopDate);
    qDebug() << url;

    client->get(url,file);

    //qDebug() << QString("/nodes/211/reports/html/av_arc?thread=1&start_time=%1&end_time=%2")/home/print?TreeId=1&typenode=node&NodeId=2203&typearc=day&datefrom=01/10/2018%2008:00&dateto=01/10/2018%2020:00&thread=1&f=true
    //            .arg(startDate).arg(stopDate);
    //qDebug() << client->
}


void HhReport::slotGasDataReady(bool v)
{
    QHttp *client=qobject_cast<QHttp*>(sender());

    file->close();

    //qint32 tBodyCount=0;
    if(v){
        qDebug() << client->errorString();
        gasOk=true;
        for(int i=0;i<12;++i)
            gass << -1.0;
    }

    else
    {
        // <td>2016-09-18 08</td><td>13.03</td><td>0.315</td><td>24.462</td><td>3993.390</td><td>0.000</td><td>0.824</td><td>5.110</td><td>2.740</td>
            QString line="";
        file->open(QIODevice::ReadOnly);
        while(!file->atEnd() && line!="</thead>") // знайти першу таблицю
        {
             line=QString::fromUtf8( file->readLine().trimmed()) ;
//            qDebug() << line;

        }
        while(!file->atEnd() ) // знайти першу таблицю
        {
             line=QString::fromUtf8( file->readLine().trimmed()) ;
            if(line=="<tr class=\"\">")
            {
            for(int i=0;i<14;++i)
                line=QString::fromUtf8( file->readLine().trimmed()) ;
            line= line.split('&')[0]+line.split(';')[1];


            gass << line.toDouble();

            }



        }

        gasOk=true;
        qDebug() << "gasOk";
        qDebug() << gass;

        if(gass.size()<12)
            for(int i=gass.size();i<12;++i) gass << 0.0;
        file->close();

    }

    //qApp->quit();

}


void HhReport::slotUpdateaData()
{

    if(++updateCount>2 && gasOk)
    {

        mass[0]=(DIFUZ[0]->getValueFloat("V0024")+DIFUZ[0]->getValueFloat("V0048"))/100.0;
        for(int i=1;i<12;++i)
        {
            mass[i]=(DIFUZ[0]->getValueFloat(QString("V00%1").arg(i,2,10,QChar('0')))
                                               +DIFUZ[0]->getValueFloat(QString("V00%1").arg(i+24,2,10,QChar('0'))))/100.0;
        }


        qDebug() << gass << gass.size();
        qDebug() << mass << mass.size();
        gasOk=false;

        QCoreApplication::setApplicationName("centrif");

        QSqlDatabase dbs=QSqlDatabase::addDatabase("QMYSQL","report");
        QSettings set;

        dbs.setHostName(set.value("/db/hostname","centrif").toString());
        dbs.setDatabaseName("report");
        dbs.setUserName(set.value("/db/username","scada").toString());
        dbs.setPassword(set.value("/db/passwd","").toString());

        if(!dbs.open()) // спробувати відкрити
            qDebug() << dbs.lastError().databaseText();
        else
        {
            QString data= "'"+QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+"'";
            for(int i=0;i<12;++i)
            {
                data+=QString(", \'%1\'").arg(mass[i],0,'f',2);
            }
            for(int i=0;i<12;++i)
            {
                data+=QString(", \'%1\'").arg(gass[i],0,'f',2);
            }

            // тут буде писалка таблиці
            QSqlQuery query(dbs);

            if(! query.exec(QString(
            "INSERT INTO  hhreport(startTime, H01, H02, H03, H04, H05, H06, H07, H08, H09, H10, H11, H12,"
            " G01, G02, G03, G04, G05, G06, G07, G08, G09, G10, G11, G12) VALUES(%1)"
            ).arg(data)))
                qDebug() << query.lastQuery();

            qDebug()
                    << QString(
                           "INSERT INTO  hhreport(startTime, H01, H02, H03, H04, H05, H06, H07, H08, H09, H10, H11, H12,"
                           " G01, G02, G03, G04, G05, G06, G07, G08, G09, G10, G11, G12) VALUES(%1)"
                           ).arg(data);
            dbs.close();


        }
        qApp->quit();

    }

}
