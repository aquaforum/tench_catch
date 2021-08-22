#include "aucollectdata.h"
#include "aulib.h"

#include <QTimer>
#include <QDateTime>

AuCollectData::AuCollectData(AuPlc &plc, int interval, QObject *parent) : QObject(parent),
    myPlc(plc),
    myInterval(interval*1000)
{
    // запустити таймер, який раз в хвилину скидуватиме дані в базу
    // вирахувати список полів, які записуються в історію
    foreach(QString tag,myPlc.tagsList())
    {
        if(myPlc[tag].logging())
        {
            fields << tag;
        }
    }

    QTimer::singleShot(0,Qt::PreciseTimer,this,&AuCollectData::slotCollectData);
}


void AuCollectData::slotCollectData()
{
    int cTime = myInterval-QDateTime::currentDateTime().time().msecsSinceStartOfDay()%myInterval+10;
    if(cTime<0 ) cTime=1;
    if(cTime>myInterval+10) cTime=myInterval+10;
    QTimer::singleShot(cTime,Qt::PreciseTimer,this,&AuCollectData::slotCollectData);

    qint32 Dt=QDateTime::currentDateTime().toTime_t();

    // qDebug() <<  QDateTime::currentDateTime().time() << QDateTime::currentDateTime().time().msecsSinceStartOfDay()%myInterval << cTime;

    QStringList values;
    values.clear();
    for(int i=0;i<fields.size();++i)
    {
        QString tag=fields[i];
        QString val;
        if(myPlc.dbReal() && myPlc[tag].dataType()==tagType::REAL) // якщо його писати в реалах
            val=QString::number(myPlc[tag].valueReal(),'f');
        else
            val=QString::number(myPlc[tag].value32());
        values << val;
    }


    emit pushData( QStringLiteral("REPLACE INTO %1(Dt,%2) VALUES(%3,%4)")
                   .arg(myPlc.name())
                   .arg(fields.join(','))
                   .arg(QString::number(Dt))
                   .arg(values.join(','))) ;


}
