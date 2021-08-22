#include "reporwrmgr.h"
#include <aulib.h>

#include <QTimer>
#include <QDateTime>

#include "fabreportwriter.h"
#include <QDebug>

ReporWrMgr::ReporWrMgr(AuBase &base):
    src(base)
{
    setObjectName("ReportWrMgr");
    //runTimer();

    QTimer::singleShot(1000,this,&ReporWrMgr::runTimer);
    //QTimer::singleShot(10000,this,&ReporWrMgr::runReport);


}



void ReporWrMgr::deleteReportWriter()
{

}


void ReporWrMgr::runReport()
{
    //qDebug() << "Start FabReportWriter";

    FabReportWriter *frw=new FabReportWriter(src,this);
    // connect(frw,&FabReportWriter::finished,this,&ReporWrMgr::runTimer);
    connect(frw,&FabReportWriter::finished,frw,&FabReportWriter::deleteLater);

    QTimer::singleShot(10000,this,&ReporWrMgr::runTimer);

    // runTimer();
}

void ReporWrMgr::runTimer()
{
    QDateTime nextT=QDateTime::currentDateTime();
    // треба засікти моменти 7.45 та 19.45
    int h=nextT.time().hour()*3600+nextT.time().minute()*60+nextT.time().second();

    if(h<27900) // запустилися вночі
    {
        nextT.setTime(QTime(7,45,0,0)); //  той же день на ранок
    }
    else if(h>71100) // запустилися ввечері
    {
        nextT=nextT.addDays(1);
        nextT.setTime(QTime(7,45,0,0)); //   на ранок наступного дня
    }
    else // запустилися вдень
    {
        nextT.setTime(QTime(19,45,0,0)); //  той же день на вечір
    }

    QDateTime cdt=QDateTime::currentDateTime();

    qint32 interval=(nextT.toTime_t()-cdt.toTime_t())*1000;


    QTimer::singleShot(interval,this,&ReporWrMgr::runReport);

    qDebug() << objectName() << " run at:" << nextT << " interval " << interval ;

}
