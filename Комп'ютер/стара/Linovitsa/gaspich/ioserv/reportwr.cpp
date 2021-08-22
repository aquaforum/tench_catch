#include "reportwr.h"
#include "iodev.h"
#include <QtSql>

ReportWiter::ReportWiter(QVector<IoDev*> &source):
        src(source),prevGst(-1.0)

{
    // ініціалізувати масив станів
    lastState.fill(0,5);

    lastState[0]=0x7FFFFFFF;

    // ініціалізувати масив даних
    QVector<double> v(11);
    data.fill(v,5) ;

}


void ReportWiter::checkState()
{
    int i=0; //sender()->objectName().right(1).toInt()-1; // визначити хто відправив сигнал
    
    qint16 s=src[i]->getValue16("State");
    qint16 X0048 = src[i]->getValue16("X0048");
    int Nc=src[i]->getValue32("Nc");

    if( Nc > lastState[i]  ) //якщо була регенерація а стала не регенерація - записати звіт
    {

            QDateTime tm=QDateTime::currentDateTime();
            //qDebug() << "Report" << sender()->objectName();
        QSqlDatabase dbs=QSqlDatabase::database("logging",true); //знайти з’єдання logging
        if(dbs.isOpen()) // якщо база відкрита
        {

            if(prevGst<0.0) // ініціалізувати змінну поточним значенням
                prevGst=data[i][3]-data[i][1];

            QSqlQuery query(dbs);
            // сформувати і виконати запит, перевірити результат
            if(! query.exec(QString("INSERT INTO report(cft,Nc,Tall,Gcl,tmGcl,Gst,tmGst,prevGst,Mode) "
                               "VALUES (\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\','%9')")
                       .arg(tm.toString("yyyy/MM/dd hh:mm:ss"))
                       .arg(Nc)
                       .arg(data[i][0])
                       .arg(data[i][1])
                       .arg(data[i][2])
                       .arg(data[i][3])
                       .arg(data[i][4])
                       .arg(prevGst)
                            .arg(src[i]->getValue16("X0020")==0?tr("P"):tr("A"))
                       )
                    )

            { // якщо помилка
                qDebug() << query.lastError().databaseText(); //на екран
                qDebug() << query.lastQuery();
            }

            prevGst=data[i][3]-data[i][1];

            dbs.close();
        }

    }

    if(s==12 || X0048 ) // якщо стадія регенераці зберегти дані, бо після її закінчення контролер їх обнулить.
    {
        data[i][0]=src[i]->getValue32("Tall")/1000;
        data[i][1]=src[i]->getValueScaled("Gcl");
        data[i][2]=src[i]->getValue32("tmGcl")/1000;
        data[i][3]=src[i]->getValueScaled("Gst");
        data[i][4]=src[i]->getValue32("tmGst")/1000;
    }
    lastState[i]=Nc; // зберегти стан для наступного виклику

}


