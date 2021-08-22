
#include "apmnemo.h"
#include "ui_apmnemo.h"

#include <IoNetClient.h>

#include <QVBoxLayout>
#include <QTimer>

#include <QDebug>
#include <QLineEdit>
#include <QProgressBar>

#include "apone.h"
#include "trendchart.h"
#include "dlgapone.h"
#include "dlgtermctrl.h"

#include "dlgalarm.h"

ApMnemo::ApMnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::aPmnemo),s(src)
{
    m_ui->setupUi(this);



    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    QTimer *t=new QTimer(this);
    t->setInterval(10000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrend()));

    //connect(m_ui->bnCall_1,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));
    //connect(m_ui->bnCall_2,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));
    //connect(m_ui->bnCall_3,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));

    {
        ApOne *t=new ApOne(/**s[0],*/m_ui->ap_1);
        ap << t;
    }
    {
        ApOne *t=new ApOne(/**s[1],*/m_ui->ap_2);
        ap << t;
    }
    {
        ApOne *t=new ApOne(/**s[2],*/m_ui->ap_3);
        ap << t;
    }


    state
            << tr("Невизначено") //0
            << tr("Підготовка")  // 1
            << tr("Набір")  // 2
            << tr("Згущення")    // 2
            << tr("Затравка")    // 3
            << tr("Підкачка")    // 5
            << tr("Ріст")     // 6
            << tr("Біла патока") // 7

            << tr("Зелена патока")    // 8

            << tr("Уварювання")  // 9
            << tr("Готово")    // 10
            << tr("Вивантаження")  // 11
            << tr("Пропарка")    // 12

;



    trc << new TrendChart(m_ui->frame_1,14400,10) << new TrendChart(m_ui->frame_2,14400,10) << new TrendChart(m_ui->frame_3,14400,10);

    {
        QVBoxLayout *vbl=new QVBoxLayout(m_ui->frame_1);
        vbl->addWidget(trc[0]);
        m_ui->frame_1->setLayout(vbl);
    }
    {
        QVBoxLayout *vbl=new QVBoxLayout(m_ui->frame_2);
        vbl->addWidget(trc[1]);
        m_ui->frame_2->setLayout(vbl);
    }
    {
        QVBoxLayout *vbl=new QVBoxLayout(m_ui->frame_3);
        vbl->addWidget(trc[2]);
        m_ui->frame_3->setLayout(vbl);
    }


    // connect(this,SIGNAL(signalAlarm()),this,SLOT(slotAlarm()));
    foreach (QLineEdit *p, findChildren<QLineEdit*>(QRegExp("^s_V"))) {
        leOB << p;
    }
    foreach (QProgressBar *p, findChildren<QProgressBar*>(QRegExp("^s_pV"))) {
        pbOB << p;
    }


}

ApMnemo::~ApMnemo()
{
    delete m_ui;
}


void ApMnemo::updateDataRaw()
{
    for(int i=0;i<3;++i)
    {
        ap[i]->updateData(*s[i]);
    }

    int sta;
    sta=s[0]->getValue16("Kod");
    if(sta>-1 && sta<state.size())
        m_ui->le_State_1->setText(state[sta]);
    else
        m_ui->le_State_1->setText(tr("Невизначено"));

    sta=s[1]->getValue16("Kod");
    if(sta>-1 && sta<state.size())
        m_ui->le_State_2->setText(state[sta]);
    else
        m_ui->le_State_2->setText(tr("Невизначено"));

    sta=s[2]->getValue16("Kod");
    if(sta>-1 && sta<state.size())
        m_ui->le_State_3->setText(state[sta]);
    else
        m_ui->le_State_3->setText(tr("Невизначено"));
/*
    m_ui->s_pV_01->setValue(s[3]->getValueFloat("V_01"));
    m_ui->s_pV_02->setValue(s[3]->getValueFloat("V_02"));
    m_ui->s_pV_03->setValue(s[3]->getValueFloat("V_03"));

    m_ui->s_V_01->setText(QString("%1").arg(s[3]->getValueScaled("V_01"),3,'f',0));
    m_ui->s_V_02->setText(QString("%1").arg(s[3]->getValueScaled("V_02"),3,'f',0));
    m_ui->s_V_03->setText(QString("%1").arg(s[3]->getValueScaled("V_03"),3,'f',0));
*/
    foreach (QLineEdit *p, leOB) {
        p->setText(QString("%1").arg(s[3]->getValueFloat(p->objectName().right(5)),0,'f',0));
    }
    foreach (QProgressBar *p, pbOB) {
        double v=s[3]->getValueFloat(p->objectName().right(5));
        if(v<0.0) v=0.0;
        if(v>100.0) v=100.0;
      p->setValue(v);
    }

}

void ApMnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{

}

void ApMnemo::updateTrend()
{
    QVector<double> v;

    for(int i=0;i<3;++i)
    {
        v.clear();
        v
                << s[i]->getValueFloat("Lt1a_ind")*40.0
                << -s[i]->getValueFloat("Pt6a_ind")*40.0
                << s[i]->getValueFloat("Tt4a_ind")*40.0
                << s[i]->getValueFloat("Dt2a_ind")*40.0
                << s[i]->getValueFloat("Ey20a_ind")*40.0;
        trc[i]->addPoint(v);

    }
    //m_ui->labelSize->setText(QString("%1x%2").arg(size().width()).arg(size().height()));
}

void ApMnemo::slotCallDlgApOne()
{
    DlgApOne p(*s[sender()->objectName().right(1).toInt()-1],this);
    p.setWindowTitle(QString(tr("Управління апаратом №%1")).arg(sender()->objectName().right(1)));
    p.exec();
}

void ApMnemo::slotCallTermCtrl()
{
    DlgTermCtrl p(*s[3],sender()->objectName().right(1).toInt(),this);
    p.setWindowTitle(QString(tr("Збірник №%1")).arg(sender()->objectName().right(1)));
    p.exec();
}


void ApMnemo::slotAlarm()
{
    DlgAlarm p(this);
    p.exec();
}


