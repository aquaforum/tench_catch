#include "dlgapsetup.h"
#include "ui_dlgapsetup.h"

#include "aulib.h"

DlgApSetup::DlgApSetup(AuPlc &source,QWidget *parent) :
    QDialog(parent),
    src(source),
    ui(new Ui::DlgApSetup)
{
    ui->setupUi(this);
    dsb
            << ui->SP_H_n
            << ui->SP_R_zv
            << ui->SP_H_2
            << ui->SP_H_3
            << ui->SP_H_f
            << ui->SP_M_d1
            << ui->SP_M_2
            << ui->SP_M_d2
            << ui->SP_M_3
            << ui->SP_M_d3
            << ui->SP_M_f
            << ui->SP_R_min
            << ui->SP_R_d
            << ui->Kpr
            << ui->Ti
            << ui->Td
            << ui->dR_max
            << ui->dH_max
            << ui->SP_R_max
            << ui->Rp_1
            << ui->Rp_2
            << ui->Rp_3
            << ui->Rz_2
            << ui->Rz_3
            << ui->Rz_4

               ;

    foreach(QDoubleSpinBox* p,dsb)
    {
        //p->setValue(src.getValueScaled(p->objectName()));
        connect(p,SIGNAL(valueChanged(double)),this,SLOT(slotSetValue(double)));

    }

    //ui->rej_rost->setCurrentIndex(src.getValue16("rej_rost")?1:0);
    //ui->rej_voda->setCurrentIndex(src.getValue16("rej_voda")?1:0);
    //ui->rej_p->setCurrentIndex(src.getValue16("rej_p")?1:0);
    connect(ui->rej_rost,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetValue(int)));
    connect(ui->rej_voda,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetValue(int)));
    connect(ui->rej_p,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetValue(int)));

    //ui->TM_zd->setValue(src.getValue32("TM_zd")/1000);
    connect(ui->TM_zd,SIGNAL(valueChanged(int)),this,SLOT(slotSetTZ(int)));
/*
    foreach(QString tag,src.getTags().keys())
    {
        if(src.isScaleChange(tag))
            tag_name << tag;
    }

    foreach(QString str,tag_name)
    {
        ui->bx_Teg->addItem(src.getText()[str]);
    }
*/
    connect(ui->bx_Teg,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));

    connect(ui->sb_Fs,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(ui->sb_Zs,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(ui->sb_f,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));



}

DlgApSetup::~DlgApSetup()
{
    delete ui;
}

void DlgApSetup::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void DlgApSetup::slotSetValue(double )
{
    //src.sendValueScaled(sender()->objectName(),v);
    //src.sendValue("Save",qint16(-1));
}

void DlgApSetup::slotSetValue(int )

{
    //src.sendValue(sender()->objectName(),qint16(-v));
    //src.sendValue("Save",qint16(-1));
}

void DlgApSetup::slotSetTZ(int )
{
    //src.sendValue(sender()->objectName(),qint32(v*1000));
    //src.sendValue("Save",qint16(-1));
}

void DlgApSetup::slotSet(QString  )
{
    /*
    if(sender()->objectName()=="sb_Zs")
        src.setScaleZero(tag_name[ui->bx_Teg->currentIndex()],v.toDouble());
    else if(sender()->objectName()=="sb_Fs")
        src.setScaleFull(tag_name[ui->bx_Teg->currentIndex()],v.toDouble());
    else if(sender()->objectName()=="sb_f")
        src.sendValue(QString("f_%1").arg(tag_name[ui->bx_Teg->currentIndex()]),qint16(v.toInt()));
*/
}

void DlgApSetup::selectTeg(int )
{
    //ui->sb_Zs->setValue(src.scaleZero(tag_name[v]));
    //ui->sb_Fs->setValue(src.scaleFull(tag_name[v]));
    //ui->sb_f->setValue(src.getValue16(QString("f_%1").arg(tag_name[v])));

    //m_ui->sb_m->setValue((double)s.getValue16(QString("m_%1").arg(tag_name[v]))/4000.0*(s.scaleFull(tag_name[v])-s.scaleZero(tag_name[v]))+s.scaleZero(tag_name[v]));
}
