#include "apone.h"
#include "ui_apone.h"

#include "valve_mask.h"

#include <QMessageBox>

#include <aulib.h>


ApOne::ApOne(AuPlc &source, QWidget *parent) :
    QLabel(parent),
    ui(new Ui::ApOne),
    src(source)
{
    setObjectName("ApOne");

    ui->setupUi(this);

//    cb

    foreach(QLCDNumber *p,findChildren<QLCDNumber*>())
    {
        lcd << p;
    }

    connect(&src,&AuPlc::updateData,this,&ApOne::updateData);



}

ApOne::~ApOne()
{
    delete ui;
}

void ApOne::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ApOne::updateData()
{
    QString tag;

    foreach(QCheckBox* p ,cb)
    {
        tag=p->objectName().right(p->objectName().size()-3);
        SAFE(p->setChecked(src[tag].value()!=0);)
    }
    qint32 St_i=0;
    SAFE(St_i=src[QStringLiteral("St_i")]);
    ui->K4f_in->setChecked(St_i&MASK_K4f_in);
    ui->K6f_in->setChecked(St_i&MASK_K6f_in);
    ui->K7f_in->setChecked(St_i&MASK_K7f_in);
    ui->K8f_in->setChecked(St_i&MASK_K8f_in);

    ui->K10f_in->setChecked(St_i&MASK_K10f_in);
    ui->K191f_in->setChecked(St_i&MASK_K191f_in);
    ui->K18f_in->setChecked(St_i&MASK_K18f_in);

    SAFE(ui->Pt6a_ind->setText(src[QStringLiteral("Pt6a_ind")]));
    SAFE(
    ui->timeProc->setText(QString("%1:%2").arg(src[QStringLiteral("dication_hour")].value(),2,10,QChar('0'))
                          .arg(src[QStringLiteral("Indication_min")].value(),2,10,QChar('0')));
    )
    SAFE(ui->Vakuum_ind->setText(src[QStringLiteral("Vakuum_ind")]));
    SAFE(ui->K4->setText(src[QStringLiteral("K4")]));
    SAFE(ui->K6->setText(src[QStringLiteral("K6")]));

    SAFE(ui->St_i->setText(src[QStringLiteral("St_i")]));

    foreach(QLCDNumber *p, lcd)
    {
        tag=p->objectName().right(p->objectName().size()-3);
        double v=-1.0;
        SAFE(v=src[tag]);
        if(v<0.0)
            p->display(QString("---"));
        else
            p->display(QString("%1").arg(v,0,'f',1));

    }
    SAFE(ui->pb_Lt1a_ind->setValue(src[QStringLiteral("Lt1a_ind")]));
}


