#include "ausqlexecuter.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>

#include <QDebug>
#include <QUuid>

AuSqlExecuter::AuSqlExecuter(QString database, QString sqlQuery, QObject *parent) : QObject(parent)
  ,dbName(database)
  ,query(sqlQuery)
{

}


void AuSqlExecuter::exec()
{ // тут відбувається робота з базою даних в блокуючому режимі
    // раді такого кусочка кода городити багатопотоковість
    QString localName=QUuid::createUuid().toString(); // генеруємо унікальне ім'я для QSqlDatabase
    // qDebug() << "AuSqlExecuter started: " << this << localName;
    {
        QSqlDatabase dbs=QSqlDatabase::addDatabase(
            #ifndef WIN32
//                    QStringLiteral("QSQLITE3")
                    QStringLiteral("QSQLITE")
            #else
                    QStringLiteral("QSQLITE")
            #endif
                    ,localName);

        dbs.setDatabaseName(dbName);
        if(dbs.isValid() &&
           dbs.open() )
        {
            QSqlQuery qry(dbs);
            //qDebug() << query;
            if(qry.exec(query)) // а все тому, що оце може виконуватися довго і заблокує event loop
            {
                int fieldCount=qry.record().count();
                while(qry.next())
                {
                    QStringList row;
                    for(int i=0;i<fieldCount;++i)
                        row << qry.value(i).toString();
                    //qDebug() << row;
                    emit dataRow(row);
                }
            }
            else
            {
                emit error(qry.lastError().text());
#ifndef QT_NO_DEBUG
                qDebug() << query << qry.lastError().text();
#endif
            }
            dbs.close();
        }
        else
            emit error(dbs.lastError().text());
    }
    {
        QSqlDatabase::removeDatabase(localName);
    }
    emit finished();
    // qDebug() << "AuSqlExecuter::finished";

}
