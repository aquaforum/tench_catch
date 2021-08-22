
#include "apmnemo.h"
#include "ui_apmnemo.h"

#include <aulib.h>
#include <trendchar/trendchart.h>

#include <QVBoxLayout>
#include <QTimer>

#include <QDebug>
#include <QLineEdit>
#include <QProgressBar>

#include "apone.h"
#include "dlgapone.h"
#include "dlgtermctrl.h"

#include "dlgalarm.h"

#define TAP1 QStringLiteral("tmaap1")
#define TAP2 QStringLiteral("tmaap2")
#define TAP3 QStringLiteral("tmaap3")
#define TAPA QStringLiteral("tmaob")

ApMnemo::ApMnemo(AuBase &src, QWidget *p) : QLabel(p), m_ui(new Ui::aPmnemo),s(src)
{
    setObjectName("ApMnemo");

    m_ui->setupUi(this);

    //connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    //connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    connect(&src[TAPA],&AuPlc::updateData,this,&ApMnemo::updateData);

    QTimer *t=new QTimer(this);
    t->setInterval(10000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrend()));

    apIx[0]=src[TAP1].index();
    apIx[1]=src[TAP2].index();
    apIx[2]=src[TAP3].index();
    apIx[3]=src[TAPA].index();

    //connect(m_ui->bnCall_1,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));
    //connect(m_ui->bnCall_2,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));
    //connect(m_ui->bnCall_3,SIGNAL(clicked()),this,SLOT(slotCallDlgApOne()));

    {
        ApOne *t=new ApOne(src[TAP1],m_ui->ap_1);
        ap << t;
    }
    {
        ApOne *t=new ApOne(src[TAP2],m_ui->ap_2);
        ap << t;
    }
    {
        ApOne *t=new ApOne(src[TAP3],m_ui->ap_3);
        ap << t;
    }

    leList[&src[TAP1]]=m_ui->le_State_1;
    leList[&src[TAP2]]=m_ui->le_State_2;
    leList[&src[TAP3]]=m_ui->le_State_3;

    connect(&src[TAP1],&AuPlc::updateData,this,&ApMnemo::updateState);
    connect(&src[TAP2],&AuPlc::updateData,this,&ApMnemo::updateState);
    connect(&src[TAP3],&AuPlc::updateData,this,&ApMnemo::updateState);

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

void ApMnemo::updateState()
{
    AuPlc &sA=*qobject_cast<AuPlc*>(sender());
    QLineEdit *le=leList[sender()];

    int sta=0;
    SAFE(sta=sA[QStringLiteral("Kod")]);
    if(sta>-1 && sta<state.size())
        le->setText(state[sta]);
    else
        le->setText(tr("Невизначено"));

}

void ApMnemo::updateData()
{
    QString tag;
    AuPlc &sA=*qobject_cast<AuPlc*>(sender());

    foreach (QLineEdit *p, leOB) {
        tag=p->objectName().right(5);
        SAFE(p->setText(sA[tag]));
    }
    foreach (QProgressBar *p, pbOB) {
        tag=p->objectName().right(5);
        SAFE(p->setValue(sA[tag].value()));
    }
}

void ApMnemo::updateTrend()
{
    QVector<double> v;

    for(int i=0;i<3;++i)
    {
        v.clear();
        SAFE(
        v
                << (double)s[apIx[i]][QStringLiteral("Lt1a_ind")]*40.0
                << -(double)s[apIx[i]][QStringLiteral("Pt6a_ind")]*40.0
                << (double)s[apIx[i]][QStringLiteral("Tt4a_ind")]*40.0
                << (double)s[apIx[i]][QStringLiteral("Dt2a_ind")]*40.0
                << (double)s[apIx[i]][QStringLiteral("Ey20a_ind")]*40.0;
        )
        trc[i]->addPoint(v);

    }
    //m_ui->labelSize->setText(QString("%1x%2").arg(size().width()).arg(size().height()));
}

void ApMnemo::slotCallDlgApOne()
{
    /*
    DlgApOne p(*s[sender()->objectName().right(1).toInt()-1],this);
    p.setWindowTitle(QString(tr("Управління апаратом №%1")).arg(sender()->objectName().right(1)));
    p.exec();
    */
}

void ApMnemo::slotCallTermCtrl()
{
    /*
    DlgTermCtrl p(*s[3],sender()->objectName().right(1).toInt(),this);
    p.setWindowTitle(QString(tr("Збірник №%1")).arg(sender()->objectName().right(1)));
    p.exec();
    */
}


void ApMnemo::slotAlarm()
{
    DlgAlarm p(this);
    p.exec();
}


