#include "sppreshdsp.h"
#include "ui_sppreshdsp.h"

#include <QUdpSocket>
#include <QBitArray>

SpPreshDsp::SpPreshDsp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpPreshDsp),
  sock(new QUdpSocket(this))
{
    ui->setupUi(this);
    sock->bind(8845);
    connect(sock,SIGNAL(readyRead()),this,SLOT(slotRecv()));

    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(slotSend(int)));

}

SpPreshDsp::~SpPreshDsp()
{
    delete ui;
}

void SpPreshDsp::slotRecv()
{
    while (sock->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(sock->pendingDatagramSize());
        QHostAddress addr;
        sock->readDatagram(datagram.data(),datagram.size(),&addr);
        qDebug() << datagram.data();
        ui->spPresh->setText(QString("%1\n").arg(addr.toString())+datagram);
    }
}


void SpPreshDsp::slotSend( int v )
{
    QUdpSocket s;
    s.writeDatagram(QString("spPresh=%1\n").arg(v).toLatin1(),QHostAddress::Broadcast,8845);
}

