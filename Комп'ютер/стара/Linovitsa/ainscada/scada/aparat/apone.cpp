#include "apone.h"
#include "ui_apone.h"

#include "valve_mask.h"

#include <QtGui>
#include <QMessageBox>

#include "iodev.h"


ApOne::ApOne(/*IoDev &source,*/ QWidget *parent) :
    QLabel(parent),
    //src(source),
    ui(new Ui::ApOne)
{
    ui->setupUi(this);

//    cb

    foreach(QLCDNumber *p,findChildren<QLCDNumber*>())
    {
        lcd << p;
    }



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

void ApOne::updateData(IoDev &src)
{
/*
    foreach(QCheckBox* p ,cb)
    {
        p->setChecked(src.getValue16(p->objectName().right(p->objectName().size()-3)));
    }
*/
    qint32 St_i= src.getValue32("St_i");
    ui->K4f_in->setChecked(St_i&MASK_K4f_in);
    ui->K6f_in->setChecked(St_i&MASK_K6f_in);
    ui->K7f_in->setChecked(St_i&MASK_K7f_in);
    ui->K8f_in->setChecked(St_i&MASK_K8f_in);

    ui->K10f_in->setChecked(St_i&MASK_K10f_in);
    ui->K191f_in->setChecked(St_i&MASK_K191f_in);
    ui->K18f_in->setChecked(St_i&MASK_K18f_in);

    ui->Pt6a_ind->setText(QString("%1").arg(src.getValueFloat("Pt6a_ind"),0,'f',1));
    ui->timeProc->setText(QString("%1:%2").arg(src.getValue16("dication_hour"),2,10,QChar('0'))
                          .arg(src.getValue16("Indication_min"),2,10,QChar('0')));
    ui->Vakuum_ind->setText(QString("%1").arg(src.getValueFloat("Vakuum_ind"),0,'f',1));
    ui->K4->setText(QString("%1%").arg(src.getValue16("K4")));
    ui->K6->setText(QString("%1%").arg(src.getValue16("K6")));

    ui->St_i->setText(QString("%1").arg(src.getValue32("St_i"),8,16,QChar('0')));

    foreach(QLCDNumber *p, lcd)
    {
        double v=src.getValueFloat(p->objectName().right(p->objectName().size()-3));
        //qDebug() << p->objectName().right(p->objectName().size()-3) << v;
        if(v<0.0)
            p->display(QString("---"));
        else
            p->display(QString("%1").arg(v,0,'f',1));

        //qDebug() << p->objectName() << src.getValueFloat(p->objectName().right(p->objectName().size()-3));
    }
    ui->pb_Lt1a_ind->setValue(src.getValueFloat("Lt1a_ind"));

}


