#include "bigpumpsmon.h"
#include <QApplication>
#include "xudpclient.h"
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // створюємо об'єкт-приймач даних
    xUdpClient xCli;
    // завантажуємо файл із списком тегів та ір контролера
    // це потрібно буде робити для кожного контролера від якого хочемо отримувати дані.
    // допустимі  різні комбінації ір та list.txt
    quint16 crcIx = xCli.loadList("192.168.15.91",":/text/list.txt");

    BigPumpsMon w(crcIx); //

    // це дуже важливий рядок. за допомогою цього з'єднання відбувається передача інформації про надходження нової порції даних
    QObject::connect(&xCli,&xUdpClient::updateData,&w,&BigPumpsMon::slotUpdate);

    w.show();

    return a.exec();
}
