#include "apsingle.h"
#include "ui_apsingle.h"
#include <QStyleOption>
#include <QPainter>
#include <QFile>

#include <aulib.h>
#include <QRegularExpression>
#include <QTimer>
#include <QDateTime>

#include "tmaaparat/apsgkl.h"
#include "tmaaparat/apwincurkyl.h"
#include "trendchar/trendchart.h"

#define TAP1 QStringLiteral("tmaap1")
#define TAP2 QStringLiteral("tmaap2")
#define TAP3 QStringLiteral("tmaap3")
#define TAPA QStringLiteral("tmaob")

ApSingle::ApSingle(AuPlc &plc, AuBase &base, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApSingle),
    src(plc),
    myBase(base),
    srcOB(base[TAPA]),
    srcBM((new BitMask(plc,this))[0])

{
    ui->setupUi(this);
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
                QStringList plc=line.split(QRegularExpression("\\s+"));
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


    // розбираю лампочки
    foreach (AuLamp *p, findChildren<AuLamp*>()) {
        if(srcBM.containts(p->objectName()))
        {
            lamp << p;
        }

#ifndef QT_NO_DEBUG
        else
        {
            qDebug() << "AuLamp, tag not found: " << p->objectName();
        }
#endif

    }


    // розбираю лайнедіти
    foreach(QLineEdit *p,findChildren<QLineEdit*>())
    {
        if(src.tagsList().contains(p->objectName()))
            apLe << p;
        else if (srcOB.tagsList().contains(p->objectName()))
            obLe << p;
#ifndef QT_NO_DEBUG
        else
            qDebug() << "QLineEdit: tag not found " << p->objectName();
#endif

    }

    // apLcd=findChildren<QLCDNumber*>();
    QStringList ind;
    ind << "Lt1a_ind"
            << "Uroven_ind"
            << "Tt4a_ind"
            << "Termo_ind"
            << "Pt6a_ind"
            << "Vakuum_ind"
            << "Ey20a_ind"
            << "Moment_ind"
            << "Sy20a_ind"
            << "Ha20a_zad";


    foreach(QLabel *p, findChildren<QLabel*>())
    {
        if(ind.contains(p->objectName()) )
            lcdS << p;
    }

    ind.clear();
    ind
            << "Delta"
            << "Delta_ind"
            << "Dt2a_ind"
            << "Cv_ind";

    foreach(QLabel *p, findChildren<QLabel*>())
    {
        if(ind.contains(p->objectName()) )
            le2 << p;
    }



    // кнопки
    foreach(QPushButton *p,findChildren<QPushButton*>(QRegularExpression("bnApSg_*")))
    {
        QString bnName=p->objectName().split('_')[1];
        // qDebug() << bnName;
        if(sgKlApList.keys().contains(bnName))
        {
            connect(p,&QPushButton::clicked,this,&ApSingle::slotSgKl);
            if(p->objectName()!="bnApSg_19_vval") // вийняти цю кнопку із списку на оновлення індикаторів
                bnLamp << p;
        }
#ifndef QT_NO_DEBUG
        else
        {
            qDebug() << "QPushButton tag not found "<< p->objectName();
        }
#endif

    }
    connect(ui->bnCyrkul,&QPushButton::clicked,this,&ApSingle::slotCallCurkyl);

    bnReg << ui->bnReg_4
          << ui->bnReg_6
          << ui->bnReg_17;
    foreach(QPushButton *p,bnReg)
    {
        connect(p,&QPushButton::clicked,this,&ApSingle::slotCallReg);
    }

    // кнопки управління процесом в лівій частині вікна
    connect(ui->Kod_up,&QPushButton::clicked,this,&ApSingle::slotControl);
    connect(ui->Kod_dw,&QPushButton::clicked,this,&ApSingle::slotControl);
    connect(ui->Kod_blok,&QPushButton::clicked,this,&ApSingle::slotControl);
    connect(ui->Stepp,&QPushButton::clicked,this,&ApSingle::slotControl);
    connect(ui->Rezi,&QPushButton::clicked,this,&ApSingle::slotControl);
    connect(ui->Water_key,&QPushButton::clicked,this,&ApSingle::slotControl);


    connect(&src,&AuPlc::updateData,this,&ApSingle::slotUpdate);
    connect(&srcOB,&AuPlc::updateData,this,&ApSingle::slotUpdateOb);

    // дані масивів kodState та kodColor
    #include "kod_data.h"

    blinkLamp=findChildren<AuLamp*>(QRegularExpression("A_k*"));
    blinkLamp << ui->M1_a_in;

    //qDebug() << blinkLamp;

    QTimer *blinkT=new QTimer(this);
    blinkT->setObjectName("blink");
    blinkT->setTimerType(Qt::PreciseTimer);
    blinkT->setInterval(500);
    connect(blinkT,&QTimer::timeout,this,&ApSingle::blink);
    blinkT->start();

    ui->label->setText(plc.name().right(1));


    trc=new TrendChart(this,3600*4);
    QFrame *p=ui->frameTrend;
    QVBoxLayout *vbl = new QVBoxLayout(p);
    vbl->addWidget(trc);
    vbl->setMargin(2);
    p->setLayout(vbl);
    QVector<QColor> clr;
    clr << QColor(0x00f3ff)
        << QColor(Qt::darkBlue)
        << QColor(Qt::yellow)
        << QColor(Qt::white)
        << QColor(Qt::blue)
        << QColor(Qt::green)
        << QColor(Qt::white)
        << QColor(Qt::yellow);

    trc->setColor(clr);

    QTimer *tmr=new QTimer(this);
    connect(tmr,&QTimer::timeout,this,&ApSingle::updateTrend);
    tmr->setInterval(10000);
    tmr->start();

    // тут ще потрібно завантажити дані для trc
    qint32 stop=QDateTime::currentDateTime().toTime_t();

    connect(&myBase,&AuBase::htrData,this,&ApSingle::slotHtrData);
    myBase.queryHtrData(src.name(),QStringLiteral("Lt1a_ind*40.0,Pt6a_ind*-40.0,(Dt2a_ind-60.0)*100.0,Ey20a_ind*40.0,St_i,St_i1,St_i2"),stop-3600*4,stop);
    connect(&myBase,&AuBase::sqlError,this,&ApSingle::sqlError);
    // qDebug() << src.name();


}

