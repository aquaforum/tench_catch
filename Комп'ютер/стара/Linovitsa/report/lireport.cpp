#include "lireport.h"
#include <IoNetClient.h>
#include <QtSql>
#include <QCoreApplication>
#include <QSettings>
#include <QDateTime>

#include <QTextStream>
#include <QDebug>

#define DIFUZ (*(src[0]))

#define VIPARKA (*(src[2]))

#define ROU (*(src[5]))

//#define __TEST__

namespace Vn {
    enum Index {
        SGstr=0,
        SVvoda=1,
        SVds=2,
        SVvm=3,
        SVsvp=4,
        SVsfv=5,
        SVkpk=6,
        SVvfd=7,
        SVvtv=8,
        SGutf=9,
        SVvm1=10,
        SVvm2=11,
        SGst=12,
        SGcl=13,
        GRout=14,
        GT1=15,
        GT2=16,
        SVvva=17

    };
}


LiReport::LiReport(QVector<IoNetClient*> &source, QObject *parent) :
    QObject(parent),src(source),updateCount(0),vCount(0)
{

    foreach(IoNetClient *s,src)
    {
        // connect(s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
        connect(s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
    }

    scale.fill(0,20);
    value.fill(0,20);



}


 void LiReport::updateDataRaw()
 {


 }

 /*
SGstr	SVvoda	SVds	SVvm	SVsvp	SVsfv	SVkpk	SVvfd	SVvtv	SGutf
V_10	V_14	V_15	V_03	V_26	V_27	V_15	V_41	V_16
                        V_04
                        V_05

  */

 void LiReport::updateDataScaled()
 {
     QTextStream out(stdout);


     IoNetClient &s=*qobject_cast<IoNetClient*>(sender());

     if(s.objectName()=="difuz")
     {
            scale[Vn::SGstr]=s[0]->scaleFull("V_10");
            scale[Vn::SVvoda]=s[0]->scaleFull("V_14");
            scale[Vn::SVds]=s[0]->scaleFull("V_15");
	    updateCount |= 0x1;
     }
     else if(s.objectName()=="satur")
     {
            scale[Vn::SVvm]=s[0]->scaleFull("V_03");
            scale[Vn::SVvm1]=s[0]->scaleFull("V_04");
            scale[Vn::SVvm2]=s[0]->scaleFull("V_05");
	    updateCount |= 0x2;
     }

     else if(s.objectName()=="viparka")
     {
            scale[Vn::SVsvp]=s[0]->scaleFull("V_26");
            scale[Vn::SVsfv]=s[0]->scaleFull("V_27");
            scale[Vn::SVkpk]=s[0]->scaleFull("V_15");

            // scale[Vn::SVvfd]=s[0]->scaleFull("V_41");
            scale[Vn::SVvtv]=s[0]->scaleFull("V_16");
	    updateCount |= 0x4;
	    vCount++;


     }
     else if(s.objectName()=="rou")
     {
         scale[Vn::GRout]=s[0]->scaleFull("V0018");
         scale[Vn::GT1]=s[0]->scaleFull("V0019");
         scale[Vn::GT2]=s[0]->scaleFull("V0020");
     }

        out << s.objectName() << updateCount <<  "\n";

//     if(updateCount==19)

     if(updateCount==7 && vCount==6) // по всіх пройшлись два рази
    {
         qApp->exit();
    }
     
     if(updateCount==7 && vCount==3) // по всіх пройшлись два рази
     {
	vCount++;

         QString queryTmpl = "SELECT %1 from trend where Dt BETWEEN \'%2\' AND \'%3\'";

         QDateTime stopTime=QDateTime::currentDateTime();
         QDateTime startTime=stopTime.addSecs(-3600*12);


         out << "startTime " << startTime.toString("yyyy/MM/dd hh:mm:ss") << "  stopTime " << stopTime.toString("yyyy/MM/dd hh:mm:ss") << "\n";

         QSqlDatabase dbs=QSqlDatabase::addDatabase("QMYSQL","report");

         // вираховуємо значення з дифузії
         {
            QCoreApplication::setApplicationName("difuz");
            QSqlDatabase dbs=QSqlDatabase::database("report",false);
            QSettings set;

            dbs.setHostName(set.value("/db/hostname","localhost").toString());
            dbs.setDatabaseName(set.value("/db/dbname","centrif").toString());
            dbs.setUserName(set.value("/db/username","scada").toString());
            dbs.setPassword(set.value("/db/passwd","").toString());

            if(!dbs.open()) // спробувати відкрити
                qDebug() << dbs.lastError().databaseText();
            else
            {
                QString fields=
                        "sum(V_10)*0.00000034722222222222, "
                        "sum(V_14)*0.00000034722222222222, "
                        "sum(V_15)*0.00000034722222222222 "
                        ;

                QSqlQuery query(dbs);

                if(query.exec(QString(queryTmpl.arg(fields).arg(startTime.toTime_t()).arg(stopTime.toTime_t()))))
                {

                    query.next();
                    QSqlRecord rec=query.record();
                    value[Vn::SGstr]=rec.value(0).toDouble()*scale[Vn::SGstr];
                    value[Vn::SVvoda]=rec.value(1).toDouble()*scale[Vn::SVvoda];
                    value[Vn::SVds]=rec.value(2).toDouble()*scale[Vn::SVds];
                    out << "Vn::SGstr=" << value[Vn::SGstr] << "\n"
                        << "Vn::SVvoda=" << value[Vn::SVvoda] << "\n"
                        << "Vn::SVds=" << value[Vn::SVds] << "\n" ;
                }
                else
                    qDebug() << query.lastQuery();
                dbs.close();
            }
         }

         // вираховуємо значення з сатурації
         {
            QCoreApplication::setApplicationName("satur");
            QSqlDatabase dbs=QSqlDatabase::database("report",false);
            QSettings set;

            dbs.setHostName(set.value("/db/hostname","localhost").toString());
            dbs.setDatabaseName(set.value("/db/dbname","centrif").toString());
            dbs.setUserName(set.value("/db/username","scada").toString());
            dbs.setPassword(set.value("/db/passwd","").toString());

            if(!dbs.open()) // спробувати відкрити
                qDebug() << dbs.lastError().databaseText();
            else
            {
                QString fields=
                        "sum(V_03)*0.00000034722222222222, "
                        "sum(V_04)*0.00000034722222222222, "
                        "sum(V_05)*0.00000034722222222222 "
                        ;

                QSqlQuery query(dbs);

                if(query.exec(QString(queryTmpl.arg(fields).arg(startTime.toTime_t()).arg(stopTime.toTime_t()))))
                {

                    query.next();
                    QSqlRecord rec=query.record();
                    value[Vn::SVvm]=rec.value(0).toDouble()*scale[Vn::SVvm] +
                            rec.value(1).toDouble()*scale[Vn::SVvm1] +
                            rec.value(2).toDouble()*scale[Vn::SVvm2];
                    out << "Vn::SVvm=" << value[Vn::SVvm] << "\n" ;
                }
                else
                    qDebug() << query.lastQuery();
                dbs.close();
            }
         }

         // вираховуємо значення з дифузії
         {
            QCoreApplication::setApplicationName("viparka");
            QSqlDatabase dbs=QSqlDatabase::database("report",false);
            QSettings set;

            dbs.setHostName(set.value("/db/hostname","localhost").toString());
            dbs.setDatabaseName(set.value("/db/dbname","centrif").toString());
            dbs.setUserName(set.value("/db/username","scada").toString());
            dbs.setPassword(set.value("/db/passwd","").toString());

            if(!dbs.open()) // спробувати відкрити
                qDebug() << dbs.lastError().databaseText();
            else
            {
                QString fields=
                        "sum(V_26)*0.00000034722222222222, "
                        "sum(V_27)*0.00000034722222222222, "
                        "sum(V_15)*0.00000034722222222222 "
                        ;

                QSqlQuery query(dbs);

                if(query.exec(QString(queryTmpl.arg(fields).arg(startTime.toTime_t()).arg(stopTime.toTime_t()))))
                {

                    query.next();
                    QSqlRecord rec=query.record();
                    value[Vn::SVsvp]=rec.value(0).toDouble()*scale[Vn::SVsvp];
                    value[Vn::SVsfv]=rec.value(1).toDouble()*scale[Vn::SVsfv];
                    value[Vn::SVkpk]=rec.value(2).toDouble()*scale[Vn::SVkpk];

                    out << "Vn::SVsvp=" << value[Vn::SVsvp] << "\n"
                        << "Vn::SVsfv=" << value[Vn::SVsfv] << "\n"
                        << "Vn::SVkpk=" << value[Vn::SVkpk] << "\n" ;
                    }
                else
                    qDebug() << query.lastQuery();
                dbs.close();
            }
         }

         // вираховуємо значення з центрифуг
         {
            QCoreApplication::setApplicationName("centrif");
            QSqlDatabase dbs=QSqlDatabase::database("report",false);
            QSettings set;

            dbs.setHostName(set.value("/db/hostname","localhost").toString());
            dbs.setDatabaseName(set.value("/db/dbname","centrif").toString());
            dbs.setUserName(set.value("/db/username","scada").toString());
            dbs.setPassword(set.value("/db/passwd","").toString());

            if(!dbs.open()) // спробувати відкрити
                qDebug() << dbs.lastError().databaseText();
            else
            {

                QSqlQuery query(dbs);

                if(query.exec(QString("SELECT sum(Gzag) FROM report WHERE cft BETWEEN \'%1\' AND \'%2\'")
                              .arg(startTime.toString("yyyy/MM/dd hh:mm:ss"))
                              .arg(stopTime.toString("yyyy/MM/dd hh:mm:ss"))))
                {

                    query.next();
                    QSqlRecord rec=query.record();
                    value[Vn::SGutf]=rec.value(0).toDouble();
                    out << "Vn::SGutf=" << value[Vn::SGutf] << "\n" ;
                }
                else
                    qDebug() << query.lastQuery();
                dbs.close();
            }
         }
         // вираховуємо значення з газової
         {
            QCoreApplication::setApplicationName("gaspich");
            QSqlDatabase dbs=QSqlDatabase::database("report",false);
            QSettings set;

            dbs.setHostName(set.value("/db/hostname","localhost").toString());
            dbs.setDatabaseName(set.value("/db/dbname","gaspich").toString());
            dbs.setUserName(set.value("/db/username","scada").toString());
            dbs.setPassword(set.value("/db/passwd","").toString());

            if(!dbs.open()) // спробувати відкрити
                qDebug() << dbs.lastError().databaseText();
            else
            {

                QSqlQuery query(dbs);

                if(query.exec(
                            QString("SELECT sum(Gcl),sum(prevGst) FROM report WHERE cft BETWEEN \'%1\' AND \'%2\'")
                                                          .arg(startTime.toString("yyyy/MM/dd hh:mm:ss"))
                                                          .arg(stopTime.toString("yyyy/MM/dd hh:mm:ss")))
                            )
                {

                    query.next();
                    QSqlRecord rec=query.record();
                    value[Vn::SGcl]=rec.value(0).toDouble();
                    value[Vn::SGst]=rec.value(1).toDouble();
                    out << "Vn::SGcl=" << value[Vn::SGcl] << "\n"
                        << "Vn::SGst=" << value[Vn::SGst] << "\n" ;
                }
                else
                    qDebug() << query.lastQuery();
                dbs.close();
            }
         }


         // вираховуємо значення з rou
         {
            QCoreApplication::setApplicationName("rou");
            QSqlDatabase dbs=QSqlDatabase::database("report",false);
            QSettings set;

            dbs.setHostName(set.value("/db/hostname","localhost").toString());
            dbs.setDatabaseName(set.value("/db/dbname","rou").toString());
            dbs.setUserName(set.value("/db/username","scada").toString());
            dbs.setPassword(set.value("/db/passwd","").toString());

            if(!dbs.open()) // спробувати відкрити
                qDebug() << dbs.lastError().databaseText();
            else
            {
                QString fields=
                        "sum(V0018)*0.00000034722222222222, "
                        "sum(V0019)*0.00000034722222222222, "
                        "sum(V0020)*0.00000034722222222222 "
                        ;

                QSqlQuery query(dbs);

                if(query.exec(QString(queryTmpl.arg(fields).arg(startTime.toTime_t()).arg(stopTime.toTime_t()))))
                {

                    query.next();
                    QSqlRecord rec=query.record();
                    value[Vn::GRout]=rec.value(0).toDouble()*scale[Vn::GRout];
                    value[Vn::GT1]=rec.value(1).toDouble()*scale[Vn::GT1];
                    value[Vn::GT2]=rec.value(2).toDouble()*scale[Vn::GT2];
                    out << "Vn::GRou=" << value[Vn::GRout] << "\n"
                        << "Vn::GT1=" << value[Vn::GT1] << "\n"
                        << "Vn::GT2=" << value[Vn::GT2] << "\n" ;

                }
                else
                    qDebug() << query.lastQuery();
                dbs.close();
            }
         }

         // писалка звітів
         {
             QCoreApplication::setApplicationName("centrif");
             QSqlDatabase dbs=QSqlDatabase::database("report",false);
             QSettings set;

             dbs.setHostName(set.value("/db/hostname","localhost").toString());
             dbs.setDatabaseName("report");
             dbs.setUserName(set.value("/db/username","scada").toString());
             dbs.setPassword(set.value("/db/passwd","").toString());

             if(!dbs.open()) // спробувати відкрити
                 qDebug() << dbs.lastError().databaseText();
             else
             {

            value[Vn::SVvfd]=VIPARKA[0]->getValueFloat("V_41");
            value[Vn::SVvtv]=VIPARKA[0]->getValueFloat("V_16");
            value[Vn::SVvva]=VIPARKA[0]->getValueFloat("V_52");


        out  << "Vn::SVvfd=!!!" << value[Vn::SVvfd] << "\n"
         << "Vn::SVvtv=" << value[Vn::SVvtv] << "\n" ;


#ifndef __TEST__

            VIPARKA[0]->sendValue("V_41",(double)0.0);
            VIPARKA[0]->sendValue("V_16",(double)0.0);
            VIPARKA[0]->sendValue("V_52",(double)0.0);



                 QSqlQuery query(dbs);

                 if(! query.exec(QString("INSERT INTO report(startTime,stopTime,SGstr,SVvoda,SVds,SVvm,SVsvp,SVsfv, SVkpk, SVvfd, SVvtv, SGutf,SGcl,SGst,GRou,GT1,GT2,SVvva ) "
                    "VALUES (\'%1\',\'%2\',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18)"
                               ).arg(startTime.toString("yyyy/MM/dd hh:mm:ss")) // 1
                               .arg(stopTime.toString("yyyy/MM/dd hh:mm:ss")) // 2
                               .arg(value[Vn::SGstr]) //3
                                 .arg(value[Vn::SVvoda]) //4
                                 .arg(value[Vn::SVds]) //5
                                 .arg(value[Vn::SVvm]) //6
                                 .arg(value[Vn::SVsvp]) //7
                                 .arg(value[Vn::SVsfv]) //8
                                 .arg(value[Vn::SVkpk]) //9
                                 .arg(value[Vn::SVvfd]) //10
                                 .arg(value[Vn::SVvtv]) //11
                                 .arg(value[Vn::SGutf]) //12
                                .arg(value[Vn::SGcl]) //11
                                 .arg(value[Vn::SGst]) //12
                                 .arg(value[Vn::GRout]) //13
                                 .arg(value[Vn::GT1]) //14
                                 .arg(value[Vn::GT2]) //15
                                 .arg(value[Vn::SVvva]) //16

                                 ))
                     qDebug() << query.lastQuery();

#else
        qDebug() <<QString("INSERT INTO report(startTime,stopTime,SGstr,SVvoda,SVds,SVvm,SVsvp,SVsfv, SVkpk, SVvfd, SVvtv, SGutf,SGcl,SGst,GRou,GT1,GT2,SVvva ) "
           "VALUES (\'%1\',\'%2\',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18)"
                      ).arg(startTime.toString("yyyy/MM/dd hh:mm:ss")) // 1
                      .arg(stopTime.toString("yyyy/MM/dd hh:mm:ss")) // 2
                      .arg(value[Vn::SGstr]) //3
                        .arg(value[Vn::SVvoda]) //4
                        .arg(value[Vn::SVds]) //5
                        .arg(value[Vn::SVvm]) //6
                        .arg(value[Vn::SVsvp]) //7
                        .arg(value[Vn::SVsfv]) //8
                        .arg(value[Vn::SVkpk]) //9
                        .arg(value[Vn::SVvfd]) //10
                        .arg(value[Vn::SVvtv]) //11
                        .arg(value[Vn::SGutf]) //12
                       .arg(value[Vn::SGcl]) //11
                        .arg(value[Vn::SGst]) //12
                        .arg(value[Vn::GRout]) //13
                        .arg(value[Vn::GT1]) //14
                        .arg(value[Vn::GT2]) //15
                        .arg(value[Vn::SVvva]) //16
                ;

#endif
                 dbs.close();
             }
         }


     }


 }

