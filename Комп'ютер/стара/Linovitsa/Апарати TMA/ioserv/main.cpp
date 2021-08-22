#include <QCoreApplication>
#include <QSettings>
#include <QDebug>


#include "iodev.h"
#include "RxModbusS.h"
#include "logging.h"
#include "IoNetServer.h"
#include "alert.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("tmaap");

    QSettings set;
    set.setValue("/db/db","aparat");

    QVector<IoDev*> src; // джерела даних.

    for(int i=1;i<4;++i)
    {
        RxModbusS *r=new RxModbusS;
        r->setObjectName(QString("tmaap%1").arg(i));
        r->setHostName(QString("tmaap%1").arg(i));
        r->setPort(502);

        r->loadList(":/text/list.txt");
        r->start();

        src << r;
    }

    {
        RxModbusS *r=new RxModbusS;
        r->setObjectName("tmaob");
        r->setHostName("tmaob");
        r->setPort(502);

        r->loadList(":/text/list2.txt");
        r->start();

        src << r;
    }

    Logging l(src);
    QStringList tables;
            tables << "ap1" << "ap2" << "ap3" << "trend";
    l.setTables(tables);

    IoNetServer s(src);

/*
    Alert al(&r);
    al.loadList(":/text/alert.txt");

    QObject::connect(&r,SIGNAL(updateData()),&al,SLOT(checkAlert()));
    QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));
    QObject::connect(&r,SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));
*/



    return a.exec();
}

