#include "IoUdpServer.h"
#include "../modbus/RxModbus.h"

#include <QDebug>


IoUdpServer::IoUdpServer(QVector<RxModbus*> s,int nPort)
    :src(s),socket(new QUdpSocket(this))
{
    socket->bind(QHostAddress::Any,8184);
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));

    foreach (RxModbus *p, s) {
        connect(p,SIGNAL(updateData()),this,SLOT(slotSendDatagram()));
    }

}

IoUdpServer::~IoUdpServer()
{

}


void IoUdpServer::slotSendDatagram()
{
    QString sName=sender()->objectName();
    QByteArray data;
    QTextStream out(&data,QIODevice::WriteOnly);
    out << sName << "\n";
    out << qobject_cast<RxModbus*>(sender())->data_raw();
    socket->writeDatagram(qCompress(data),QHostAddress::Broadcast,8184);


}

void IoUdpServer::slotReadyRead()
{

}


void IoUdpServer::sendAlert(QString v)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << qint8('R') << qint8('A') << qint8(-1) << qint16(0) << qint16(0) << v;
    out.device()->seek(5); // переміститись до поля із довжиною
    out << qint16(arrBlock.size()-7); // записати довжину блоку даних




}
