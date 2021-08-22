#include "mnemoboiler2.h"
#include "ui_mnemoboiler2.h"

#include <QStyleOption>
#include <QDebug>
#include <QVBoxLayout>
#include <QTimer>

#include <aulib.h>
#include "trendchar/trendchart.h"

MnemoBoiler2::MnemoBoiler2(AuBase &base, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MnemoBoiler2),
    source(base),
    src(base["plck2"])

{
    ui->setupUi(this);

    label=findChildren<QLabel*>();
    // qDebug() << label;
    lamp=findChildren<AuLamp*>();

    int i=0,iX=-1;
    for(i=0;i<lamp.size();++i)
        if(lamp[i]==ui->flame)
            iX=i;
    lamp.removeAt(iX);
    // донастроїти індикатор факеда
    ui->flame->addLamp(QPixmap(QString::fromUtf8(":/mnemo/flame_1.svg")));
    ui->flame->addLamp(QPixmap(QString::fromUtf8(":/mnemo/flame_4.svg")));

    QList<QFrame*> fTrc;
    fTrc << ui->frame_01
         << ui->frame_02
         << ui->frame_03;

    trcTags << "plck2\tPpara\tPp7a\tFpara\tTpar"
            << "plck2\tLdrum\tSp0001\tcPump\tFpara\tFwater"
            << "plck2\tPvac\tSp0003\tcExh\tPair\tSp0002\tcFan\tQO2fume";
    i=0;
    foreach(QFrame *p,fTrc)
    {
        // Створити сам графік
        TrendChart *t=new TrendChart(this);
        QVBoxLayout *vbl = new QVBoxLayout(p);
        vbl->addWidget(t);
        vbl->setMargin(2);
        p->setLayout(vbl);
        trc << t;

        base.loadTrendChartData(t,trcTags[i]);

        p->parentWidget()->layout()->addWidget(drawLegend(trcTags[i++],t,src));

    }
    QTimer *tmr=new QTimer(this);
    tmr->setInterval(5000);
    connect(tmr,&QTimer::timeout,this,&MnemoBoiler2::slotUpdateTrc);
    tmr->start();


    // завантажити ісонки для кнопок
    QIcon icon1,icon2,icon3,icon4;
    icon1.addFile(QString::fromUtf8(":/pr_36x34_0.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon2.addFile(QString::fromUtf8(":/pr_36x34_2.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon3.addFile(QString::fromUtf8(":/mnemo/val_g_0.svg"), QSize(), QIcon::Normal, QIcon::Off);
    icon4.addFile(QString::fromUtf8(":/mnemo/val_g_1.svg"), QSize(), QIcon::Normal, QIcon::Off);

    bnPumpIcon << icon1
               << icon2
               << icon3
               << icon4;

    foreach(QPushButton *p,findChildren<QPushButton*>(QRegularExpression("^bnReg_")))
    {
        //qDebug() << p->objectName();
        connect(p,&QPushButton::clicked,this,&MnemoBoiler2::slotCallReg);
    }



    connect(&src,&AuPlc::updateData,this,&MnemoBoiler2::slotUpdate);
}

MnemoBoiler2::~MnemoBoiler2()
{
    delete ui;
}


QFrame* MnemoBoiler2::drawLegend(QString tags, TrendChart *trChar, AuPlc &plc)
{
    QFrame *label=new QFrame(this);
    label->setStyleSheet("background-color: black;");
    label->setFrameShape(QFrame::StyledPanel);

    QGridLayout *glx=new QGridLayout(label);
    glx->setMargin(1);
    glx->setVerticalSpacing(1);
    glx->setHorizontalSpacing(1);
    label->setLayout(glx);

    // розділити на поля
    QStringList tagL=tags.split('\t');
    for(int m=tagL.size();m<9;++m)
        tagL<<""; // доповнити до 9 полів

     //qDebug() << tagL;
    QColor clr;
    for(int m=0;m<3;++m)
    {

        for(int i=0;i<2;++i)
        {

            QFrame *f1=new QFrame(label);
            QVBoxLayout *v1=new QVBoxLayout(f1);
            v1->setMargin(1);
            f1->setLayout(v1);

            f1->setFrameStyle(QFrame::Box);

            clr=trChar->color(m*2+i);

            QLabel *l1=new QLabel(f1);
            l1->setFont(QFont("Hack",8));
            v1->addWidget(l1);

            QString text;
            if(tagL[m*2+1+i].size()>0)
            {
                SAFE(text=plc[tagL[m*2+1+i]].text());
                // text.truncate(20);

                f1->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(clr.red()).arg(clr.green()).arg(clr.blue()));
                l1->setStyleSheet("color: white;");
                l1->setText(text);
            }
            else
            {
                text="";
            }

            glx->addWidget(f1,m,i);


        }

    }
    return label;
}


void MnemoBoiler2::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MnemoBoiler2::slotUpdate()
{
    foreach(QLabel *p,label)
    {
        SAFE(p->setText(src[p->objectName()]));
    }

    foreach(AuLamp *p,lamp)
    {
        SAFE(p->setChecked(src[p->objectName()]));
    }

    SAFE(
    ui->flame->setLamp(src["flame"].value());
    // qDebug() << "flame " << src["flame"].value() ;
    SAFE(ui->sl_SP0001->setValue(src[QStringLiteral("Sp0001")].valueReal()));

    ui->bnReg_01->setIcon(bnPumpIcon[src["xPumpRun"]?1:0]);
    ui->bnReg_02->setIcon(bnPumpIcon[src["xFanRun"]?1:0]);
    ui->bnReg_03->setIcon(bnPumpIcon[src["xExhRun"]?1:0]);
    ui->bnReg_04->setIcon(bnPumpIcon[src["gGas"].valueReal()>200.0?3:2]);
    ui->bnReg_05->setIcon(bnPumpIcon[src["gTerm"].valueReal()>200.0?3:2]);
    ui->bnReg_06->setIcon(bnPumpIcon[src["gSupp"].valueReal()>200.0?3:2]);

    ui->pb_Ldrum->setValue(src["Ldrum"].value32());
    ui->pb_Ldrum->update();
    )

}

void MnemoBoiler2::slotUpdateTrc()
{
    QVector<double> val;
    for(int i=0;i<trc.size();++i)
    {
        val.clear();
        QStringList tags=trcTags[i].split("\t");

        tags.removeAt(0);
        for(int j=0;j<tags.size();++j)
            SAFE(val << src[tags[j]].valueReal());
        trc[i]->addPoint(val);
        // qDebug() << tags << val;
    }
}

void MnemoBoiler2::slotCallReg()
{
    int nReg=sender()->objectName().right(2).toInt()-1;

    if(nReg==5) // скоригувати на кнопу клапану підпору
        nReg=4;
    AuPanelReg reg(source,src.index(),nReg,":/text/reg_boiler2.txt",this);
    reg.exec();

}

