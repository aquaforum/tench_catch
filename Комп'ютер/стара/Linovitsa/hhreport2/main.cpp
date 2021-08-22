#include <QCoreApplication>
#include "hhreport.h"

#include <QSettings>
#include <QString>
#include <QStringList>

#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    HhReport report;

    return a.exec();
}
