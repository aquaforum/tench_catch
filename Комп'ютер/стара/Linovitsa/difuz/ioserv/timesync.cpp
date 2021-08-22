#include "timesync.h"
#include <iodev.h>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QTime>

#define INTERVALS 3600
#define OFFSETS 300

TimeSync::TimeSync(IoDev &source,QObject *parent) :
    QObject(parent),src(source)
{
 // синхронізуватися зразу

    QTimer::singleShot(10000,this,SLOT(slotSetTime()));

}


void TimeSync::slotSetTime()
{
      // синхронізація годинника контролера, це би зробити автоматично......
    QVector<qint16> time;
    QDateTime ct=QDateTime::currentDateTime();
    time << ct.date().year() << ct.date().month() << ct.date().day() << ct.time().hour() << ct.time().minute() << ct.time().second() << -1;
    src.sendValue("yy_s",time);


    int hh=ct.time().hour();
    QDateTime stop=ct;

    if(hh<4)
    {
        stop.setTime(QTime(4,0,0));
    }
    else if(hh>16)
    {
        stop=stop.addDays(1);
        stop.setTime(QTime(4,0,0));
    }
    else
    {
        stop.setTime(QTime(16,0,0));
    }

    quint32 interval = (stop.toTime_t()-ct.toTime_t()+OFFSETS)*1000;
    qDebug() << ct << stop << interval;

    QTimer::singleShot(interval,this,SLOT(slotSetTime()));

}
