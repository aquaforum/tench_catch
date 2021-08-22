#include "filscam.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FilsCam w;
    w.showFullScreen();
    return a.exec();
}
