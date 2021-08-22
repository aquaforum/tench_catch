#include "aulibdefs.h"
#include "autagsetup.h"
#include "ui_autagsetup.h"
#include "aulib.h"
#include "autcptransmitter.h"

#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QMessageBox>
#include <QRandomGenerator>

#include <QDebug>

/* Алгоритм синхронізації шкал
 *  ЦЕ ВЖЕ ЗАСТАЛІЛО І БУДЕ ПЕРЕРОБЛЕНО
 * 1.  НЕМАЄ НІЯКОГО ГОЛОВНОГО КОМПʼЮТАРА ІЗ ШКАЛАМИ.
 * всі компи в цьому плані рівноправні.
 * Якщо на якомусь компі міняють значення параметрів myZero myFull myBase та myDecimal (див. autag.h)
 * тоді цей клас (AuTagScale) само собою міняє локальні значення цих змінних
 * і в деструкторі відправліє їх у мережу, код функції UDP_PARAM
 * ЩЕ РАЗ !!! Відправліє тільки те, що по факту мінялося.
 * всі інші отримавши цей пакунок тихенько міняють їх собі
 * 2. Якщо хтось хоче  отримати всі параметри з мережі він робить запит UDP_ALLPARAM_SEND
 * всі cусіди, хто отримав цей пакунок і в кого є активні контролери відповідають пакунком UDP_ALLPARAM_RECV
 * який обробляється не в AuUdpTransiver, а тут (див. slotDatagram). Трансівер тільки переселає його далі.
 * Якщо є змінені відносно місцевих параметрів вони будуть виведені за допомогою MessageBox
 *
 * */

AuTagSetup::AuTagSetup(AuBase &base, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuTagSetup),
    src(base),
    lCrc(0)
{
    ui->setupUi(this);

    // заповнити список котролерів
    QString plcName;
    foreach (plcName, src.plcList().keys()) {
        ui->plcName->addItem(src[plcName].text(),plcName);
    }
    //ui->plcName->setCurrentIndex(0);
    // plcName=ui->plcName->currentData().toString();
    slotPlcChange(ui->plcName->currentIndex()); // Зповнити список тегів

    connect(ui->plcName,QOverload<int>::of(&QComboBox::currentIndexChanged) ,this,&AuTagSetup::slotPlcChange);
    connect(ui->tagName,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&AuTagSetup::slotTagChange);

    connect(ui->tagZero,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&AuTagSetup::setZero);
    connect(ui->tagFull,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&AuTagSetup::setFull);
    connect(ui->tagBase,QOverload<int>::of(&QSpinBox::valueChanged),this,&AuTagSetup::setBase);
    connect(ui->tagDp,QOverload<int>::of(&QSpinBox::valueChanged),this,&AuTagSetup::setDecimal);

    //ui->plcName->setCurrentIndex(0);// відправка сигнала

    // розмістити себе
    QVBoxLayout *tagLayout = new QVBoxLayout(parent);
    tagLayout->addWidget(this);
    parent->setLayout(tagLayout);

    //  відправка запиту на надсилання шкал та розбір результатів
    connect(ui->bnQueryParam,&QPushButton::clicked,this,&AuTagSetup::slotSendQuery);

    if(src.transmitter()!=nullptr)
    {
        connect(src.transmitter(),&AuTcpTransmitter::scaleResult,this,&AuTagSetup::slotDatagram);
    }

}

AuTagSetup::~AuTagSetup()
{
/*
 * Передача даних про шкалу та інші атрибути іде у вигляді QString де поля розділені символом ;
 * формат поля
 * QHash<String tagName <QHash<String type, double value> >
 *
*/
    QUdpSocket sock;
    sock.bind(QHostAddress(QHostAddress::AnyIPv4),0);
    sock.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);


    foreach(QString plcName,src.plcList().keys())
    {
        if(src[plcName].net() && netPush.keys().contains(plcName) ) // якщо відпавляємо в мережу і є що віддавати
        {

            QByteArray d;
            QDataStream out(&d,QIODevice::WriteOnly);
            out.setVersion(QTVER);

            out << plcName << quint8(UDP_PARAM)<< netPush[plcName];

            sock.writeDatagram(qCompress(d),QHostAddress(ADDR),PORTN);
            //qDebug() << plcName << netPush[plcName];


        }
    }

    delete ui;
}

//void AuTagSetup::slotPlcChange(const QString &pName)
void AuTagSetup::slotPlcChange(int v)
{

    QString plcName=ui->plcName->itemText(v);

   ui->tagName->blockSignals(true);
   ui->tagName->clear();
   QString tagName;
   foreach(tagName,src[plcName].tagsList())
   {
       if(src[plcName][tagName].scaleType()==tagScale::SET) // якщо потрібно встановлювати шкалу
       {
           ui->tagName->addItem(QStringLiteral("%1:%2").arg(tagName).arg(src[plcName][tagName].text()),tagName);
       }
   }

   ui->tagName->setCurrentIndex(0); // це повинно б викликати відповідний слот
   slotTagChange(ui->tagName->currentIndex());

   ui->tagName->blockSignals(false);

}

