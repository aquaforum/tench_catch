#include "amVater.h"
#include "ui_amVater.h"
#include <aulib.h>

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


RAmVater::RAmVater(AuPlc &source,QStringList &tagList,QWidget *p)
    : QDialog(p) ,
      ui(new Ui::AmVater),
      src(source),
      tags(tagList)

{
        ui->setupUi(this);


        //var << "Y_01" << "X_07" << "V_33" << "V_25" << "SP_V_33_min" << "SP_V_33_max" << "SP_V_25_min" << "AM_Y_01";
        SAFE(ui->cbY->setChecked(src[tags[AmV::cbY]]));

        SAFE(
        if(src[tags[AmV::cbAM]]==0)
        {
            connect(ui->cbY,SIGNAL(clicked(bool)),this,SLOT(slotManY(bool)));
        })

        SAFE(ui->cbAM->setCurrentIndex(src[tags[AmV::cbAM]]==0?0:1));

        SAFE(ui->sbSPzb->setValue(src[tags[AmV::SPzb]]));
        SAFE(ui->vsSPzb->setValue(src[tags[AmV::SPzb]].valueReal()));

        SAFE(ui->sbSPkr->setValue(src[tags[AmV::SPkr]]));
        SAFE(ui->vsSPkr->setValue(src[tags[AmV::SPkr]].valueReal()));

        SAFE(ui->sbDSPzb->setValue(src[tags[AmV::DSPzb]]));
        SAFE(ui->sbDSPkr->setValue(src[tags[AmV::DSPkr]]));


        connect(ui->cbAM,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetAM(int)));
	
        connect(ui->sbSPzb,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));
        connect(ui->vsSPzb,SIGNAL(valueChanged(int)),this,SLOT(slotSetValue(int)));
	
        connect(ui->sbSPkr,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));
        connect(ui->vsSPkr,SIGNAL(valueChanged(int)),this,SLOT(slotSetValue(int)));

        connect(ui->sbDSPzb,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));
        connect(ui->sbDSPkr,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));

        connect(&src,&AuPlc::updateData,this,&RAmVater::updateData);



}

RAmVater::~RAmVater()
{
    delete ui;
}

void RAmVater::updateData() // поновлення даних у вікні
{
    SAFE(ui->cbX->setChecked(src[tags[AmV::cbX]]!=0));

    SAFE(ui->pbPV_1->setValue(src[tags[AmV::PV_1]].value32()));
    SAFE(ui->pbPV_2->setValue(src[tags[AmV::PV_2]].value32()));

    SAFE(ui->ldPV_1->display((double)src[tags[AmV::PV_1]]));
    SAFE(ui->ldPV_2->display((double)src[tags[AmV::PV_2]]));

    if(ui->cbAM->currentIndex()==1)
        SAFE(ui->cbY->setChecked(src[tags[AmV::cbY]]));
}

void RAmVater::slotSetAM(int v) // перемикач ручний-автомат
{
    SAFE(src[tags[AmV::cbAM]]=qint16(-v));

    if(v)
        disconnect(ui->cbY,SIGNAL(clicked(bool)),this,SLOT(slotManY(bool)));
    else
        connect(ui->cbY,SIGNAL(clicked(bool)),this,SLOT(slotManY(bool)));
}


void RAmVater::slotManY(bool v)  //ручне упарвління клапаном
{
    SAFE(src[tags[AmV::cbY]]=qint16(v?-1:0));
}

void RAmVater::slotSetValue(double v) // встановлення параметрів
{
    QString s=sender()->objectName();
    if(s=="sbSPzb")
    {
        SAFE(src[tags[AmV::SPzb]]=v);
        ui->vsSPzb->blockSignals(true);
        ui->vsSPzb->setValue(v*40.0);
        ui->vsSPzb->blockSignals(false);
    }
    else if(s=="sbSPkr")
    {
        SAFE(src[tags[AmV::SPkr]]=v);
        ui->vsSPkr->blockSignals(true);
        ui->vsSPkr->setValue(v*40.0);
        ui->vsSPkr->blockSignals(false);

    }
    else if(s=="sbDSPzb")
    {
        SAFE(src[tags[AmV::DSPzb]]=v);
    }
    else if(s=="sbDSPkr")
    {
        SAFE(src[tags[AmV::DSPkr]]=v);
    }

}

void RAmVater::slotSetValue(int v) // встановлення парамертів
{
    QString s=sender()->objectName();

    if(s=="vsSPzb")
    {
        SAFE(src[tags[AmV::SPzb]].setValue(v));
        ui->sbSPzb->blockSignals(true);
        ui->sbSPzb->setValue((double)v/40.0);
        ui->sbSPzb->blockSignals(false);
    }
    else if(s=="vsSPkr")
    {
        SAFE(src[tags[AmV::SPkr]].setValue(v));
        ui->sbSPkr->blockSignals(true);
        ui->sbSPkr->setValue((double)v/40.0);
        ui->sbSPkr->blockSignals(false);

    }
}


