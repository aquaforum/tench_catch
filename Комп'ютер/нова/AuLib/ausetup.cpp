#include "aulibdefs.h"
#include "ausetup.h"
#include <aulib.h>
#include "aulogging.h"
#include "autcpreceiver.h"
#include "autcptransmitter.h"

#include <QFile>
#include <QSettings>
#include <QStringList>
#include <QRegExp>

#include <QDebug>


// все інше розглядається як режим монітора.

AuSetup::AuSetup(QString plcList,QObject *parent) : QObject(parent),
    src(new AuBase(plcList))
  ,transiver(nullptr) // ініціалізувати нулем для того щоб потім перевірити чи воно існує
  ,tcpTx(nullptr)
  ,tcpRx(nullptr)
  ,myLogging(nullptr)
{
    QSettings s;
    AuPlcDrive *plc;

    QFile filePlcList(plcList);
    filePlcList.open(QIODevice::ReadOnly);
    while(!filePlcList.atEnd())
    {
        QString line=filePlcList.readLine().trimmed();
        QString plcName=line.split(';').at(0).trimmed(); // отримати ім'я контролера

        if(plcName.size()>0)
        {
            qint32 commType=s.value(plcName,
#ifndef WIN32
                                    OFF
#else
                                    NETMON
#endif
                                    ).toInt(); // отримати тип комунікації
            //qDebug() << plcName << commType;
            switch (commType) {
            default:
            case OFF: // виключено, нічого не робити
                break;
            case LOCALONLY: // тільки обмін з контролером
                plc=new AuModbus(*src,(*src)[plcName].index(),(*src)[plcName].offset(),this);
                plc->setObjectName(plcName);

                src->addActivePlc(plc);
                plc->start();
                connect(plc,&AuModbus::Alert,src,&AuBase::getAlert);

                break;

            case LOCALMON: // обмін з контролером і надсилання даних в мережу
                if(transiver==nullptr)
                {
                    transiver= new AuUdpTransiver(*src,this);                
                }

                (*src)[plcName].setNet(true); // ця опція потрібна для того щоб потім можна було для початку відправити шкали в мережу
                plc=new AuModbus(*src,(*src)[plcName].index(),(*src)[plcName].offset(),this);
                plc->setObjectName(plcName);
                src->addActivePlc(plc);

                plc->start();
                connect(plc,&AuModbus::Alert,src,&AuBase::getAlert);

                break;
            case LOCALFULL: // обмін з контролером і надсилання та прийом даних з мережі
                if(transiver==nullptr)
                {
                       transiver= new AuUdpTransiver(*src,this);
                }

                (*src)[plcName].setNet(true);
                plc=new AuModbus(*src,(*src)[plcName].index(),(*src)[plcName].offset(),this);
                plc->setObjectName(plcName);
                src->addActivePlc(plc);

                plc->start();
                connect(plc,&AuModbus::Alert,src,&AuBase::getAlert);

                if(tcpRx==nullptr)
                {
                    tcpRx=new AuTcpReceiver(*src,this);

                }
                tcpRx->addPlcDrive(plcName,plc);

                break;
            case NETFULL: // отримання і нідсилання даних з мережі
                if(transiver==nullptr)
                {
                    transiver= new AuUdpTransiver(*src,this);
                }
                if(tcpTx==nullptr)
                {
                    tcpTx=new AuTcpTransmitter(transiver->scadaAddreses(),this);
                }

                (*src)[plcName].setNet(true);
// #warning Це треба переробити
                foreach (AuTag *tag, (*src)[plcName].tags()) { // підключаємось до тегів для відправки даних в контролер
                    connect(tag,QOverload<QString,qint32,QVector<qint16>&>::of(&AuTag::sendData),tcpTx,&AuTcpTransmitter::setData);
                }

                //connect(transiver,QOverload<quint16>::of(&AuUdpTransiver::updateData),&(*src)[plcName] ,&AuPlc::setData); // блін, зірочки-карлючки....
                break;

            case NETMON: // тіьки отримання даних з мережі, без відправки назад
                if(transiver==nullptr)
                {
                    transiver= new AuUdpTransiver(*src,this);
                }
                if(tcpTx==nullptr)
                {
                    tcpTx=new AuTcpTransmitter(transiver->scadaAddreses(),this);
                }

                //connect(transiver,QOverload<quint16>::of(&AuUdpTransiver::updateData),&(*src)[plcName] ,&AuPlc::setData); // блін, зірочки-карлючки....
                break;
            }

        }

    }

    // Створюємо писалку історії, за потреби
    if(src->activePlcCount()>0)
    {
        myLogging=new AuLogging(*src,this); // тут є ідея включити цей функціонал напряму в AuBase
        // myLogging->setStoreInterval(1000);
    }

    src->setTransmitter(tcpTx);
    src->setReceiver(tcpRx);

    // переслка шкал далі
    // connect(tcpTx,&AuTcpTransmitter::scaleResult,src,&AuBase::inputScaleResult);

    // це для обміну історією через мережу
    if(tcpTx)
    {
        connect(tcpTx,&AuTcpTransmitter::resultRemoteQuery,src,&AuBase::processQuery);
        connect(tcpTx,&AuTcpTransmitter::tableFinded,src,&AuBase::setTablePlcName);
    }

    if(transiver!=nullptr)
    {
        // це потенційне місце петлі, треба дуже уважно перевірити умову відправки сигналу AuBase::updateData
        connect(transiver,QOverload<quint16>::of(&AuUdpTransiver::updateData),src,&AuBase::setData);
        connect(src,&AuBase::updateData,transiver,&AuUdpTransiver::sendData); // це відправить дані в порт

        src->setTransiver(transiver);
        transiver->setObjectName("AuUdpTransiver");
        connect(transiver,&AuUdpTransiver::Alert,src,&AuBase::getAlert);
    }

}

AuSetup::~AuSetup()
{
    delete src;  //Враховуючи що воно нащадок QObject то можливо його не треба видаляти....
}
