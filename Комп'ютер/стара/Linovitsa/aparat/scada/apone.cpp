#include "apone.h"
#include "ui_apone.h"

#include <QtGui>
#include "iodev.h"


ApOne::ApOne(/*IoDev &source,*/ QWidget *parent) :
    QLabel(parent),
    //src(source),
    ui(new Ui::ApOne)
{
    ui->setupUi(this);

    cb
            << ui->c1_X_sir
            << ui->c1_Y_sir
            << ui->c1_X_amsir
            << ui->c1_X_voda
            << ui->c1_Y_voda
            << ui->c1_X_amvoda
            << ui->c1_X_vk
            << ui->c2_X_vk
            << ui->c1_X_vo
            << ui->c1_X_amvk
            << ui->c1_Y_vk ;

    lcd
            << ui->lc_V_r
            << ui->lc_V_m
            << ui->lc_V_l
            << ui->lc_V_vak
            << ui->lc_V_temp
            << ui->lc_Spr_m
            << ui->lc_Spr_r;

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
    foreach(QCheckBox* p ,cb)
    {
        p->setChecked(src.getValue16(p->objectName().right(p->objectName().size()-3)));
    }
    foreach(QLCDNumber *p, lcd)
    {
        double v=src.getValueScaled(p->objectName().right(p->objectName().size()-3));
        //qDebug() << p->objectName().right(p->objectName().size()-3) << v;
        if(v<0.0)
            p->display(QString("---"));
        else
            p->display(QString("%1").arg(v,0,'f',1));

        //qDebug() << p->objectName() << src.getValueFloat(p->objectName().right(p->objectName().size()-3));
    }
    ui->pb_V_l->setValue(src.getValueScaled("V_l"));

}


