#include "dlgpumpctrl.h"
#include "ui_dlgpumpctrl.h"
#include "aulib.h"
#include <QTimer>

#include <QDebug>

dlgPumpCtrl::dlgPumpCtrl(AuPlc &source, QStringList &tags,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgPumpCtrl),
    src(source),
    t(tags)
{
    ui->setupUi(this);
    // ініціалізувати все
    ui->s_SP_max->setValue(src[t[Pc::Max]].value32());
    ui->s_SP_min->setValue(src[t[Pc::Min]].value32());
    ui->s_sSP_max->setValue(src[t[Pc::Max]]);
    ui->s_sSP_min->setValue(src[t[Pc::Min]]);

    ui->s_AM->setCurrentIndex(src[t[Pc::Am]]!=0?1:0);
    ui->s_R_M->setCurrentIndex(src[t[Pc::Y]]!=0?1:0);

    connect(ui->s_SP_max,SIGNAL(valueChanged(int)),this,SLOT(slotSetSl(int)));
    connect(ui->s_SP_min,SIGNAL(valueChanged(int)),this,SLOT(slotSetSl(int)));

    connect(ui->s_sSP_max,SIGNAL(valueChanged(double)),this,SLOT(slotSetDs(double)));
    connect(ui->s_sSP_min,SIGNAL(valueChanged(double)),this,SLOT(slotSetDs(double)));

    connect(ui->s_AM,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBool(int)));
    connect(ui->s_R_M,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBool(int)));
    ui->s_R_M->blockSignals(ui->s_AM->currentIndex());


    connect(&src,&AuPlc::updateData,this,&dlgPumpCtrl::slotUpdate);
}

dlgPumpCtrl::~dlgPumpCtrl()
{
    delete ui;
}

void dlgPumpCtrl::changeEvent(QEvent *e)
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

void dlgPumpCtrl::slotUpdate() // в цьому слоті буде поновлюватися інформація на картинці
{
    ui->s_pV->setValue(src[t[Pc::V]].valueReal());
    ui->s_pV->update();
    ui->s_lV->setText(src[t[Pc::V]]);
    ui->s_I->setChecked(src[t[Pc::X]]);
    ui->s_md->setChecked(src[t[Pc::MD]]);

    if(ui->s_AM->currentIndex())
    {
        ui->s_R_M->setCurrentIndex(src[t[Pc::Y]]!=0?1:0);
    }
}

void dlgPumpCtrl::slotSetSl(int v)
{
    double td=v;
//    qDebug() << sender()->objectName();

    if(sender()->objectName()=="s_SP_min")
    {
        src[t[Pc::Min]].setValue(v);
        ui->s_sSP_min->blockSignals(true);
        ui->s_sSP_min->setValue(td*src[t[Pc::Min]].k()+src[t[Pc::Min]].b());
        ui->s_sSP_min->blockSignals(false);
//        qDebug() << t[Pc::Min] << src.scaleZero(t[Pc::Min]) << src.scaleFull(t[Pc::Min]);

    }
    else
    {
        src[t[Pc::Max]].setValue(v);
        ui->s_sSP_max->blockSignals(true);
        ui->s_sSP_max->setValue(td*src[t[Pc::Min]].k()+src[t[Pc::Min]].b());
        ui->s_sSP_max->blockSignals(false);
    }
}

void dlgPumpCtrl::slotSetDs(double v)
{
    if(sender()->objectName()=="s_sSP_min")
    {
        src[t[Pc::Min]]=v;
        ui->s_SP_min->blockSignals(true);
        ui->s_SP_min->setValue(src[t[Pc::Min]].value32());
        ui->s_SP_min->blockSignals(false);
    }
    else
    {
        src[t[Pc::Max]]=v;
        ui->s_SP_max->blockSignals(true);
        ui->s_SP_max->setValue(src[t[Pc::Max]].value32());
        ui->s_SP_max->blockSignals(false);
    }
}


void dlgPumpCtrl::slotSetBool(int v)
{
    if(sender()->objectName()=="s_AM")
    {
        src[t[Pc::Am]]=qint16(-v);
        ui->s_R_M->blockSignals(v);
    }
    else
    {
        src[t[Pc::Y]]=qint16(-v);
    }

}


