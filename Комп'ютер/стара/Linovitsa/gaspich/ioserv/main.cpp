#include <QCoreApplication>
#include <QSettings>
#include <QDebug>


#include "iodev.h"
#include "RxModbus.h"
#include "logging.h"
#include "IoNetServer.h"
#include "alert.h"
#include "reportwr.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Kashperivka");
    QCoreApplication::setApplicationName("gaspich");

    QSettings set;
    set.setValue("/db/db","gaspich");


    RxModbus r;

    r.loadList(":/text/list.txt");
    r.setHostName("plcgpi4");
    r.setPort(502);

    QVector<IoDev*> src; // джерела даних.
    src << &r;
    Logging l(src);

    r.start();

    IoNetServer s(src);

    Alert al(&r);
    al.loadList(":/text/alert.txt");

    ReportWiter rep(src);


    QObject::connect(&r,SIGNAL(updateData()),&al,SLOT(checkAlert()));
    QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));

    QObject::connect(&r,SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));

    QObject::connect(&r,SIGNAL(updateData()),&rep,SLOT(checkState()));


    return a.exec();
}

