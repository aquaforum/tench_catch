#include "dlgvbctrl.h"
#include "ui_dlgvbctrl.h"

#include "aulib.h"
#include <QTimer>

dlgVbCtrl::dlgVbCtrl(AuPlc &source , int nValve , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgVbCtrl),
    src(source),
    nI(nValve)
{
    ui->setupUi(this);
    SAFE(ui->en_p->setCurrentIndex(src[QStringLiteral("en_p_%1").arg(nI,2,10,QChar('0'))]!=0?1:0));

    connect(&src,&AuPlc::updateData,this,&dlgVbCtrl::slotUpdate);

    connect(ui->en_p,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSet(int)));
    connect(ui->Q,SIGNAL(clicked()),this,SLOT(slotSet()));


}

dlgVbCtrl::~dlgVbCtrl()
{
    delete ui;
}

void dlgVbCtrl::changeEvent(QEvent *e)
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

void dlgVbCtrl::slotUpdate()
{
    SAFE(
    ui->Q->setIcon(QIcon(
            QPixmap(src[QStringLiteral("X_%1").arg(nI,2,10,QChar('0'))]!=0
                                  ?":/butons/pict/klapan_diskrette_vert_on_na_25x26.png"
                                      :":/butons/pict/klapan_diskrette_vert_off_na_25x46.png")));
    )

    SAFE(ui->I->setChecked(src[QString("Y_%1").arg(nI,2,10,QChar('0'))]!=0));

}

void dlgVbCtrl::slotSet(int v)
{
    SAFE(src[QString("en_p_%1").arg(nI,2,10,QChar('0'))]=qint16(-v));
    ui->Q->blockSignals(v!=0);
}

void dlgVbCtrl::slotSet()
{
    qint16 v = 0;
    SAFE(
    v=src[QString("Y_%1").arg(nI,2,10,QChar('0'))]!=0?0:-1;
    src[QString("Y_%1").arg(nI,2,10,QChar('0'))]=v;
    )
}
