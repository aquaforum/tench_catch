#include "aulibdefs.h"
#include "aulogging.h"

#include <QApplication>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>

#include <QDir>
#include <QTimer>
#include <QDateTime>
#include <QTime>

#include <QDebug>
#include <QRegExp>

#include "aubase.h"
#include "auplc.h"
#include "autag.h"
#include "aucollectdata.h"

/* Цей клас буде записувати історію з активних  контролерів.
 * База даних буде sqlinte, назва таблиці - по назві контролера
 * Файл бази даних створюється в QDir::homeDir().
 * Назві файлу - applicationName
 *
 * Дуже сильно хочеться синхронізувати часові мітки (Dt) із годинником
 *
 *
*/

AuLogging::AuLogging(AuBase &base, QObject *parent) : QObject(parent)
  ,src(base)
  ,myStoreInterval(5000)
{
    QSqlDatabase dbs;

    QStringList listPlc;

    activeList.clear();
    foreach (quint16 iX,  src.plcList()) {
        if(src[iX].active())
        {
            activeList << iX;
            listPlc << src[iX].name();
            // ця штука збирає дані і віддає сюди запити на збереження даних.
            AuCollectData *cd=new AuCollectData(src[iX],src[iX].interval(),this);
            connect(cd,&AuCollectData::pushData,this,&AuLogging::slotCollectData);
        }
    }

    if(activeList.size()>0) // перевірити чи є контролери для запису історії
    {
        dbs=QSqlDatabase::addDatabase(
            #ifndef WIN32
//                    "QSQLITE3"
                    "QSQLITE"
            #else
                    "QSQLITE"
            #endif
                    ,AUDBNAME);
        dbs.setDatabaseName(DBFILE);
        dbs.open();
        // qDebug() << dbs.tables();

        foreach(QString plcName,listPlc) // перебрати існуючі таблиці
        {
            // КОСТИЛЬ, метод contains не може працювати з . в ключах
            // тай мабуть таблицю  в SQLITE з . в імені не строврити,
            // тому ip ідреси не можна використовувати, тільки імена

            bool writeField=false; // а раптом нічого в історію писати не треба

            if(dbs.tables().contains(plcName)) // перевірити наявність таблиці
            { // перевірити структуру таблиці
                // оскільки sqlite3 підтримує обмежений синткасис команди ALTER TABLE
                // то механізм зміни структури таблиці дуже складний,
                // стару таблицю треба переіменувати,
                // потім створити нову таблицю із необхідними полями
                // а потім перести дані із старої таблиці в нову та вилучити страру.

                QStringList fields; // те що є в базі даних, після очистки залишиться те, що видалити з бази
                QStringList tags; // те що додати в базу

                { // отримати список полів із таблиці
                    QSqlDriver *drv=dbs.driver();
                    QSqlRecord record=drv->record(plcName);
                    for(int i=0;i<record.count();++i)
                        fields<< record.field(i).name();
                }
                // а тепер будемо видаляти із обох списків
                fields.removeAt(fields.indexOf(QStringLiteral("Dt"))); // перши ділом прибрати звідти Dt щоб його не видалило бо це все-таки індекс

                QString createQuery=QStringLiteral("CREATE TABLE %1 (Dt INTEGER PRIMARY KEY ").arg(plcName);
                bool tblTrans=false; // прапор, який позначає що є нове поле
                foreach(QString tagName, src[plcName].tagsList())
                {
                    if(src[plcName][tagName].logging()) // якщо пишимо історію
                    {
                        writeField=true;
                        QString tType="INTEGER";
                        if(src[plcName].dbReal() && src[plcName][tagName].dataType()==tagType::REAL)
                            tType="REAL";
                        createQuery.append(QStringLiteral(",\n %1 %2 DEFAULT \'0\'").arg(tagName).arg(tType));
                        if(fields.contains(tagName)) // якщо поле існує в базі даних
                            tags << tagName; // відмітити його для переносу
                        else
                            tblTrans=true;
                    }
                }
                createQuery.append(") WITHOUT ROWID");
                // визначити чи є поля для видалення
                foreach(QString delTag,tags)
                {
                    fields.removeAt(fields.indexOf(delTag));
                }


                if(tblTrans|| fields.size()>0) // якщо є що видаляти або добавляти
                {
                    QString alterQuery=QStringLiteral("ALTER TABLE %1  RENAME TO %1_old").arg(plcName);

                    tags.insert(0,QStringLiteral("Dt"));
                    QString fList=tags.join(QStringLiteral(","));
                    QString transferQuery=QStringLiteral("INSERT INTO %1 (%2) SELECT %2 FROM %1_old").arg(plcName).arg(fList);

                    QSqlQuery qry(dbs);
                    if(writeField) // якщо є що писати в історію
                    {
                        if(!qry.exec(alterQuery))
                            qDebug() << qry.lastError();

                        if(!qry.exec(createQuery))
                            qDebug() << qry.lastError();

                        if(!qry.exec(transferQuery))
                            qDebug() << qry.lastError();

                        if(!qry.exec(QStringLiteral("DROP TABLE %1_old").arg(plcName)))
                            qDebug() << qry.lastError();


                    }
                    else // інакше просто видалити таблицю
                        qry.exec(QStringLiteral("DROP TABLE %1").arg(plcName));

                    //qDebug()  << alterQuery;
                    //qDebug()  << createQuery;
                    //qDebug() << transferQuery;
                    //qDebug() << QStringLiteral("DROP TABLE %1_old").arg(plcName);

                }

                // дописати нуль в кінець щоб не було дірки
                QSqlQuery qry(dbs);
                if(qry.exec(QStringLiteral("SELECT max(DT) from %1").arg(plcName)))
                {
                    qry.next();
                    int maxDate=qry.value(0).toInt();
                    qry.finish(); // закінчити поточний запит
                    qry.exec(QStringLiteral("INSERT INTO %1(Dt) VALUES(%2)").arg(plcName).arg(++maxDate));

                }


            }
            else
            { // якщо відсутня - створити
                QString createQuery=QStringLiteral("CREATE TABLE %1 (Dt INTEGER PRIMARY KEY ").arg(plcName);
                foreach(QString tagName, src[plcName].tagsList())
                {
                    if(src[plcName][tagName].logging()) // якщо пишимо історію
                    {
                        writeField=true;
                        QString tType="INTEGER";
                        if(src[plcName].dbReal() && src[plcName][tagName].dataType()==tagType::REAL)
                            tType="REAL";
                        createQuery.append(QStringLiteral(",\n %1 %2 DEFAULT \'0\'").arg(tagName).arg(tType));
                        //createQuery.append(QStringLiteral(",\n %1 INTEGER DEFAULT \'0\'").arg(tagName));
                    }
                }
                createQuery.append(") WITHOUT ROWID");
                if(writeField) // якщо є що писати то таки створити таблицю
                {
                    QSqlQuery qry(dbs);
                    if(!qry.exec(createQuery))
                    {
                        qDebug() << qry.lastError().text(); // це треба записати кудись по-серйьозному а не просто в консоль
                        qDebug() << qry.lastQuery();
                    }
                }

                // qDebug() << createQuery;

            }
        }

        // запустити таймер, який раз в хвилину скидуватиме дані в базу
        QTimer *tmr=new QTimer(this);
        tmr->setInterval(60000);
        connect(tmr,&QTimer::timeout,this,&AuLogging::slotStoreQuery);
        tmr->start();

    }

}

AuLogging::~AuLogging()
{
    {
        QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME);
        if(dbs.isValid())
        {
            if(! dbs.isOpen())
                dbs.open();

            if(!queryPool.isEmpty()) // скинути в базу те що ще не скинуто
            {
                dbs.transaction();
                QSqlQuery qry(dbs);
                while(!queryPool.isEmpty())
                {
                    qry.exec(queryPool.dequeue());
                }
                dbs.commit();
            }

            dbs.close();
        }
    }
    QSqlDatabase::removeDatabase(AUDBNAME);
}



void AuLogging::slotStoreQuery()
{
    // qDebug() << queryPool.size();
    QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME);
    if(dbs.isValid())
    {
        if(! dbs.isOpen())
            dbs.open();
        if(!queryPool.isEmpty()) // скинути в базу те що ще не скинуто
        {
            dbs.transaction();
            QSqlQuery qry(dbs);
            while(!queryPool.isEmpty())
            {
                qry.exec(queryPool.dequeue());
            }
            dbs.commit();
        }
        dbs.close();
    }
    else
        qDebug() << dbs.lastError().text();
}

void AuLogging::slotCollectData(QString qry)  //event
{
    queryPool.enqueue(qry);
}

