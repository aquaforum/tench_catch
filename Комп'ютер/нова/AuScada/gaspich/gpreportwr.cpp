#include "gpreportwr.h"
#include "aulib.h"
#include <QtSql>

GpReportWiter::GpReportWiter(AuPlc &source):
        src(source),prevGst(-1.0)

{
    // ініціалізувати масив станів
    lastState.fill(0,5);

    lastState[0]=0x7FFFFFFF;

    // ініціалізувати масив даних
    QVector<double> v(11);
    data.fill(v,5) ;

    connect(&src,&AuPlc::updateData,this,&GpReportWiter::checkState);

    // створення чи перевірка наявност таблиці

    QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME,true); //знайти з’єдання logging
    if(dbs.open())
    {
        if(!dbs.tables().contains("gpreport"))
        {
            QSqlQuery qry(dbs);
            if(!qry.exec(QStringLiteral("CREATE TABLE gpreport("
                                    "Dt INTEGER DEFAULT \'0\',"
                                    "Nc INTEGER DEFAULT \'0\',"
                                    "Tall INTEGER DEFAULT \'0\',"
                                    "Gcl INTEGER DEFAULT \'0\',"
                                    "tmGcl INTEGER DEFAULT \'0\',"
                                    "Gst INTEGER DEFAULT \'0\',"
                                    "tmGst INTEGER DEFAULT \'0\',"
                                    "prevGst INTEGER DEFAULT \'0\',"
                                    "Mode INTEGER DEFAULT \'0\'"
                                    ")"
                                    )))
            {
                qDebug() << qry.lastError().text() << "\n" << qry.lastQuery();
            }

        }
        dbs.close();
    }

}


void GpReportWiter::checkState()
{
    //AuPlc &s=*qobject_cast<AuPlc*>(sender());

    int i=0;
    
    qint16 s=src[QStringLiteral("State")];

    qint16 X0048 = src[QStringLiteral("X0048")];
    int Nc=src[QStringLiteral("Nc")];

    // qDebug() << sender();

    if( Nc > lastState[i]  ) //якщо змінився лічильник циклів
    {
        uint tm=QDateTime::currentDateTime().toTime_t(); // час пишимо в unix time
        //qDebug() << "Report" << sender()->objectName();
        QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME,true); //знайти з’єдання logging
        if(dbs.open()) // якщо база відкрита
        {
            if(prevGst<0.0) // ініціалізувати змінну поточним значенням
                prevGst=data[i][3]-data[i][1];

            QSqlQuery query(dbs);
            // сформувати і виконати запит, перевірити результат
            if(! query.exec(QString("INSERT INTO gpreport(Dt,Nc,Tall,Gcl,tmGcl,Gst,tmGst,prevGst,Mode) "
                               "VALUES (\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\','%9')")
                       .arg(tm)
                       .arg(Nc)
                       .arg(data[i][0])
                       .arg(data[i][1])
                       .arg(data[i][2])
                       .arg(data[i][3])
                       .arg(data[i][4])
                       .arg(prevGst)
                       .arg(src[QStringLiteral("X0020")]==0?tr("P"):tr("A"))
                       )
                    )

            { // якщо помилка
                qDebug() << query.lastError().databaseText(); //на екран
                qDebug() << query.lastQuery();
            }
/*
            else
            {
                qDebug() << query.lastQuery();
            }
*/
            prevGst=data[i][3]-data[i][1];

            dbs.close();
        }

    }

    if(s==12 || X0048 ) // якщо стадія регенераці зберегти дані, бо після її закінчення контролер їх обнулить.
    {
        data[i][0]=src[QStringLiteral("Tall")] ;
        data[i][1]=src[QStringLiteral("Gcl")];
        data[i][2]=src[QStringLiteral("tmGcl")] ;
        data[i][3]=src[QStringLiteral("Gst")];
        data[i][4]=src[QStringLiteral("tmGst")];
    }
    lastState[i]=Nc; // зберегти стан для наступного виклику

}


