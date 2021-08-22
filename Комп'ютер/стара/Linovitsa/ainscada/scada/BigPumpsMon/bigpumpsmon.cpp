#include "bigpumpsmon.h"
#include "ui_bigpumpsmon.h"
#include <QByteArray>
#include <QString>
#include <QLCDNumber>
#include "xudpclient.h"
#include <QDebug>

BigPumpsMon::BigPumpsMon(quint16 crcIx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BigPumpsMon),
    index(crcIx),
    count(0)
{
    ui->setupUi(this);

}

BigPumpsMon::~BigPumpsMon()
{
    delete ui;
}

// тобі потрібно написати свою реалізацію  цієї функції
// яка буде двіставати дані із
void BigPumpsMon::slotUpdate(quint16 crcIx)
{
    // це саме смачне - добираємося до даниж
    xUdpClient &X=*qobject_cast<xUdpClient*>(sender()); // тут перетворюємо вказівник на відправника на посилання на xUdpClient

    /*foreach(QLineEdit *l, findChildren<QLineEdit *>())
    {
        l->setText(QString::number(X[crcIx][l->objectName()], 'G', 2));
    }*/
    ui->V0001->setText(QString::number(X[crcIx]["V0001"], 'f', 2));
    ui->V0002->setText(QString::number(X[crcIx]["V0002"], 'f', 2));
    ui->V0003->setText(QString::number(X[crcIx]["V0003"], 'f', 2));

    ui->V0041->setText(QString::number(X[crcIx]["V0021"]*X[crcIx]["V0041"]*0.00173, 'f', 1));
    ui->V0042->setText(QString::number(X[crcIx]["V0022"]*X[crcIx]["V0042"]*0.00173, 'f', 1));
    ui->V0043->setText(QString::number(X[crcIx]["V0023"]*X[crcIx]["V0043"]*0.00173, 'f', 1));
    ui->SP0001->setText(QString::number(X[crcIx].valueFl("SP0001")/400.0, 'f', 2));

    ui->V0021->setText(QString::number(X[crcIx]["V0021"], 'g', 3));
    ui->V0022->setText(QString::number(X[crcIx]["V0022"], 'g', 3));
    ui->V0023->setText(QString::number(X[crcIx]["V0023"], 'g', 3));

    ui->C0001->setText(QString::number(X[crcIx]["C0001"], 'g', 3));
    ui->C0002->setText(QString::number(X[crcIx]["C0002"], 'g', 3));
    ui->C0003->setText(QString::number(X[crcIx]["C0003"], 'g', 3));

    ui->V0059->setText(QString::number(X[crcIx]["V0059"], 'g', 2));
    ui->V0058->setText(QString::number(X[crcIx]["V0058"], 'f', 1));

        if(X[crcIx].value16("Run0001")!= 0)
        {
            ui->Work0001->setText(tr("Робота"));
            ui->Work0001->setStyleSheet("background-color:#0bff13;");
        }
        else
        {
            ui->Work0001->setText(tr("Зупинено"));
            ui->Work0001->setStyleSheet("background-color:yellow;");
        }
        if(X[crcIx].value16("Run0002") != 0)
        {
            ui->Work0002->setText(tr("Робота"));
            ui->Work0002->setStyleSheet("background-color:#0bff13;");
        }
        else
        {
            ui->Work0002->setText(tr("Зупинено"));
            ui->Work0002->setStyleSheet("background-color:yellow;");
        }
        if(X[crcIx].value16("Run0003") != 0)
        {
            ui->Work0003->setText(tr("Робота"));
            ui->Work0003->setStyleSheet("background-color:#0bff13;");
        }
        else
        {
            ui->Work0003->setText(tr("Зупинено"));
            ui->Work0003->setStyleSheet("background-color:yellow;");
        }
        if(X[crcIx].value16("Link0001")!=0)
        {
            ui->Link0001->setText(tr("На зв'язку"));
            ui->Link0001->setStyleSheet("background-color:#fff01d;");
        }
        else
        {
            ui->Link0001->setText(tr("Немає зв'язку!"));
            ui->Link0001->setStyleSheet("background-color:#ff0000;");
        }
        if(X[crcIx].value16("Link0002")!=0)
        {
            ui->Link0002->setText(tr("На зв'язку"));
            ui->Link0002->setStyleSheet("background-color:#fff01d;");
        }
        else
        {
            ui->Link0002->setText(tr("Немає зв'язку!"));
            ui->Link0002->setStyleSheet("background-color:#ff0000;");
        }
        if(X[crcIx].value16("Link0003")!=0)
        {
            ui->Link0003->setText(tr("На зв'язку"));
            ui->Link0003->setStyleSheet("background-color:#fff01d;");
        }
        else
        {
            ui->Link0003->setText(tr("Немає зв'язку!"));
            ui->Link0003->setStyleSheet("background-color:#ff0000;");
        }
        if(X[crcIx].value16("Alarm0001")==0)
        {
            if(X[crcIx].value16("L0001")!=0)
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
        if(X[crcIx].value16("Alarm0002")==0)
        {
            if(X[crcIx].value16("L0002")!=0)
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
        if(X[crcIx].value16("Alarm0003")==0)
        {
            if(X[crcIx].value16("L0003")!=0)
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

        ui->C0002slider->setValue(X[crcIx].valueFl("C0002"));
        ui->C0003slider->setValue(X[crcIx].valueFl("C0003"));
        if(X[crcIx].value16("AM0001") != 0)
        {
            ui->SP0001->setStyleSheet("background-color:#0bff13;");
        }
        else
        {
            ui->SP0001->setStyleSheet("background-color:#fff01d;");
        }



}
