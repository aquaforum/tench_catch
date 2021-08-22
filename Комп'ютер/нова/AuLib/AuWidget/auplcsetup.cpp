#include "aulibdefs.h"
#include "auplcsetup.h"
#include <QFile>
#include <QRegExp>
#include <QSettings>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>

#include <QMessageBox>

#include <QNetworkInterface>

#define OFF -1 // виключено, обмін даними в буль-якому вигляді відсутній
#define LOCALONLY 0 // тільки локальний обмін, без мережі
#define LOCALMON 1 // локальний з віддачею даних в мережу
#define LOCALFULL 2 // локальний обмін з повним доступом по мережі
#define NETFULL 3 //  повний доступ по мережі, якщо на іншійстанції буде LOCALFULL
#define NETMON 4 //  повний доступ по мережі, якщо на іншійстанції буде LOCALFULL


AuPlcSetup::AuPlcSetup(QString plcList, QWidget *parent) : QWidget(parent)
  , restart(false)
{
    QSettings set;
    QGridLayout *grLayout = new QGridLayout(this);
    setLayout(grLayout);

    QFile fPlcList(plcList);
    fPlcList.open(QIODevice::ReadOnly);

    int row=0;
    while(!fPlcList.atEnd())
    {
        QString fPlcLine=fPlcList.readLine().trimmed();
        if(fPlcLine.size()>0)
        {
            QString plcName=fPlcLine.split(';').at(0).trimmed();
            QString plcText=fPlcLine.split(';').at(3).trimmed();
            QLabel *name=new QLabel(this);
            name->setText(plcText);

            grLayout->addWidget(name,row,0);

            QComboBox *cb=new QComboBox(this);
            cb->addItem(QStringLiteral("Вимкнено"),OFF);
            cb->addItem(QStringLiteral("Тільки локально"),LOCALONLY);
            cb->addItem(QStringLiteral("Локільно+передавач"),LOCALMON);
            cb->addItem(QStringLiteral("Локально+мережа"),LOCALFULL);
            cb->addItem(QStringLiteral("Мережа повний"),NETFULL);
            cb->addItem(QStringLiteral("Мережа монітор"),NETMON);
            cb->setObjectName(plcName);

            int cbIx=set.value(plcName,
                   #ifndef WIN32
                                                       OFF
                   #else
                                                       NETMON
                   #endif
                               ).toInt();
            if(cbIx<OFF) cbIx=OFF;
            if(cbIx>NETMON) cbIx=NETMON;

            cb->setCurrentIndex(cbIx+1);
            grLayout->addWidget(cb,row++,1);

            connect(cb,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&AuPlcSetup::slotPlcSet);

        }

    }


    QLabel *ifaceText=new QLabel(this);
    ifaceText->setText(QStringLiteral("Слухати інтерфейс"));
    grLayout->addWidget(ifaceText,row,0);

    QComboBox *ifaceList=new QComboBox(this);
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces()) {
        ifaceList->addItem(iface.name());
    }
    grLayout->addWidget(ifaceList,row++,1);

    if(set.contains("iface"))
    {
        ifaceList->setCurrentText(set.value("iface","lo").toString());
    }
    else
        ifaceList->setCurrentIndex(1);
    connect(ifaceList,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&AuPlcSetup::slotSetIface);


    grLayout->addItem(new QSpacerItem(20, 53, QSizePolicy::Minimum, QSizePolicy::Expanding),row++,0);



    fPlcList.close();

    // розмістити себе
    QVBoxLayout *plcLayout = new QVBoxLayout(parent);
    plcLayout->addWidget(this);
    parent->setLayout(plcLayout);
}

void AuPlcSetup::slotPlcSet(int v)
{
    QSettings set;
    set.setValue(sender()->objectName(),v-1);
    restart=true;
}

AuPlcSetup::~AuPlcSetup()
{
    if(restart)
        QMessageBox::information(this,QStringLiteral("Повідомлення"),QStringLiteral("Увага\nЗменено конфігурацію вводу-виводу\nПотрібно перезапустити програму"));
}

void AuPlcSetup::slotSetIface(int)
{
    QComboBox *p=qobject_cast<QComboBox*>(sender());
    QSettings set;
    set.setValue("iface",p->currentText());
}

