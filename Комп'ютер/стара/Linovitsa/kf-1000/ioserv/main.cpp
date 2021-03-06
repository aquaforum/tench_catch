#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include "reportwr.h"

#include "iodev.h"
#include "RxModbus.h"
#include "logging.h"
#include "IoNetServer.h"
#include "alert.h"


//#include "display.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("kf1000");

    QSettings set;
    set.setValue("/db/db","kf1000");


    QVector<RxModbus*> r;
    QVector<IoDev*> src; // джерела даних.


    for(int i=0;i<3;++i)
    {
        RxModbus *t= new RxModbus;
        t->setObjectName(QString("kf_%1").arg(i+1));
        t->loadList(":/text/list.txt");
        t->setHostName(QString("plckf%1").arg(i+1));
        t->setPort(502);
        r << t;
        src << t;
    }

    RxModbus *t= new RxModbus;
    t->setObjectName(QString("common"));
    t->loadList(":/text/list2.txt");
    t->setHostName(QString("plckf0"));
    t->setPort(502);
    r << t;
    src << t;


    Logging l(src,5000); // писалка історіх
    QStringList tables;
    tables << "kf_1" << "kf_2" << "kf_3" << "trend" ;
    l.setTables(tables);

    IoNetServer s(src); // мережевий інтерфейс

    ReportWiter rep(src);

    //Alert al(&r);
    //al.loadList(":/text/Linovitsa/filters/text/alert.txt");
    //QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));

    for(int i=0;i<4;++i)
    {
        if(i<3) QObject::connect(r[i],SIGNAL(updateData()),&rep,SLOT(checkState()));

        QObject::connect(r[i],SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));
        //QObject::connect(&r,SIGNAL(updateData()),&al,SLOT(checkAlert()));

        r[i]->start();
    }
    //r[3]->start();

    qDebug() << "Started" ;
    return a.exec();
}