//void AuTagSetup::slotTagChange(const QString &)
void AuTagSetup::slotTagChange(int)
{

    QString tagName=ui->tagName->currentData().toString(); // дістати назву тега
    QString plcName=ui->plcName->currentData().toString();
    if(src[plcName].tagsList().contains(tagName))
    {
        SAFE(ui->tagText->setText(src[plcName][tagName].text()));

        ui->tagBase->blockSignals(true);
        SAFE(ui->tagBase->setValue(src[plcName][tagName].baseScale()));
        ui->tagBase->blockSignals(false);

        ui->tagZero->blockSignals(true);
        SAFE(ui->tagZero->setValue(src[plcName][tagName].zero()));
        ui->tagZero->blockSignals(false);

        ui->tagFull->blockSignals(true);
        SAFE(ui->tagFull->setValue(src[plcName][tagName].full()));
        ui->tagFull->blockSignals(false);

        ui->tagDp->blockSignals(true);
        SAFE(ui->tagDp->setValue(src[plcName][tagName].deciaml()));
        ui->tagDp->blockSignals(false);
    }
    else // якщо тегів гемає чи список пустий
    {
        ui->tagText->setText(QStringLiteral("пусто"));

        ui->tagBase->blockSignals(true);
        ui->tagBase->setValue(0);
        ui->tagBase->blockSignals(false);

        ui->tagZero->blockSignals(true);
        ui->tagZero->setValue(0);
        ui->tagZero->blockSignals(false);

        ui->tagFull->blockSignals(true);
        ui->tagFull->setValue(0);
        ui->tagFull->blockSignals(false);

        ui->tagDp->blockSignals(true);
        ui->tagDp->setValue(0);
        ui->tagDp->blockSignals(false);

    }

}

void AuTagSetup::setZero(double v)
{
    src[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()].setZero(v);
    netPush[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()][TAG_ZERO]=v;

}
void AuTagSetup::setFull(double v)
{
    src[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()].setFull(v);
    netPush[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()][TAG_FULL]=v;
}

void AuTagSetup::setBase(int v)
{
    src[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()].setBaseScale(v);
    netPush[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()][TAG_BASE]=v;
}

void AuTagSetup::setDecimal(int v)
{
    src[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()].setDecimal(v);
    netPush[ui->plcName->currentData().toString()][ui->tagName->currentData().toString()][TAG_DECIMAL]=v;
}

void AuTagSetup::slotSendQuery() // відправка запиту на отримання параметрів тегів
{
    src.scaleQuery(ui->plcName->currentData().toString());
}

void AuTagSetup::slotDatagram(const QString &plcName, const QHash<QString, QVector<double> > &param)
{
    QString diff="\n"; // різниця для змінених шкал.
    if(plcName==ui->plcName->currentData().toString()) // якщо дані для запрошеного контролера
    {
        //qDebug() << "Tag count: " << param.keys().size();

        foreach(QString tagName, param.keys())
        {
            // на випадок, якщо відсутні теги
            SAFE(
                        if(src[plcName][tagName].zero()!=param[tagName][TAG_ZERO])
                        {
                             diff.append(QStringLiteral("%1 %2 МІНІМУМ було %3 стало %4\n")
                                        .arg(src[plcName][tagName].text())
                                        .arg(tagName)
                                        .arg(src[plcName][tagName].zero())
                                        .arg(param[tagName][TAG_ZERO]));
                            src[plcName][tagName].setZero(param[tagName][TAG_ZERO]);
                        }

                        if(src[plcName][tagName].full()!=param[tagName][TAG_FULL])
                        {
                             diff.append(QStringLiteral("%1 %2 МАКСИМУМ було %3 стало %4\n")
                                        .arg(src[plcName][tagName].text())
                                        .arg(tagName)
                                        .arg(src[plcName][tagName].full())
                                        .arg(param[tagName][TAG_FULL]));
                            src[plcName][tagName].setFull(param[tagName][TAG_FULL]);
                        }

                        if(src[plcName][tagName].baseScale()!=param[tagName][TAG_BASE])
                        {
                             diff.append(QStringLiteral("%1 %2 БАЗА було %3 стало %4\n")
                                        .arg(src[plcName][tagName].text())
                                        .arg(tagName)
                                        .arg(src[plcName][tagName].baseScale())
                                        .arg(param[tagName][TAG_BASE]));
                            src[plcName][tagName].setBaseScale(param[tagName][TAG_BASE]);
                        }

                        if(src[plcName][tagName].deciaml()!=static_cast<int>(param[tagName][TAG_DECIMAL]))
                        {
                             diff.append(QStringLiteral("%1 %2 ДЕСЯТКОВИХ було %3 стало %4\n")
                                        .arg(src[plcName][tagName].text())
                                        .arg(tagName)
                                        .arg(src[plcName][tagName].deciaml())
                                        .arg(param[tagName][TAG_DECIMAL]));
                            src[plcName][tagName].setDecimal(param[tagName][TAG_DECIMAL]);
                        }

            );
        }
        // QMessageBox::information(this,QStringLiteral("Отримано параметри"),diff);

        if(param.contains(ui->tagName->currentData().toString())) // поновити дані на екрані.
        {
            slotTagChange(ui->tagName->currentIndex());
        }
    }

}
