/*
 * 2017-10-19
 * Цей клас буде відповідати за обмін даними між прокладкою та клієнтскою частиною
 * ідея  в чому. Щоб зробити доступ доданих по принциму двовимірного масиву і позбавитися викликів get/send
 * отже шкальовані дані будуть діставатися таким чином
 * double value = X[crcIx][tagName];
 * value - в цю змінну будуть записані дані отримані із мережі
 * X - екземпляр класу xUdpClient
 * crcIx - ір адреса контролера, те, що повертається функцією QHostInfo::fromName("plchostnameorip").addresses()[0].toIPv4Address()
 * яка буде використовуватися при ініціалізації за допомогою виклику функції loadList
 * tagName - назва тега із list.txt
 * Важливо!!! цей оператор треба перегрузити так, щоб він працював в обидава боки, наприклад
 * X[crcIx][tagName]=data;
 * не шкальовані значення дістаються функціями value--(). Приклад
 * qint16 value= X[crcIx].value16(tagName); -
 * qint32 value= X[crcIx].value32(tagName); -
 * float value= X[crcIx].valueFl(tagName); -
 *
 * Функціонал локального сховища даних винесено в окримий клас. Див. xudpstore.h там реальзована більша частина описаного тут функціоналу
 *
 * Треба пропрацювати алгоритми визначення crcIx для задіювання в програмі
 *
 */

#include "xudpclient.h"

#include <QUdpSocket>
#include <QDataStream>
#include <QCryptographicHash>
#include <QFile>
#include <QHostInfo>

xUdpClient::xUdpClient(QObject *parent) : QObject(parent),sock(new QUdpSocket(this))
{
    sock->bind(8945);

//    connect(sock,&QUdpSocket::readyRead,this,&xUdpClient::slotRead);
    connect(sock,SIGNAL(readyRead()),this,SLOT(slotRead()));
}

/*
 * деструктор на всякий випадок
 * пам'ять після себе треба чистити
*/
xUdpClient::~xUdpClient()
{
    /*
    foreach (xUdpStore *p, store) {
        delete p;
    };
    store.clear();
    delete zero;
    */
}



/*
 * Функція loadList використувається для ініціалізації сховищ - xUdpStore
 * як параметер передається ім'я/ip контролера визначене функцією
 * та файл list.txt
 * повертає crcIx у випадку успіху. для того щоб його можна було зберегти та використовувати в майбутньому
 *
 *
*/

quint16 xUdpClient::loadList(QString hostName ,QString fileName)
{
    quint16 crcIx; // CRC16 який буде використовуватись як індекс
    QByteArray hash;
    quint32 ip = QHostInfo::fromName(hostName).addresses()[0].toIPv4Address();


    // визначити MD5 вхідного файла
    QFile fileList(fileName);
    fileList.open(QIODevice::ReadOnly);
    hash=QCryptographicHash::hash(fileList.readAll(),QCryptographicHash::Md5);
    fileList.close();

    hash.append((char*)(&ip),sizeof(ip)); // додати ір адресу контролера


    crcIx=qChecksum(hash.data(),hash.size()); // порахувати контрольну суму.

    store[crcIx]=xUdpStore(fileName);
    return crcIx;

}

/*
 * тут чинається саме цікаве. перегружений оператор [] повертає посилання на об'єкт із даними xUdpStore
 * питання в тому що він повинен повертати якщо вказаного об'єкта немає ?
*/
xUdpStore& xUdpClient::operator [](quint32 ip)
{
    if(store.contains(ip))
        return store[ip];
    else
        return zero; // тут треба повернути пустий служ
}
/*
 * функція slotRead() використовується для обробки отриманих даних та розпихування іх по xUdpStore
 * Опис протоколу.
 * перших чотири байти - сума CRC16 (qChecksum) із хеша MD5 файла list.txt доповненого чотрма байтами ір адреси контролера
 * далі йде вектор data_raw
 * і все це запакованою за допомогою  qCompress без параметрів.
*/

void xUdpClient::slotRead()
{
    quint16 crcIx;
    while (sock->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(sock->pendingDatagramSize());
        QHostAddress addr;
        sock->readDatagram(datagram.data(),datagram.size(),&addr);
        QByteArray data=qUncompress(datagram);
        QDataStream ds(&data,QIODevice::ReadOnly);
        ds >> crcIx;
        if(store.contains(crcIx)) // перевірити чи є куди зберігати.
            ds >> store[crcIx].dataRaw();
        //else
        //    qInfo("crcIx %x not found",crcIx);

        emit updateData(crcIx); // погукаємо, що є свіженькі дані

    }
}