ApSingle::~ApSingle()
{
    delete ui;
}

void ApSingle::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void ApSingle::updateTrend()
{
    QVector<double> v;
    v.fill(0.0,8);

    v[0]=src["Lt1a_ind"]*40.0;
    v[1]=src["Pt6a_ind"]*-40.0;
    v[2]=(src["Dt2a_ind"]-60.0)*100.0;
    v[3]=src["Ey20a_ind"]*40.0;
    v[4]=srcBM["K10c_in"]?3000.0:3200.0;
    v[5]=srcBM["K9c_in"]?3250.0:3450.0;
    v[6]=srcBM["K8c_in"]?3500.0:3700.0;
    v[7]=srcBM["K7c_in"]?3750.0:3950.0;

    trc->addPoint(v);

}

void ApSingle::slotHtrData(qint32 iD, QByteArray baData, int row, int col)
{
    // qDebug() << "slotHtrData" << iD << baData.size() << row << col;
    QVector<double> v;
    qint32 *htrVal=(qint32*)baData.data();

    /*
    for(int i=0;i<row*col;++i)
    {
        qDebug() << i << htrVal[i] ;
    }
    */

    for(int i=0;i<row;++i)
    {
        v.clear();
        v.fill(0.0,8);
        v[0]=htrVal[i*col+1];
        v[1]=htrVal[i*col+2];
        v[2]=htrVal[i*col+3];
        v[3]=htrVal[i*col+4];
        // qDebug() << v;
        trc->addPoint(v);
    }

}

void ApSingle::sqlError(QString text)
{
    qDebug() <<  "sqlError " << text;
}

