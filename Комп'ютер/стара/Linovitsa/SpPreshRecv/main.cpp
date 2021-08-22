#include "sppreshdsp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpPreshDsp w;
    w.show();

    return a.exec();
}
