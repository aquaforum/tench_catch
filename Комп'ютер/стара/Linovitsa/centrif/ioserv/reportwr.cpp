#include "reportwr.h"
#include "iodev.h"
#include <QtSql>

ReportWiter::ReportWiter(QVector<IoDev*> &source):
        src(source)

{
    // ініціалізувати масив станів
    lastState.fill(0,10);

    // ініціалізувати масив даних
    QVector<int> v(12);
    data.fill(v,5) ;

}

#define STATESTORE 15

void ReportWiter::checkState()
{

    int dev=sender()->objectName().right(1).toInt(); // визначити хто відправив сигнал
    if (dev>0)
    {


        int i=dev-3 ;


        //qDebug() << "dev" << dev << "i" << i << src.size() << sender()->objectName() ;
        dev=i; // !!!!!!!!!
        qint16 s=src[dev]->getValue16("State");
        // qDebug() << "Io #" << i << "Get status" << s;
        if(lastState[i]==STATESTORE && s!=STATESTORE) //якщо була регенерація а стала не регенерація - записати звіт
        {
            QSqlDatabase dbs=QSqlDatabase::database("logging",true); //знайти з’єдання logging
            if(dbs.isOpen()) // якщо база відкрита
            {
                QDateTime tm=QDateTime::currentDateTime();
                QSqlQuery query(dbs);
                // сформувати і виконати запит, перевірити результат
                if(! query.exec(QString("INSERT INTO report(cft,nCf,Nc,T_all,Gzag,Q_prl,T_prl,T_fug,Q_pr,T_pr,T_nt,Q_reg,T_reg, Q_man,Qall) "
                                   "VALUES (\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\',\'%9\',\'%10\',\'%11\',\'%12\',\'%13\',\'%14\', \'%15\')")
                           .arg(tm.toString("yyyy/MM/dd hh:mm:ss"))
                           .arg(i+1)
                           .arg(src[dev]->getValue32("Nc"))
                           .arg(data[i][0])
                           .arg(data[i][1])
                           .arg(data[i][2])
                           .arg(data[i][3])
                           .arg(data[i][4])
                           .arg(data[i][5])
                           .arg(data[i][6])
                           .arg(data[i][7])
                           .arg(data[i][8])
                           .arg(data[i][9])
                           .arg(data[i][10])
                                .arg(data[i][11]) )
                )
                { // якщо помилка
                    qDebug() << query.lastError().databaseText(); //на екран
                    qDebug() << query.lastQuery();
                }
                qDebug() << query.lastQuery();


                dbs.close();
            }
        }

        if(s==STATESTORE) // якщо стадія регенераці зберегти дані, бо після її закінчення контролер їх обнулить.
        {
            data[i][0]=src[dev]->getValue16("T_all");
            data[i][1]=src[dev]->getValueFloat("Gzag");
            data[i][2]=src[dev]->getValueFloat("Q_prl");
            data[i][3]=src[dev]->getValue16("T_prl");
            data[i][4]=src[dev]->getValue16("T_fug");
            data[i][5]=src[dev]->getValueFloat("Q_pr");
            data[i][6]=src[dev]->getValue16("T_pr");
            data[i][7]=src[dev]->getValue16("T_nt");
            data[i][8]=src[dev]->getValueFloat("Q_reg");
            data[i][9]=src[dev]->getValue16("T_reg");
            data[i][10]=src[dev]->getValueFloat("Q_man");
            data[i][11]=src[dev]->getValueFloat("Qall");
        }
        lastState[i]=s; // зберегти стан для наступного виклику

    }
}


