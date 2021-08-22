#include "cfparam1.h"
#include "ui_cfparam1.h"
#include <iodev.h>

CfParam1::CfParam1(IoDev &source, QWidget *parent) :
    QDialog(parent),
    src(source),
    m_ui(new Ui::CfParam1)
{
    m_ui->setupUi(this);

    foreach (QDoubleSpinBox *p, findChildren<QDoubleSpinBox*>()) {
       dsb_tag << p->objectName();
          dsb << p ;
    }

    foreach (QSpinBox *p,findChildren<QSpinBox*>())
    {
        sb_tag << p->objectName();
        sb << p;
    }



    int i=0;
    foreach(QString str,dsb_tag)
    {
        dsb[i++]->setValue(src.getValueFloat(str));
    }
    i=0;
    foreach(QString str,sb_tag)
    {
        sb[i++]->setValue(src.getValue16(str));
    }
    m_ui->Imp->setCurrentIndex(src.getValue16("Imp")-1);
    m_ui->Rej_sir->setChecked(src.getValue16("rej_sir"));
    m_ui->Rej_fug->setCurrentIndex(src.getValue16("rej_fug")?1:0);
//    m_ui->rej_Tvod->setCurrentIndex(src.getValue16("rej_tvod")?1:0);

    connect(m_ui->buttonBox,SIGNAL(accepted()),this,SLOT(slotAccept()));
//    connect(m_ui->zeroNc,SIGNAL(clicked()),this,SLOT(setZeroNc()));
}

CfParam1::~CfParam1()
{
    delete m_ui;
}

void CfParam1::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CfParam1::slotAccept()
{
    int i=0;
    foreach(QString str,dsb_tag)
        src.sendValue(str,dsb[i++]->value());

    i=0;
    foreach(QString str,sb_tag)
        src.sendValue(str,qint16(sb[i++]->value()));

    src.sendValue("Imp",qint16(m_ui->Imp->currentIndex()+1));
    src.sendValue("rej_sir",qint16(m_ui->Rej_sir->checkState()?-1:0));
    src.sendValue("rej_fug",qint16(m_ui->Rej_fug->currentIndex()?-1:0));
    // src.sendValue("rej_tvod",qint16(m_ui->rej_Tvod->currentIndex()?-1:0));
    accept();
}


void CfParam1::setZeroNc()
{
    src.sendValue("Nc",qint32(0));
}
