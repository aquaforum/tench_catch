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
//#include "form.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("centrif");

    QSettings set;
    set.setValue("/db/db","centrif");


    QVector<RxModbus*> r;
    QVector<IoDev*> src; // джерела даних.

    { // контролен загальних параметрів
        RxModbus *t= new RxModbus;
        t->setObjectName("cfall0");
        t->loadList(":/text/listall.txt");
        t->setHostName("plcap0");
        t->setPort(502);
        r << t;
        src << t;
    }

	// тут має бути ще два контролера одиночних центрифуг

    for(int i=4;i<6;++i){
        RxModbus *t= new RxModbus;
        t->setObjectName(QString("cf%1").arg(i));
        t->loadList(":/text/list.txt");
        t->setHostName(QString("plccf%1").arg(i));
        t->setPort(502);
        r << t;
        src << t;
    }

    Logging l(src,1010); // писалка історіх
    QStringList tables;
    tables << "trend" << "cf4" << "cf5"  ; //  << "cf" << "trend" ;
    l.setTables(tables);

    IoNetServer s(src,8185); // мережевий інтерфейс

    ReportWiter rep(src);

    //Alert al(&r);
    //al.loadList(":/text/Linovitsa/filters/text/alert.txt");
    //QObject::connect(&r,SIGNAL(updateData()),&al,SLOT(checkAlert()));
    //QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));
    for(int i=0;i<r.size();++i)
    {
        QObject::connect(r[i],SIGNAL(updateData()),&rep,SLOT(checkState()));
        QObject::connect(r[i],SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));
        r[i]->start();
    }
//    r[0]->start();

    qDebug() << "Started" ;
    return a.exec();
}

