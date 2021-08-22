#include "apcommon.h"
#include "ui_apcommon.h"
#include <aulib.h>

#include <QRegExp>
#include <QStyleOption>
#include <QPainter>
#include "tmaaparat/apone.h"
#include <QDebug>
#include <QFile>

#include "apsgkl.h"
#include <QTimer>

#define TAP1 QStringLiteral("tmaap1")
#define TAP2 QStringLiteral("tmaap2")
#define TAP3 QStringLiteral("tmaap3")
#define TAPA QStringLiteral("tmaob")

ApCommon::ApCommon(AuBase &base, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApCommon),
    src(base),
    srcBM((new BitMask(base[TAPA],this))[0]),
    myOff(true),
    count(0)
{
    ui->setupUi(this);
//------------------------------------------------------------
// цей код змінить базу шкали для вакуум-апаратів.
/*
    QStringList plcs;
    plcs << TAPA
         << TAP1
         << TAP2
         << TAP3;
    foreach(QString plcName,plcs)
        foreach(auto tag,src[plcName].tags())
            tag->setBaseScale(100.0);
*/
//-------------------------------------------------------------
    // завантажити списки джина !sg_kl
    QFile fList(":/text/sgkl.list");
    fList.open(QIODevice::ReadOnly);
    while(!fList.atEnd())
    {
        QString line=fList.readLine().trimmed();
        line=line.split("//")[0].trimmed();
        if(line.size()>0) // перевірити чи вийщов не пустий рядок
        {
            QStringList plc=line.split(QRegExp("\\s+"));
            QString tagList=line.split('\"').at(1);
            if(plc[0]=="tmaob")
                foreach(QString s,tagList.split(','))
                    sgKlObList[plc[1]]<<s.trimmed();
            else
                foreach(QString s,tagList.split(','))
                    sgKlApList[plc[1]]<<s.trimmed();
        }
    }
    // qDebug() << sgKlObList << sgKlApList;
    fList.close();
//----------------------------------------------------------------

    le=findChildren<QLineEdit*>();    
    pb=findChildren<QProgressBar*>();
    lamp=findChildren<AuLamp*>();

    foreach(QPushButton *bn,findChildren<QPushButton*>(QRegExp("bnApSg_*")))
    {
        connect(bn,&QPushButton::clicked,this,&ApCommon::slotSgKl);
        // тут би ще немішало зберегти його для подальшої індикації
        bnLamp << bn;
    }
    // qDebug() << bnLamp;

    connect(ui->reg_1,&QPushButton::clicked,this,&ApCommon::slotCallReg);
    connect(ui->reg_2,&QPushButton::clicked,this,&ApCommon::slotCallReg);


    ap1
     << new ApOne(src[TAP1],ui->frameAp1)
     << new ApOne(src[TAP2],ui->frameAp2)
     << new ApOne(src[TAP3],ui->frameAp3);
    ap1[0]->setObjectName("AP1");
    ap1[1]->setObjectName("AP2");
    ap1[2]->setObjectName("AP3");

    ap1[0]->setText("1");
    ap1[1]->setText("2");
    ap1[2]->setText("3");

    connect(&src[TAPA],&AuPlc::updateData,this,&ApCommon::slotUpdate);

    QTimer *blinkT=new QTimer(this);
    blinkT->setObjectName("blink");
    blinkT->setTimerType(Qt::PreciseTimer);
    blinkT->setInterval(500);
    connect(blinkT,&QTimer::timeout,this,&ApCommon::blink);
    blinkT->start();

    connect(this,&ApCommon::blinkOff,ap1[0],&ApOne::blink);
    connect(this,&ApCommon::blinkOff,ap1[1],&ApOne::blink);
    connect(this,&ApCommon::blinkOff,ap1[2],&ApOne::blink);


    blinkLamps << ui->K5f_in;


}

ApCommon::~ApCommon()
{
    delete ui;
}

void ApCommon::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ApCommon::slotUpdate()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    QString tag;
    foreach (QLineEdit *p , le) {
        SAFE(p->setText(s[p->objectName()]));
        // qDebug() << p->objectName() << s[p->objectName()].valueReal();
    }
    foreach(AuLamp *p,lamp)
    {
        SAFE(p->setChecked(s[p->objectName()]!=0));
    }
    foreach(QProgressBar *p,pb)
    {
        tag=p->objectName();
        tag.remove(0,1);
        SAFE(p->setValue(s[tag].valueReal()));
        p->update();
    }

    foreach(QPushButton *p,bnLamp)
    {
        QString bnName=p->objectName().split('_')[1];
        int v=0;
        SAFE(v=(srcBM[sgKlObList[bnName][0]]?2:0) + (srcBM[sgKlObList[bnName][1]]?1:0));
        p->setIcon(QIcon(QStringLiteral(":/images/%1_gval.svg").arg(v)));
    }

    ui->va1_msg1->setText(QString("До кінця стадії %1 сек.").arg(src[TAP1]["Indication_stad"].value()));
    ui->va1_msg2->setText(QString("Варка триває вже %1 год. %2 хв.")
                          .arg(src[TAP1]["Dication_hour"].value()) //
                          .arg(src[TAP1]["Indication_min"].value()));//


    ui->va2_msg1->setText(QString("До кінця стадії %1 сек.").arg(src[TAP2]["Indication_stad"].value()));
    ui->va2_msg2->setText(QString("Варка триває вже %1 год. %2 хв.")
                          .arg(src[TAP2]["Dication_hour"].value()) //
                          .arg(src[TAP2]["Indication_min"].value()));//

    ui->va3_msg1->setText(QString("До кінця стадії %1 сек.").arg(src[TAP3]["Indication_stad"].value()));
    ui->va3_msg2->setText(QString("Варка триває вже %1 год. %2 хв.")
                          .arg(src[TAP3]["Dication_hour"].value()) //
                          .arg(src[TAP3]["Indication_min"].value()));//


}

void ApCommon::slotCallReg()
{
        AuPanelReg p(src,src[TAPA].index(),sender()->objectName().split('_')[1].toInt()-1,":/text/reg_ob.txt",this);
        p.exec();
}

void ApCommon::slotSgKl()
{
    QStringList tags;
    ApSgKl p(src[TAPA],sgKlObList[sender()->objectName().split('_').at(1)],this);
    p.exec();
}

void ApCommon::blink()
{
    myOff ^= true; // переключити

    SAFE(
    if(src[TAPA][QStringLiteral("K5f_in")]!=0) // якщо треба блінкати
    {
        ui->K5f_in->setLampOn(myOff?QStringLiteral(":/images/redLamp.png"):QStringLiteral(":/images/blackLamp.png"));
        ui->label_14->setText(QStringLiteral("Низький тиск повітря в магістралі!"));
        ui->label_14->setStyleSheet(myOff?QStringLiteral("color: red"):QStringLiteral("color: black"));

        // ui->K5f_in->repaint();

    }
    else
    {
        ui->label_14->setText(QStringLiteral("Тиск повітря в нормі"));
        ui->label_14->setStyleSheet(QStringLiteral("color:green"));
    }
    )  // SAFE
    SAFE(
    if(src[TAPA][QStringLiteral("K6f_in")]==0)
    {
        ui->K6f_in->setLampOff(myOff?QStringLiteral(":/images/redRect.png"):QStringLiteral(":/images/blackRect.png"));
    }
    )


    emit blinkOff(myOff);


}

