#include "amVater.h"
#include "ui_amVater.h"
#include <iodev.h>

#include <QDebug>
#include <QTimer>

#define K2 6
#define ZB 1
#define AM 8

namespace AmV
{
    enum Index {
        cbY=0, //<< "Y0001"
        cbX=1, //<< "X0005"
        PV_1=2, //<< "V0003"
        PV_2=3,//<< "V0004"
        SPzb=4,//<< "SP0025"
        DSPzb=5, //<< "SP2025"
        SPkr=6, //<< "SP0026"
        DSPkr=7, // << "SP2026"
        cbAM = 8//<< "AM0025"
    };
}


RAmVater::RAmVater(IoDev &source,QStringList &tagList,QWidget *p)
    : QDialog(p) ,
      ui(new Ui::AmVater),
      src(source),
      tags(tagList)

{
        ui->setupUi(this);


        //var << "Y_01" << "X_07" << "V_33" << "V_25" << "SP_V_33_min" << "SP_V_33_max" << "SP_V_25_min" << "AM_Y_01";
        ui->cbY->setChecked(src.getValue16(tags[AmV::cbY]));

        if(src.getValue16(tags[AmV::cbAM])==0)
        {
            connect(ui->cbY,SIGNAL(clicked(bool)),this,SLOT(slotManY(bool)));
        }

        ui->cbAM->setCurrentIndex(src.getValue16(tags[AmV::cbAM])==0?0:1);


        ui->sbSPzb->setValue(src.getValueScaled(tags[AmV::SPzb]));
        ui->vsSPzb->setValue(src.getValueFloat(tags[AmV::SPzb]));

        ui->sbSPkr->setValue(src.getValueScaled(tags[AmV::SPkr]));
        ui->vsSPkr->setValue(src.getValueFloat(tags[AmV::SPkr]));

        ui->sbDSPzb->setValue(src.getValueScaled(tags[AmV::DSPzb]));
        ui->sbDSPkr->setValue(src.getValueScaled(tags[AmV::DSPkr]));


        connect(ui->cbAM,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetAM(int)));
	
        connect(ui->sbSPzb,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));
        connect(ui->vsSPzb,SIGNAL(valueChanged(int)),this,SLOT(slotSetValue(int)));
	
        connect(ui->sbSPkr,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));
        connect(ui->vsSPkr,SIGNAL(valueChanged(int)),this,SLOT(slotSetValue(int)));

        connect(ui->sbDSPzb,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));
        connect(ui->sbDSPkr,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));

        QTimer *tmr = new QTimer(this);
        tmr->setInterval(1000);
        tmr->start();
        connect(tmr,SIGNAL(timeout()),this,SLOT(updateData()));

}

RAmVater::~RAmVater()
{
    delete ui;
}

void RAmVater::updateData() // ???????????????????? ?????????? ?? ??????????
{
    ui->cbX->setChecked(src.getValue16(tags[AmV::cbX])!=0);

    ui->pbPV_1->setValue(src.getValueFloat(tags[AmV::PV_1]));
    ui->pbPV_2->setValue(src.getValueFloat(tags[AmV::PV_2]));

    ui->ldPV_1->display(src.getValueScaled(tags[AmV::PV_1]));
    ui->ldPV_2->display(src.getValueScaled(tags[AmV::PV_2]));

    if(ui->cbAM->currentIndex()==1)
        ui->cbY->setChecked(src.getValue16(tags[AmV::cbY]));

}

void RAmVater::slotSetAM(int v) // ?????????????????? ????????????-??????????????
{
    src.sendValue(tags[AmV::cbAM],qint16(-v));

    if(v)
        disconnect(ui->cbY,SIGNAL(clicked(bool)),this,SLOT(slotManY(bool)));
    else
        connect(ui->cbY,SIGNAL(clicked(bool)),this,SLOT(slotManY(bool)));

}


void RAmVater::slotManY(bool v)  //?????????? ???????????????????? ????????????????
{
    src.sendValue(tags[AmV::cbY],qint16(v?-1:0));
    qDebug() << sender()->objectName() << tags[AmV::cbY] << v;

}

void RAmVater::slotSetValue(double v) // ???????????????????????? ????????????????????
{
    QString s=sender()->objectName();
    if(s=="sbSPzb")
    {
        src.sendValueScaled(tags[AmV::SPzb],v);
        ui->vsSPzb->blockSignals(true);
        ui->vsSPzb->setValue(v*40.0);
        ui->vsSPzb->blockSignals(false);
    }
    else if(s=="sbSPkr")
    {
        src.sendValueScaled(tags[AmV::SPkr],v);
        ui->vsSPkr->blockSignals(true);
        ui->vsSPkr->setValue(v*40.0);
        ui->vsSPkr->blockSignals(false);

    }
    else if(s=="sbDSPzb")
    {
        src.sendValueScaled(tags[AmV::DSPzb],v);
    }
    else if(s=="sbDSPkr")
    {
        src.sendValueScaled(tags[AmV::DSPkr],v);
    }

}

void RAmVater::slotSetValue(int v) // ???????????????????????? ????????????????????
{
    QString s=sender()->objectName();
    qDebug() << "slotSetValue(int v)" << v << s;

    if(s=="vsSPzb")
    {
        src.sendValue(tags[AmV::SPzb],(double)v);
        ui->sbSPzb->blockSignals(true);
        ui->sbSPzb->setValue((double)v/40.0);
        ui->sbSPzb->blockSignals(false);
    }
    else if(s=="vsSPkr")
    {
        src.sendValue(tags[AmV::SPkr],(double)v);
        ui->sbSPkr->blockSignals(true);
        ui->sbSPkr->setValue((double)v/40.0);
        ui->sbSPkr->blockSignals(false);

    }

}


