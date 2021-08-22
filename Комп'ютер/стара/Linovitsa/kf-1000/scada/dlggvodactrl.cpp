#include "dlggvodactrl.h"
#include "ui_dlggvodactrl.h"

#include <QTimer>

#include "iodev.h"

dlgGVodaCtrl::dlgGVodaCtrl(IoDev &source,QWidget *parent) :
    QDialog(parent),
    src(source),
    ui(new Ui::dlgGVodaCtrl)
{
    ui->setupUi(this);


    foreach(QDoubleSpinBox *sb,findChildren<QDoubleSpinBox*>())
    {
        sb->setValue(src.getValueScaled(sb->objectName().right(sb->objectName().size()-3)));
        connect(sb,SIGNAL(valueChanged(double)),this,SLOT(slotSetData(double)));
    }


    ui->sl_MSP_12->setValue(src.getValueFloat("MSP_12"));
    ui->sl_XSP_13->setValue(src.getValueFloat("XSP_13"));
    connect(ui->sl_MSP_12,SIGNAL(valueChanged(int)),this,SLOT(slotSetData(int)));
    connect(ui->sl_XSP_13,SIGNAL(valueChanged(int)),this,SLOT(slotSetData(int)));

    ui->cb_AM_12->setCurrentIndex(src.getValue16("AM_12")?1:0);
    ui->cb_AM_13->setCurrentIndex(src.getValue16("AM_13")?1:0);
    connect(ui->cb_AM_12,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetData(int)));
    connect(ui->cb_AM_13,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetData(int)));

    ui->cb_Y_14->setChecked(src.getValue16("Y_14"));
    ui->cb_Y_15->setChecked(src.getValue16("Y_15"));

    connect(ui->cb_Y_14,SIGNAL(clicked(bool)),this,SLOT(slotSetData(bool)));
    connect(ui->cb_Y_15,SIGNAL(clicked(bool)),this,SLOT(slotSetData(bool)));


    QTimer *t=new QTimer(this)    ;
    t->setInterval(1000);
    t->start();
        connect(t,SIGNAL(timeout()),this,SLOT(slotUpdateData()));

    // заблокувати в автомати сигнали від клапанів
    if(ui->cb_AM_12->currentIndex())
        ui->cb_Y_14->blockSignals(true);
    if(ui->cb_AM_13->currentIndex())
        ui->cb_Y_15->blockSignals(true);

}

dlgGVodaCtrl::~dlgGVodaCtrl()
{
    delete ui;
}

void dlgGVodaCtrl::changeEvent(QEvent *e)
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


void dlgGVodaCtrl::slotSetData(bool v)
{
    src.sendValue(sender()->objectName().right(sender()->objectName().size()-3),qint16(v?-1:0));
}

void dlgGVodaCtrl::slotSetData(int v )
{
    if(sender()->objectName()=="cb_AM_12") // відправити температуру води
    {
        src.sendValue("AM_12",qint16(-v));
        ui->cb_Y_14->blockSignals(v);
    }
    else if(sender()->objectName()=="cb_AM_13") // відправити температуру води
    {
        src.sendValue("AM_13",qint16(-v));
        ui->cb_Y_15->blockSignals(v);
    }
    else
    {
        src.sendValue(sender()->objectName().right(sender()->objectName().size()-3),(double)v);
            if(sender()->objectName()=="sl_MSP_12")
            {
                ui->sb_MSP_12->blockSignals(true);
                ui->sb_MSP_12->setValue((double)v/4000.0*(src.scaleFull("MSP_12")-src.scaleZero("MSP_12"))+src.scaleZero("MSP_12"));
                ui->sb_MSP_12->blockSignals(false);
            }
            if(sender()->objectName()=="sl_XSP_13")
            {
                ui->sb_XSP_13->blockSignals(true);
                ui->sb_XSP_13->setValue((double)v/4000.0*(src.scaleFull("XSP_13")-src.scaleZero("XSP_13"))+src.scaleZero("XSP_13"));
                ui->sb_XSP_13->blockSignals(false);
            }
    }
}

void dlgGVodaCtrl::slotSetData(double v)
{
    src.sendValueScaled(sender()->objectName().right(sender()->objectName().size()-3),v);

    if(sender()->objectName()=="sb_MSP_12")
        {
            ui->sl_MSP_12->blockSignals(true);
            ui->sl_MSP_12->setValue((v-src.scaleZero("MSP_12"))/(src.scaleFull("MSP_12")-src.scaleZero("MSP_12"))*4000.0);
            ui->sl_MSP_12->blockSignals(false);
        }
    if(sender()->objectName()=="sb_XSP_13")
        {
            ui->sl_XSP_13->blockSignals(true);
            ui->sl_XSP_13->setValue((v-src.scaleZero("XSP_13"))/(src.scaleFull("XSP_13")-src.scaleZero("XSP_13"))*4000.0);
            ui->sl_XSP_13->blockSignals(false);
        }
}


void dlgGVodaCtrl::slotUpdateData()
{
    ui->pb_Lp->setValue(src.getValueFloat("Lp"));
    ui->le_Lp->setText(QString("%1").arg(src.getValueScaled("Lp"),3,'f',0));
    ui->le_V_12->setText(QString("%1").arg(src.getValueScaled("V_12"),3,'f',0));
    ui->le_V_13->setText(QString("%1").arg(src.getValueScaled("V_13"),3,'f',0));

    ui->cb_X_14->setChecked(src.getValue16("Y_14"));
    ui->cb_X_15->setChecked(src.getValue16("Y_15"));

    if(ui->cb_AM_12->currentIndex())
        ui->cb_Y_14->setChecked(src.getValue16("Y_14"));

    if(ui->cb_AM_13->currentIndex())
        ui->cb_Y_15->setChecked(src.getValue16("Y_15"));

}


