#include <QCoreApplication>
#include "gasgetdata.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GasGetData data;

    return a.exec();
}
