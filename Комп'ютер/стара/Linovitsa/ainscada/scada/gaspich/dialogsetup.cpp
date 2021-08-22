#include "dialogsetup.h"
#include "ui_dialogsetup.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMessageBox>

//#include "iodev.h"

DialogSetup::DialogSetup(IoDev &s, QWidget *parent) :
    QDialog(parent),bAccess(true),
    ui(new Ui::DialogSetup),
    src(s)
{
    ui->setupUi(this);

    connect(ui->bnOk,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(ui->bnCancel,SIGNAL(clicked()),this,SLOT(reject()));




    ui->maxTaGcl->setValue(src.getValueScaled("maxTaGcl"));
    ui->maxTaGst->setValue(src.getValueScaled("maxTaGst"));

    ui->subGcl->setValue(src.getValueScaled("subGcl"));
    ui->subGst->setValue(src.getValueScaled("subGst"));

    ui->zdTloadCl->setValue(src.getValue32("zdTloadCl")/1000.0);
    ui->zdLoadSt->setValue(src.getValue32("zdLoadSt")/1000.0);
    ui->zdDelayCl->setValue(src.getValue32("zdDelayCl")/1000.0);
    ui->zdTPostCl->setValue(src.getValue32("zdTPostCl")/1000.0);

    ui->zdTalarmStart->setValue(src.getValue32("zdTalarmStart")/1000.0);
    ui->zdTwaitStart->setValue(src.getValue32("zdTwaitStart")/1000.0);
    ui->zdTalarmLift->setValue(src.getValue32("zdTalarmLift")/1000.0);
    ui->zdTwaitLift->setValue(src.getValue32("zdTwaitLift")/1000.0);


    ui->zdTn01->setValue(src.getValue32("zdTn01")/1000.0);
    ui->zdTn02->setValue(src.getValue32("zdTn02")/1000.0);
    ui->zdTn03->setValue(src.getValue32("zdTn03")/1000.0);

    ui->zdTv01->setValue(src.getValue32("zdTv01")/1000.0);
    ui->zdTv02->setValue(src.getValue32("zdTv02")/1000.0);
    ui->zdTv03->setValue(src.getValue32("zdTv03")/1000.0);


    ui->zdTliftTR->setValue(src.getValue32("zdTliftTR")/1000.0);
    ui->zdTUpWait->setValue(src.getValue32("zdTUpWait")/1000.0);
    ui->zdTark->setValue(src.getValue32("zdTark")/1000.0);
    ui->zdTTorm->setValue(src.getValue32("zdTTorm")/1000.0);

    ui->zdTaccelUp->setValue(src.getValue32("zdTaccelUp")/1000.0);
    ui->zdTaccelDown->setValue(src.getValue32("zdTaccelDown")/1000.0);

    ui->ZdTShovel->setValue(src.getValue32("ZdTShovel")/1000.0);

    ui->iKGcl->setValue(src.getValueFloat("iKGcl"));
    ui->iKGst->setValue(src.getValueFloat("IKGst"));

#ifndef WIN32
    // перевірка наявності файлу розмежування доступу
    QFile fAccess("/usr/etc/ac.list");
    if(fAccess.exists())
    {
        bAccess=false;
        QDir dlist("/dev/disk/by-uuid");
        QStringList list=dlist.entryList();
        list.removeAt(0); // видалити два перших елементи
        list.removeAt(0);
        fAccess.open(QIODevice::ReadOnly);
        for(;!fAccess.atEnd();)
        {
            QString key=fAccess.readLine().trimmed();
            if(list.contains(key)) bAccess=true;
        }


    }
#endif

}

DialogSetup::~DialogSetup()
{
    delete ui;
}

void DialogSetup::slotAccept()
{

    // src.sendValue("kW",ui->scaleGcl->value()/ui->scaleGst->value());

    if(bAccess)
    {
    src.sendValueScaled("maxTaGcl",ui->maxTaGcl->value());
    src.sendValueScaled("maxTaGst",ui->maxTaGst->value());

    src.sendValueScaled("subGcl",ui->subGcl->value());
    src.sendValueScaled("subGst",ui->subGst->value());




    src.sendValue("zdTloadCl",qint32(ui->zdTloadCl->value()*1000.0));
    src.sendValue("zdLoadSt",qint32(ui->zdLoadSt->value()*1000.0));
    src.sendValue("zdDelayCl",qint32(ui->zdDelayCl->value()*1000.0));
    src.sendValue("zdTPostCl",qint32(ui->zdTPostCl->value()*1000.0));


    src.sendValue("zdTalarmStart",qint32(ui->zdTalarmStart->value()*1000.0));
    src.sendValue("zdTwaitStart",qint32(ui->zdTwaitStart->value()*1000.0));
    src.sendValue("zdTalarmLift",qint32(ui->zdTalarmLift->value()*1000.0));
    src.sendValue("zdTwaitLift",qint32(ui->zdTwaitLift->value()*1000.0));

    src.sendValue("zdTark",qint32(ui->zdTark->value()*1000.0));

    src.sendValue("zdTn01",qint32(ui->zdTn01->value()*1000.0));
    src.sendValue("zdTn02",qint32(ui->zdTn02->value()*1000.0));
    src.sendValue("zdTn03",qint32(ui->zdTn03->value()*1000.0));

    src.sendValue("zdTv01",qint32(ui->zdTv01->value()*1000.0));
    src.sendValue("zdTv02",qint32(ui->zdTv02->value()*1000.0));
    src.sendValue("zdTv03",qint32(ui->zdTv03->value()*1000.0));

    src.sendValue("zdTUpWait",qint32(ui->zdTUpWait->value()*1000.0));
    src.sendValue("zdTTorm",qint32(ui->zdTTorm->value()*1000.0));

    src.sendValue("zdTliftTR",qint32(ui->zdTliftTR->value()*1000.0));

    src.sendValue("zdTaccelUp",qint32(ui->zdTaccelUp->value()*1000.0));
    src.sendValue("zdTaccelDown",qint32(ui->zdTaccelDown->value()*1000.0));
    src.sendValue("ZdTShovel",qint32(ui->ZdTShovel->value()*1000.0));

    src.sendValue("iKGcl",ui->iKGcl->value());
    src.sendValue("IKGst",ui->iKGst->value());

    }
    else
    {
         QMessageBox::information(this,tr("Повідомлення"),tr("Ключ доступу  не знайдено.\nПараметри не змінено"));

    }
    accept();
}