void ApSingle::slotUpdate()
{
    foreach(QLineEdit *p,apLe)
    {
       SAFE(p->setText(src[p->objectName()]));
    }

    foreach(QLabel *p, lcdS)
    {
        SAFE(p->setText(src[p->objectName()]));
    }

    foreach(QLabel *p, le2)
    {
        double v=src[p->objectName()];
        SAFE(p->setText(QString::number(v,'f',1)));
    }
/*
    foreach(QLCDNumber *p,findChildren<QLCDNumber*>())
    {
        SAFE(p->display((double)src[p->objectName()]));
    }
*/

    foreach(AuLamp *p,lamp)
    {
        SAFE(p->setChecked(srcBM[p->objectName()]));
    }
    foreach(QPushButton *p,bnLamp)
    {
        // qDebug() << p->objectName();
        QString bnName=p->objectName().split('_')[1];
        QString bnVal=p->objectName().split('_')[2];
        int v=0;
        SAFE(v=(srcBM[sgKlApList[bnName][0]]?2:0) + (srcBM[sgKlApList[bnName][1]]?1:0));
        p->setIcon(QIcon(QStringLiteral(":/images/%1_%2.svg").arg(v).arg(bnVal)));

    }


    // вивести назву стадії
    int Kod=0;
    SAFE(Kod=src[QStringLiteral("Kod")]);
    if(Kod<0)
        Kod=0;
    if(Kod>kodState.size()-2)
        Kod=kodState.size()-1;
    ui->Kod->setText(kodState[Kod]);
    ui->Kod->setStyleSheet(kodColor[Kod]);

    qint16 Ey20=0;
    SAFE(Ey20=src[QStringLiteral("Ey20a_ind")]);
    if(Ey20<60)
        ui->blEy20a_ind->hide();
    else
        ui->blEy20a_ind->show();

    // можливо його ще треба буде порахувати

    // клапан K19
    int K19=1;
    const qint8 valK19[8]={5,0,7,2,1,4,3,6}; // карта співставлення індексів з картинками
    SAFE(K19=(srcBM["K192f_in"]?1:0)+(srcBM["K19_out"]?2:0)+(srcBM["K191f_in"]?4:0));
    ui->bnApSg_19_vval->setIcon(QIcon(QStringLiteral(":/images/%1_vval.svg").arg(valK19[K19])));

    // рамки навколо кнопок Kod_blok та Water_key
    SAFE(ui->Frame_block->setFrameShape(src[QStringLiteral("Kod_blok")]!=0?QFrame::Box:QFrame::NoFrame);)
    SAFE(ui->Kod_blok->setIcon(QIcon(src[QStringLiteral("Kod_blok")]?QStringLiteral(":/images/zamok1.png"):QStringLiteral(":/images/zamok2.png"))));

    SAFE(ui->frameWater->setFrameShape(src[QStringLiteral("Water_key")]!=0?QFrame::Box:QFrame::NoFrame);)
    SAFE(ui->pLt1a_ind->setValue(src[QStringLiteral("Lt1a_ind")]));
    ui->pLt1a_ind->update();

    SAFE(ui->bnReg_4->setIcon(QIcon(srcBM[QStringLiteral("K4f_in")]!=0?QStringLiteral(":/images/gr_vval.svg"):QStringLiteral(":/images/r_vval.svg"))));
    SAFE(ui->bnReg_6->setIcon(QIcon(srcBM[QStringLiteral("K6f_in")]!=0?QStringLiteral(":/images/gr_vval.svg"):QStringLiteral(":/images/r_vval.svg"))));
    SAFE(ui->bnReg_17->setIcon(QIcon(srcBM[QStringLiteral("K17f_in")]!=0?QStringLiteral(":/images/gr_vval.svg"):QStringLiteral(":/images/r_vval.svg"))));

    ui->va_msg1->setText(QString("До кінця стадії %1 сек.").arg(src["Indication_stad"].value()));
    ui->va_msg2->setText(QString("Варка триває вже %1 год. %2 хв.")
                          .arg(src["Dication_hour"].value()) //
                          .arg(src["Indication_min"].value()));//


}

void ApSingle::slotUpdateOb()
{
    foreach(QLineEdit *p,obLe)
    {
       p->setText(srcOB[p->objectName()]);
    }
    SAFE(ui->A_k1->setChecked(srcOB[QStringLiteral("A_k1")]));
    SAFE(ui->Ap_k1->setChecked(srcOB[QStringLiteral("Ap_k1_1")]));

    QString tag;
    foreach(QProgressBar *p,findChildren<QProgressBar*>(QRegularExpression("pAi*")))
    {
        tag=p->objectName();
        tag.remove(0,1);
        SAFE(p->setValue(srcOB[tag].valueReal()));
        p->update();
    }

    // роботу з кнопкою подачі зеленої патоки в мішалку прописати вручну


}

void ApSingle::slotSgKl()
{
    //qDebug() << sender()->objectName().split('_')[1];
    ApSgKl p(src,sgKlApList[sender()->objectName().split('_')[1]],this);
    p.exec();
}

// виклик вікна управління циркулятором
void ApSingle::slotCallCurkyl()
{
    ApWinCurkyl p(src,this);
    p.exec();
}

// як тут все просто
void ApSingle::slotControl()
{
    bool Val=false;
    QString tag=sender()->objectName();
    SAFE(Val=src[tag]);
    // qDebug() << tag << Val;
    if(tag=="Water_key" || tag=="Kod_blok")
        Val ^= true; // переключити
    else
        Val=true;

    SAFE(src[tag]=Val);
}

void ApSingle::blink()
{
    static bool off=false;

    foreach(AuLamp *p , blinkLamp)
    {
        if(p->checked()) // треба блимати
        {
                p->setLampOn(QPixmap(off?QStringLiteral(":/images/Alarm.png"):QStringLiteral(":/images/blackAlarm.png")));
        }
    }

    if(ui->frameWater->frameShape()==QFrame::Box)
        ui->frameWater->setStyleSheet(off?QStringLiteral("color: green;"):QStringLiteral("color: black;"));

    if(ui->Frame_block->frameShape()==QFrame::Box)
        ui->Frame_block->setStyleSheet(off?QStringLiteral("color: green;"):QStringLiteral("color: black;"));

    ui->blEy20a_ind->setStyleSheet(off?QStringLiteral("color: red"):QStringLiteral("color: black"));

    off ^=true;

}

void ApSingle::slotCallReg()
{
    int index=bnReg.indexOf(qobject_cast<QPushButton*>( sender()) );
    AuPanelReg p(myBase,src.index(),index,":/text/reg_ap.txt",this);
    p.exec();
}

