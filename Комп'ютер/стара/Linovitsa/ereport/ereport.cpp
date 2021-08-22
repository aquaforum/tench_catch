#include "ereport.h"
#include <QDateTime>
#include <QTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>
#include <QVariant>


EReport::EReport(QObject *parent) :
    QObject(parent)
{
    {
        // тут треба вставити звіт по електроенергії
        QSqlDatabase eDbs=QSqlDatabase::addDatabase("QMYSQL","fqdrv");
        eDbs.setHostName("difuz");
        eDbs.setDatabaseName("fqdrvmon");
        eDbs.setUserName("scada");
        eDbs.setPassword("");

        if(eDbs.open()) // спробувати відкрити
        {
            QString eQuery="SELECT count(Dt),"
                           "AVG(V0021*V0041),"
                           "AVG(V0022*V0042),"
                           "AVG(V0023*V0043),"
                           "AVG(V0024*V0044),"
                           "AVG(V0025*V0045),"
                           "AVG(V0026*V0046),"
                           "AVG(V0027*V0047),"
                           "AVG(V0028*V0048),"
                           "AVG(V0029*V0049),"
                           "AVG(V0030*V0050),"
                           "AVG(V0031*V0051),"
                           "AVG(V0032*V0052),"
                           "AVG(V0033*V0053),"
                           "AVG(V0034*V0054),"
                           "AVG(V0035*V0055) "
                    "FROM trend WHERE Dt BETWEEN %1 AND %2";
            QDateTime start=QDateTime::currentDateTime();

            QSqlQuery query(eDbs);
            if(query.exec(eQuery.arg(start.addSecs(-43200).toTime_t()).arg(start.toTime_t())))
            {
                query.next();
                for(int i=0;i<query.record().count();++i)
                {
                    qDebug() << query.record().value(i).toString();
                }
                query.finish();

            }
            else
                qDebug() << query.lastError().text();
            eDbs.close();

        }
        else
            qDebug() << eDbs.lastError().text();


    }
    QSqlDatabase::removeDatabase("fqdrv");
}
