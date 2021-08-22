#ifndef XUDPCLIENT_H
#define XUDPCLIENT_H

#include <QObject>
#include <QHash>
#include <QString>
#include "xudpstore.h"

class QUdpSocket;

class xUdpClient : public QObject
{
    Q_OBJECT
public:
    explicit xUdpClient(QObject *parent);
    ~xUdpClient();

    quint16 loadList(QString hostName, QString fileName);
    QList<quint16> crcX(){
        return store.keys();
    }

    xUdpStore &operator [] (quint32);


signals:
    void updateData(quint16);

public slots:

private slots:
    void slotRead();

private:
    QUdpSocket *sock;
    xUdpStore zero; // це фітивне сховище для обробки неіснуючих екземплярів ip

    // сховище даних
    // ключ відповідного джерела визначається по CRC16(qChecksum) сумі із MD5 хеша файла list.txt
    // доровненого чотирма байтами ір адреси контролера.
    QHash<quint16 , xUdpStore> store;
    
    
};

#endif // XUDPCLIENT_H
