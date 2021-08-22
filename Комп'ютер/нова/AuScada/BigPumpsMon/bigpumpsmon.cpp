#include "bigpumpsmon.h"
#include "ui_bigpumpsmon.h"
#include <QByteArray>
#include <QString>
#include <QLCDNumber>
#include <QDebug>
#include <aulib.h>

#define BPMON QStringLiteral("plcbp")

BigPumpsMon::BigPumpsMon(AuBase &source, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BigPumpsMon),
    src(source),
    count(0)
{
    ui->setupUi(this);

    connect(&src[BPMON],&AuPlc::updateData,this,&BigPumpsMon::slotUpdate);


}

BigPumpsMon::~BigPumpsMon()
{
    delete ui;
}

// тобі потрібно написати свою реалізацію  цієї функції
// яка буде двіставати дані із
void BigPumpsMon::slotUpdate()
{
    // це саме смачне - добираємося до даниж
    AuPlc &X=*qobject_cast<AuPlc*>(sender());

    /*foreach(QLineEdit *l, findChildren<QLineEdit *>())
    {
        l->setText(X[l->objectName()], 'G', 2));
    }*/

    ui->V0001->setText(X[QStringLiteral("V0001")]);
    ui->V0002->setText(X[QStringLiteral("V0002")]);
    ui->V0003->setText(X[QStringLiteral("V0003")]);


    ui->V0041->setText(QString::number(X[QStringLiteral("V0021")]*X[QStringLiteral("V0041")]*0.00173,'f',0));
    ui->V0042->setText(QString::number(X[QStringLiteral("V0022")]*X[QStringLiteral("V0042")]*0.00173,'f',0));
    ui->V0043->setText(QString::number(X[QStringLiteral("V0023")]*X[QStringLiteral("V0043")]*0.00173,'f',0));
    ui->SP0001->setText(QString::number(X[QStringLiteral("SP0001")]/400.0,'f',0));

    ui->V0021->setText(X[QStringLiteral("V0021")]);
    ui->V0022->setText(X[QStringLiteral("V0022")]);
    ui->V0023->setText(X[QStringLiteral("V0023")]);

    ui->C0001->setText(X[QStringLiteral("C0001")]);
    ui->C0002->setText(X[QStringLiteral("C0002")]);
    ui->C0003->setText(X[QStringLiteral("C0003")]);

    ui->V0059->setText(X[QStringLiteral("V0059")]);
    ui->V0058->setText(X[QStringLiteral("V0058")]);

    if(X[QStringLiteral("Run0001")]!= 0)
    {
        ui->Work0001->setText(tr("Робота"));
        ui->Work0001->setStyleSheet("background-color:#0bff13;");
    }
    else
    {
        ui->Work0001->setText(tr("Зупинено"));
        ui->Work0001->setStyleSheet("background-color:yellow;");
    }
    if(X[QStringLiteral("Run0002")] != 0)
    {
        ui->Work0002->setText(tr("Робота"));
        ui->Work0002->setStyleSheet("background-color:#0bff13;");
    }
    else
    {
        ui->Work0002->setText(tr("Зупинено"));
        ui->Work0002->setStyleSheet("background-color:yellow;");
    }
    if(X[QStringLiteral("Run0003")] != 0)
    {
        ui->Work0003->setText(tr("Робота"));
        ui->Work0003->setStyleSheet("background-color:#0bff13;");
    }
    else
    {
        ui->Work0003->setText(tr("Зупинено"));
        ui->Work0003->setStyleSheet("background-color:yellow;");
    }
    if(X[QStringLiteral("Link0001")]!=0)
    {
        ui->Link0001->setText(tr("На зв'язку"));
        ui->Link0001->setStyleSheet("background-color:#fff01d;");
    }
    else
    {
        ui->Link0001->setText(tr("Немає зв'язку!"));
        ui->Link0001->setStyleSheet("background-color:#ff0000;");
    }
    if(X[QStringLiteral("Link0002")]!=0)
    {
        ui->Link0002->setText(tr("На зв'язку"));
        ui->Link0002->setStyleSheet("background-color:#fff01d;");
    }
    else
    {
        ui->Link0002->setText(tr("Немає зв'язку!"));
        ui->Link0002->setStyleSheet("background-color:#ff0000;");
    }
    if(X[QStringLiteral("Link0003")]!=0)
    {
        ui->Link0003->setText(tr("На зв'язку"));
        ui->Link0003->setStyleSheet("background-color:#fff01d;");
    }
    else
    {
        ui->Link0003->setText(tr("Немає зв'язку!"));
        ui->Link0003->setStyleSheet("background-color:#ff0000;");
    }
    if(X[QStringLiteral("Alarm0001")]==0)
    {
        if(X[QStringLiteral("L0001")]!=0)
        {
            ui->Alarm0001->setText(tr("Дистанційне"));
            ui->Alarm0001->setStyleSheet("background-color:#0088ff;");
        }
        else
        {
            ui->Alarm0001->setText(tr("Місцеве"));
            ui->Alarm0001->setStyleSheet("background-color:#0088ff;");
        }
    }
    else
    {
        ui->Alarm0001->setText(tr("Аварія!!"));
        ui->Alarm0001->setStyleSheet("background-color:#ff0000;");
    }
    if(X[QStringLiteral("Alarm0002")]==0)
    {
        if(X[QStringLiteral("L0002")]!=0)
        {
            ui->Alarm0002->setText(tr("Дистанційне"));
            ui->Alarm0002->setStyleSheet("background-color:#0088ff;");
        }
        else
        {
            ui->Alarm0002->setText(tr("Місцеве"));
            ui->Alarm0002->setStyleSheet("background-color:#0088ff;");
        }
    }
    else
    {
        ui->Alarm0002->setText(tr("Аварія!!"));
        ui->Alarm0002->setStyleSheet("background-color:#ff0000;");
    }
    if(X[QStringLiteral("Alarm0003")]==0)
    {
        if(X[QStringLiteral("L0003")]!=0)
        {
            ui->Alarm0003->setText(tr("Дистанційне"));
            ui->Alarm0003->setStyleSheet("background-color:#0088ff;");
        }
        else
        {
            ui->Alarm0003->setText(tr("Місцеве"));
            ui->Alarm0003->setStyleSheet("background-color:#0088ff;");
        }
    }
    else
    {
        ui->Alarm0003->setText(tr("Аварія!!"));
        ui->Alarm0003->setStyleSheet("background-color:#ff0000;");
    }

    ui->C0002slider->setValue(X[QStringLiteral("C0002")]);
    ui->C0003slider->setValue(X[QStringLiteral("C0003")]);
    if(X[QStringLiteral("AM0001")] != 0)
    {
        ui->SP0001->setStyleSheet("background-color:#0bff13;");
    }
    else
    {
        ui->SP0001->setStyleSheet("background-color:#fff01d;");
    }



}
