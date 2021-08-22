
#include "mnemo.h"
#include "ui_mnemo.h"

#include <IoNetClient.h>

#include <QVBoxLayout>
#include <QTimer>

#include <apone.h>
#include "trendchart.h"
#include "dlgapone.h"
#include "dlgtermctrl.h"

#include "dlgalarm.h"

Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);



    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    QTimer *t=new QTimer(this);
    t->setInterval(10000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrend()));

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

    connect(m_ui->bnCall_1,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));
    connect(m_ui->bnCall_2,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));
    connect(m_ui->bnCall_3,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));

    state
            << tr("Невизначено") //0
            << tr("Очікування")  // 1
            << tr("Очікування")  // 2
            << tr("Набір")       // 3
            << tr("Згущення")    // 4
            << tr("Затравка")    // 5
            << tr("Згущення")    // 6
            << tr("Поличка")     // 7
            << tr("Розкачака 1") // 8
            << tr("Згущення")    // 9
            << tr("Розкачка 2")  // 10
            << tr("Згущення")    // 11
            << tr("Розкачка 3")  // 12
            << tr("Згущення")    // 13

            << tr("Розкачака 4") // 14
            << tr("Згущення")    // 15
            << tr("Розкачка 5")  // 16
            << tr("Згущення")    // 17
            << tr("Розкачка 6")  // 18
            << tr("Згущення")    // 19

            << tr("Ріст 1")      // 20
            << tr("Ріст 2")      // 21
            << tr("Ріст 3")      // 22
            << tr("Уварювання")  // 23
            << tr("Вивантаження") // 24
    << tr("Розкачака 1") // 25
    << tr("Згущення")    // 26
    << tr("Розкачка 2")  // 27
    << tr("Згущення")    // 28
    << tr("Розкачка 3")  // 29
    << tr("Згущення") ;   // 30



    trc << new TrendChart(m_ui->frame_1,7200,10) << new TrendChart(m_ui->frame_2,7200,10) << new TrendChart(m_ui->frame_3,7200,10);

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


    connect(this,SIGNAL(signalAlarm()),this,SLOT(slotAlarm()));

}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateDataRaw()
{
    for(int i=0;i<3;++i)
    {
        ap[i]->updateData(*s[i]);
    }
    int sta;
    sta=s[0]->getValue16("State");
    if(sta>-1 && sta<31)
        m_ui->le_State_1->setText(state[sta]);
    else
        m_ui->le_State_1->setText(tr("Невизначено"));

    sta=s[1]->getValue16("State");
    if(sta>-1 && sta<31)
        m_ui->le_State_2->setText(state[sta]);
    else
        m_ui->le_State_2->setText(tr("Невизначено"));

    sta=s[2]->getValue16("State");
    if(sta>-1 && sta<31)
        m_ui->le_State_3->setText(state[sta]);
    else
        m_ui->le_State_3->setText(tr("Невизначено"));

    m_ui->s_pV_01->setValue(s[3]->getValueFloat("V_01"));
    m_ui->s_pV_02->setValue(s[3]->getValueFloat("V_02"));
    m_ui->s_pV_03->setValue(s[3]->getValueFloat("V_03"));

    m_ui->s_V_01->setText(QString("%1").arg(s[3]->getValueScaled("V_01"),3,'f',0));
    m_ui->s_V_02->setText(QString("%1").arg(s[3]->getValueScaled("V_02"),3,'f',0));
    m_ui->s_V_03->setText(QString("%1").arg(s[3]->getValueScaled("V_03"),3,'f',0));

}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{

}

void Mnemo::updateTrend()
{
    QVector<double> v;

    for(int i=0;i<3;++i)
    {
        v.clear();
        v
                << s[i]->getValueFloat("V_l")
                << s[i]->getValueFloat("V_vak")
                << s[i]->getValueFloat("V_temp")
                << s[i]->getValueFloat("V_r")
                << s[i]->getValueFloat("V_m");
        trc[i]->addPoint(v);

    }
    //m_ui->labelSize->setText(QString("%1x%2").arg(size().width()).arg(size().height()));
}

void Mnemo::slotCallDlgApOne()
{
    DlgApOne p(*s[sender()->objectName().right(1).toInt()-1],this);
    p.setWindowTitle(QString(tr("Управління апаратом №%1")).arg(sender()->objectName().right(1)));
    p.exec();
}

void Mnemo::slotCallTermCtrl()
{
    DlgTermCtrl p(*s[3],sender()->objectName().right(1).toInt(),this);
    p.setWindowTitle(QString(tr("Збірник №%1")).arg(sender()->objectName().right(1)));
    p.exec();
}


void Mnemo::slotAlarm()
{
    DlgAlarm p(this);
    p.exec();
}


