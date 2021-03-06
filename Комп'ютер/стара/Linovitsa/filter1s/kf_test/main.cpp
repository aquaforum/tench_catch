#include <QApplication>
#include <QSettings>
#include <QDebug>


#include "iodev.h"
#include "RxModbus.h"
#include "form.h"

#ifdef WIN32
#include <QtPlugin>
Q_IMPORT_PLUGIN(qsvg)
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("kf_test");

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    RxModbus r;

    r.loadList(":/text/list.txt");
    r.setHostName("192.168.1.72");
    r.setPort(502);

    r.start();
    Form w(r);

    QObject::connect(&r,SIGNAL(updateData()),&w,SLOT(updateData()));

    w.show();

    return a.exec();
}

