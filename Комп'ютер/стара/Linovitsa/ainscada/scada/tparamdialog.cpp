#include "tparamdialog.h"
#include "ui_tparamdialog.h"
#include <iodev.h>
#include <QTime>


TParamDialog::TParamDialog(IoDev &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TParamDialog),
    s(src)
{
    m_ui->setupUi(this);

    m_ui->bx_Nc_zd->setCurrentIndex(s.getValue16("Nc_zd"));

    m_ui->sb_Q_zd->setValue(s.getValueScaled("Q_zd"));

    m_ui->sp_Tper_zd->setTime(QTime().addSecs((s.getValue32("Tper_zd"))));
    m_ui->sp_Tcyc_zd->setTime(QTime().addSecs((s.getValue32("Tcyc_zd"))));

    m_ui->sp_Tzatr_zd->setTime(QTime().addMSecs(s.getValue32("Tzatr_zd")));
    m_ui->sp_Tskyd_zd->setTime(QTime().addMSecs(s.getValue32("Tskyd_zd")));
    m_ui->sp_Tprom_zd->setTime(QTime().addMSecs(s.getValue32("Tprom_zd")));
    m_ui->sp_Tvids_zd->setTime(QTime().addMSecs(s.getValue32("Tvids_zd")));


    m_ui->sb_Tst_zd->setTime(QTime().addSecs(s.getValue32("Tst_zd")));
    m_ui->sb_d_L_sus_zd->setValue(s.getValueFloat("d_L_sus_zd"));

    m_ui->bx_Tmode->setCurrentIndex(s.getValue16("Tmode")?1:0);

    m_ui->Taval_1->setValue(s.getValue32("Taval_1")/1000);
    m_ui->Taval_2->setValue(s.getValue32("Taval_2")/1000);
    m_ui->Taval_3->setValue(s.getValue32("Taval_3")/1000);
    m_ui->Taval_4->setValue(s.getValue32("Taval_4")/1000);
    m_ui->Taval_5->setValue(s.getValue32("Taval_5")/1000);
    m_ui->Taval_6->setValue(s.getValue32("Taval_6")/1000);


    connect(m_ui->bn_Ok,SIGNAL(clicked()),this,SLOT(myAccept()));



}

TParamDialog::~TParamDialog()
{
    delete m_ui;
}

void TParamDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TParamDialog::myAccept() // реакція на кнопку Ок
{
    s.sendValue("Tzatr_zd",qint32(QTime().msecsTo( m_ui->sp_Tzatr_zd->time())));
    s.sendValue("Tskyd_zd",qint32(QTime().msecsTo( m_ui->sp_Tskyd_zd->time())));
    s.sendValue("Tprom_zd",qint32(QTime().msecsTo( m_ui->sp_Tprom_zd->time())));
    s.sendValue("Tvids_zd",qint32(QTime().msecsTo( m_ui->sp_Tvids_zd->time())));


    s.sendValue("Tper_zd",qint32(QTime().secsTo(m_ui->sp_Tper_zd->time())));
    s.sendValue("Tcyc_zd",qint32(QTime().secsTo(m_ui->sp_Tcyc_zd->time())));

    s.sendValue("Tst_zd",qint32(QTime().secsTo(m_ui->sb_Tst_zd->time())));

    s.sendValueScaled("Q_zd",m_ui->sb_Q_zd->value());

    s.sendValue("Nc_zd",qint16(m_ui->bx_Nc_zd->currentIndex()));

    s.sendValue("Tmode",qint16(m_ui->bx_Tmode->currentIndex()?-1:0));

    s.sendValue("d_L_sus_zd",(double)m_ui->sb_d_L_sus_zd->value());

    s.sendValue("Taval_1",m_ui->Taval_1->value());
    s.sendValue("Taval_2",m_ui->Taval_2->value());
    s.sendValue("Taval_3",m_ui->Taval_3->value());
    s.sendValue("Taval_4",m_ui->Taval_4->value());
    s.sendValue("Taval_5",m_ui->Taval_5->value());
    s.sendValue("Taval_6",m_ui->Taval_6->value());

    // смикнут контролер
    //s.sendValue("Run",qint16(0));
    s.sendValue("Save",qint16(-1));

     qDebug() << s.getValue16("Tst_zd") << m_ui->sb_Tst_zd->time();
    accept();
}
