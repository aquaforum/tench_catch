#include "dlgdidisplay.h"
#include "ui_dlgdidisplay.h"

#include <QCheckBox>
#include "aulib.h"
#include <QTimer>


dlgDiDisplay::dlgDiDisplay(AuPlc &source,QWidget *parent) :
    QDialog(parent),src(source),
    ui(new Ui::dlgDiDisplay)
{
    ui->setupUi(this);

    foreach (QCheckBox *p, findChildren<QCheckBox*>()) {
        cb<< p;
    }

    QTimer::singleShot(1000,this,SLOT(updateToolTip()));

    QTimer *tmr=new QTimer(this);
    tmr->setInterval(1000);
    tmr->start();
    connect(tmr,SIGNAL(timeout()),this,SLOT(updateDataRaw()));

}

dlgDiDisplay::~dlgDiDisplay()
{
    delete ui;
}


void dlgDiDisplay::updateToolTip()
{
    /*
    foreach (QCheckBox *p, cb) {
        //if(src.getTags().contains(p->objectName()))
        //    p->setText(src.getText()[p->objectName()]);

    }
    */
}


void dlgDiDisplay::updateDataRaw()
{
    /*
    foreach (QCheckBox *p, cb) {
        //p->setChecked(src.getValue16(p->objectName())==0?Qt::Unchecked:Qt::Checked);
    }
    */

}
