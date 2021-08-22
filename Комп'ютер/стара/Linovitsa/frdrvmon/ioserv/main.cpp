#include <QCoreApplication>
#include <QSettings>
#include <QDebug>


#include "iodev.h"
#include "RxModbus.h"
#include "logging.h"
#include "IoNetServer.h"
#include "alert.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("fqdrvmon");

    QSettings set;
    set.setValue("/db/db","fqdrvmon");


    RxModbus r,r1,r2,r3;

    r.loadList(":/text/list.txt");
    r.setHostName("plcfdmon");
    r.setPort(502);

    r1.loadList(":/text/list2.txt");
    r1.setHostName("plcfdmon2");
    r1.setPort(502);

    r2.setUnitIdentifier(255);
    r2.loadList(":/text/listfr.txt");
    r2.setHostName("fddiff1");
    r2.setPort(502);

    r3.setUnitIdentifier(255);
    r3.loadList(":/text/listfr.txt");
    r3.setHostName("fdvip1");
    r3.setPort(502);

    QVector<IoDev*> src; // джерела даних.
    src << &r << &r1 << &r2 << &r3;
    Logging l(src);
    QStringList tables;
    tables  << "trend" << "trend1" << "trend2" << "trend3" ;

    l.setTables(tables);


    r.start();
    r1.start();
    r2.start();
    r3.start();

    IoNetServer s(src,8185);

    Alert al(&r);
    al.loadList(":/text/alert.txt");

    QObject::connect(&r,SIGNAL(updateData()),&al,SLOT(checkAlert()));
    QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));

    QObject::connect(&r,SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));


    return a.exec();
}

