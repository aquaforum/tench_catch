#ifndef __RNETIOSERVER_H__
#define __RNETIOSERVER_H__

#include <QObject>
#include <QtNetwork>
#include <QtCore>

#include "../header.h"

class RxModbus;

class IoUdpServer : public QObject
{
Q_OBJECT
public:
    IoUdpServer(QVector<RxModbus*> s,int nPort=8184);
    ~IoUdpServer();

public slots:
    void slotSendDatagram();
    void slotReadyRead();
    void sendAlert(QString);

private:
    QVector<RxModbus*> src;
    QUdpSocket *socket;
};


#endif
