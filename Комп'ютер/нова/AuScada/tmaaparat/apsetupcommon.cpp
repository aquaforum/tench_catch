#include "apsetupcommon.h"
#include "ui_apsetupcommon.h"
#include <aulib.h>
#include "apmainform.h"

#define TAP1 QStringLiteral("tmaap1")
#define TAP2 QStringLiteral("tmaap2")
#define TAP3 QStringLiteral("tmaap3")
#define TAPA QStringLiteral("tmaob")

ApSetupCommon::ApSetupCommon(AuBase &base, ApMainForm *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApSetupCommon),
    src(base)
{
    ui->setupUi(this);

    AuPlc &plcOb=src[TAPA];

    foreach(QDoubleSpinBox *p,findChildren<QDoubleSpinBox*>())
    {
        SAFE(p->setValue(plcOb[p->objectName()]));
        connect(p,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&ApSetupCommon::setValue);
    }


    SAFE(ui->Bp->setChecked(plcOb["Bp"]));
    SAFE(ui->Zp->setChecked(plcOb["Zp"]));

    connect(ui->Bp,&QCheckBox::clicked,this,&ApSetupCommon::setChecked);
    connect(ui->Zp,&QCheckBox::clicked,this,&ApSetupCommon::setChecked);

    connect(ui->bnApSetup_1,&QPushButton::clicked,p,&ApMainForm::slotChangeMnemo);
    connect(ui->bnApSetup_2,&QPushButton::clicked,p,&ApMainForm::slotChangeMnemo);
    connect(ui->bnApSetup_3,&QPushButton::clicked,p,&ApMainForm::slotChangeMnemo);

}

ApSetupCommon::~ApSetupCommon()
{
    delete ui;
}

void ApSetupCommon::setValue(double v)
{
    SAFE(src[TAPA][sender()->objectName()]=v);
}

void ApSetupCommon::setChecked(bool v)
{
    SAFE(src[TAPA][sender()->objectName()]=v);
}


