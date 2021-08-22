#include <QCoreApplication>
#include <QSettings>
#include <QDebug>


#include "iodev.h"
#include "RxModbus.h"
#include "logging.h"
#include "IoNetServer.h"
#include "alert.h"
#include "timesync.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("difuz");

    QSettings set;
    set.setValue("/db/db","difuz");


    RxModbus r;

    r.loadList(":/text/list.txt");
    r.setHostName("plcdifuz");
    r.setPort(502);

    QVector<IoDev*> src; // джерела даних.
    src << &r;
    Logging l(src);

    TimeSync tSync(r);

    r.start();

    IoNetServer s(src);

    Alert al(&r);
    al.loadList(":/text/alert.txt");

    QObject::connect(&r,SIGNAL(updateData()),&al,SLOT(checkAlert()));
    QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));

    QObject::connect(&r,SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));


    return a.exec();
}

