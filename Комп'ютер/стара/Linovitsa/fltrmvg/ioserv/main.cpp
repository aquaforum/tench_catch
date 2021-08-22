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

    QCoreApplication::setOrganizationName("Rabitsa");
    QCoreApplication::setApplicationName("filter1s");

    QSettings set;
    set.setValue("/db/db","filter1s");


    RxModbus s1,s2;
    s1.setObjectName("filter1s");
    s1.loadList(":/text/list.txt");
    s1.setHostName("192.168.16.15");
    s1.setPort(502);


    QVector<IoDev*> src; // джерела даних.
    src << &s1  ;
    Logging l(src);
    QStringList tables;
    tables << "trend";
    l.setTables(tables);

    s1.start();

    IoNetServer s(src);

    /*
    Alert al(&s1);
    al.loadList(":/text/alert.txt");

    QObject::connect(&s1,SIGNAL(updateData()),&al,SLOT(checkAlert()));
    QObject::connect(&s2,SIGNAL(updateData()),&al,SLOT(checkAlert()));

    QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));

    QObject::connect(&s1,SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));
*/

    return a.exec();
}

