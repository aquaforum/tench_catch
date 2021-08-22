#include "roumnemo.h"
#include "ui_roumnemo.h"


#include <aulib.h>
#include <trendchar/trendchart.h>

#include <QVBoxLayout>
#include <QPalette>
#include <QTimer>
#include <QUiLoader>
#include <QFile>

#include <QPixmap>

#include <QRegExp>
#include <QTimer>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>
#include <QSlider>

#include <QUdpSocket>
#include <QFile>
#include <QDateTime>
#include <QHostInfo>
#include <QStyleOption>
#include <QPainter>


#include "dlglimit.h"

#include <QDebug>

#include <QPalette>

/*
#define ROU (*(src[6]))
#define GPI4 (*(src[5]))
#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))
#define APARAT (*(src[3]))
#define FQDRV (*(src[7]))
*/
#define ROU     QStringLiteral("plcrou")

RouMnemo::RouMnemo(AuBase &source,QWidget *parent) :
    QWidget(parent),src(source),
    ui(new Ui::RouMnemo)
{
    ui->setupUi(this);

    rou=src["plcrou"].index();



    connect(&src[rou],&AuPlc::updateData,this,&RouMnemo::updateData);

    //connect(src[2],SIGNAL(updateDataRaw()),this,SLOT(updateViparka()));
    connect(&src["plcvipar"],&AuPlc::updateData,this,&RouMnemo::updateViparka);

    //connect(src[7],SIGNAL(updateDataRaw()),this,SLOT(slotFqDrvUpdate()));
    connect(&src["plcfdmom"],&AuPlc::updateData,this,&RouMnemo::slotFqDrvUpdate);

    // виведення інформації з вакуум-апаратів
    for(int i=0;i<3;++i)
    {
        QString plc=QString("tmaap%1").arg(i+1);
        apIx[&src[plc]]=i;
        connect(&src[plc],&AuPlc::updateData,this,&RouMnemo::updateAparat);
    }

    foreach (QLineEdit *p, findChildren<QLineEdit*>(QRegularExpression("^r1_"))) {
       le << p;
    }


    foreach (QCheckBox *p, findChildren<QCheckBox*>()) {
        cb << p;
    }
    foreach (QPushButton *p, findChildren<QPushButton*>(QRegularExpression("^r1_xC"))) {
        connect(p,SIGNAL(clicked()),this,SLOT(slotCallReg()));
    }

    foreach (QProgressBar *p, findChildren<QProgressBar*>(QRegularExpression("^v1"))) {
       pb_v1 << p;
    }

    foreach (QLineEdit *p, findChildren<QLineEdit*>(QRegularExpression("^v1_"))) {
        le_v1 << p;
    }

    QFile ttagList(":/text/roulist.txt");
    ttagList.open(QIODevice::ReadOnly);

    {
        QFrame *frame=ui->trend1;

        TrendChart *t=new TrendChart(this);
        QVBoxLayout *vbl = new QVBoxLayout(frame);
        vbl->addWidget(t);
        vbl->setMargin(2);
        frame->setLayout(vbl);
        trc << t;

        QString tags=ttagList.readLine().trimmed();
        src.loadTrendChartData(t,tags);
        frame->parentWidget()->layout()->addWidget(drawLegend(tags,t,src[rou]));


    }
    {
        QFrame *frame=ui->trend2;

        TrendChart *t=new TrendChart(this);
        QVBoxLayout *vbl = new QVBoxLayout(frame);
        vbl->addWidget(t);
        vbl->setMargin(2);
        frame->setLayout(vbl);
        trc << t;
        QString tags=ttagList.readLine().trimmed();
        src.loadTrendChartData(t,tags);
        frame->parentWidget()->layout()->addWidget(drawLegend(tags,t,src[rou]));

    }
    ttagList.close();

    QTimer *t = new QTimer(this);
    t->setInterval(5000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrendChart()));

//    connect(ui->bnY0001,SIGNAL(pressed()),this,SLOT(slotOpen()));
//    connect(ui->bnY0002,SIGNAL(pressed()),this,SLOT(slotOpen()));

//    connect(ui->bnY0001,SIGNAL(released()),this,SLOT(slotClose()));
//    connect(ui->bnY0002,SIGNAL(released()),this,SLOT(slotClose()));




    ap_le << ui->a1_Lt1a_ind
          << ui->a2_Lt1a_ind
          << ui->a3_Lt1a_ind
          << ui->a1_Pt6a_ind
          << ui->a2_Pt6a_ind
          << ui->a3_Pt6a_ind
             ;
    ap_pb << ui->a1_pLt1a_ind
          << ui->a2_pLt1a_ind
          << ui->a3_pLt1a_ind
             ;


    connect(ui->bnLimit,SIGNAL(clicked()),this,SLOT(setupLimit()));




/*
    QCoreApplication::setApplicationName("viparka");
    QSettings s;
    if(s.value("/ioserv/hostname","localhost").toString()!="localhost") // якщо хост != localhost тоді слухаємо
    {
    sock=new QUdpSocket(this);
    sock->bind(8845);
    connect(sock,SIGNAL(readyRead()),this,SLOT(slotRead()));
    QTimer::singleShot(5000,this,SLOT(slotPreshQuery()));

    }
    else
    {
        ui->labelSpPresh->hide();
        ui->spPresh->hide();
    }
*/
}

RouMnemo::~RouMnemo()
{
    delete ui;
}

QFrame* RouMnemo::drawLegend(QString tags, TrendChart *trChar, AuPlc &plc)
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

void RouMnemo::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void RouMnemo::updateData() // слот обновляє дані на мнемосхемі
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());

    QString tag;
    foreach(QCheckBox *p,cb)
    {
        tag=p->objectName().split("_").at(1);
        SAFE(p->setChecked(s[tag]));
    }
    ui->r2_V0017->setText(s[QStringLiteral("V0017")]);
/*
    SAFE(if(s[QStringLiteral("X0001")].value()!=0)
        ui->r2_X0001->hide();
    else
        ui->r2_X0001->show(); )
*/

    SAFE(if(s[QStringLiteral("X0016")].value()!=0)
    {
        ui->fX0016->setFrameStyle(QFrame::Box);
        ui->nX0016->setFrameStyle(QFrame::NoFrame);

    }
    else
    {
        ui->fX0016->setFrameStyle(QFrame::NoFrame);
        ui->nX0016->setFrameStyle(QFrame::Box);

    })

//------------------------------------------------------------
    foreach (QLineEdit *p, le) {
        tag=p->objectName().split("_").at(1);
        SAFE(p->setText(s[tag]));
    }


    SAFE(ui->r2_V0004->setText(s[QStringLiteral("V0004")]));

    SAFE(ui->SP50003->setText(s[QStringLiteral("SP50003")]));
    SAFE(ui->SP70003->setText(s[QStringLiteral("SP70003")]));

}


void RouMnemo::updateAparat()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());

    int i=apIx[sender()];

    ap_le[i]->setText(s[QStringLiteral("Lt1a_ind")]);
    ap_le[i+3]->setText(s[QStringLiteral("Pt6a_ind")]);
    ap_pb[i]->setValue(s[QStringLiteral("Lt1a_ind")].value32());

}

void RouMnemo::updateTrendChart() // поновлення графіків
{
    QVector<double> v;
    QFile ttagList(":/text/roulist.txt");

    if(ttagList.open(QIODevice::ReadOnly))
    {
        int i=0;
        for(;!ttagList.atEnd();)
        {
            QStringList tags=QString::fromLatin1(ttagList.readLine()).trimmed().split('\t'); // прочитати рядок
            QString plcName =tags[0]; // перший елемент - назва контролера
            tags.pop_front();

            if(tags.size()>0) // якщо там щось є тоді
            {
                v.clear();
                SAFE( foreach(QString tag,tags)
                    v<< src[plcName][tag].valueReal();
                trc[i++]->addPoint(v);
                )
            }

        }
        ttagList.close();
    }
    //qDebug() << "-";
}

void RouMnemo::updateToolTips()
{

}

void RouMnemo::slotCallReg()
{
    AuPanelReg p(src,rou,sender()->objectName().right(2).toInt()-1,":/text/reg_rou.txt",this);
    p.exec();
}


void RouMnemo::slotOpen()
{
    QString tag = sender()->objectName().right(5);
    //ROU[0]->sendValue(tag,qint16(-1));
    SAFE(src[rou][tag].setValue(-1));
    //qDebug() << "------!!!!!!!!!--------"<<  sender()->objectName().right(5) ;
}

void RouMnemo::slotClose()
{
    QString tag = sender()->objectName().right(5);
    //ROU[0]->sendValue(tag,qint16(0));
    SAFE(src[rou][tag].setValue(0));
    //qDebug() << "------&&&&&&&&&&&&--------"<<  sender()->objectName().right(5) ;
}

void RouMnemo::updateViparka()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    QString tag;

    SAFE(ui->v2_V_35->setText(s[QStringLiteral("V_35")]));
    SAFE(ui->v2_V_36->setText(s[QStringLiteral("V_36")]));

    foreach (QLineEdit *p , le_v1) {
        tag=p->objectName().right(4);
        SAFE(p->setText(s[tag]));
    }
    foreach (QProgressBar *p, pb_v1) {
        tag=p->objectName().right(4);
        SAFE(p->setValue(s[tag].value32()));
    }

}


void RouMnemo::setupLimit()
{
    DlgLimit p(src[ROU],this);
    p.exec();
}

void RouMnemo::slotFqDrvUpdate()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    QString tag;

    QVector<QPushButton*> fd1_BN;
    fd1_BN << nullptr // 0
           << nullptr // 1
           << nullptr // 2
           << nullptr // 3
           << nullptr // 4
           << nullptr // 5
           << nullptr // 6
           << ui->v1_xC_16;

    int i=7;
    {
        bool Link,Run,Alarm;
        SAFE(
        Link=s[QString("Link%1").arg(i+1,4,10,QChar('0'))];
        Run=s[QString("Run%1").arg(i+1,4,10,QChar('0'))];
        Alarm=s[QString("Alarm%1").arg(i+1,4,10,QChar('0'))];
        )

        if(fd1_BN[i])
        {
            QString prefix="pr";
            switch (i) {
            case 0:
            case 4:
            case 5:
            case 13:
            case 14:
                prefix="pl";
                break;
            default:
                break;
            }

            if(Link)
            {
                if(Alarm)
                {
                    fd1_BN[i]->setIcon(QIcon(QString(":/%1_36x34_1.png").arg(prefix)));
                    fd1_BN[i]->setToolTip(tr("Аварійна зупинка"));
                }
                else
                {
                    if(Run)
                    {

                        fd1_BN[i]->setIcon(QIcon(QString(":/%1_36x34_2.png").arg(prefix)));
                        fd1_BN[i]->setToolTip(QString(tr("Робота\n"
                                                "Частота: %1Гц\n"
                                                "Струм: %2А\n"
                                                "Напруга: %3В\n"
                                                         "Потужність: %4кВт")
                                .arg((double)s[QString("V%1").arg(i+1,4,10,QChar('0'))])
                                .arg((double)s[QString("V%1").arg(i+21,4,10,QChar('0'))])
                                .arg((double)s[QString("V%1").arg(i+41,4,10,QChar('0'))])
                                .arg((double)s[QString("V%1").arg(i+41,4,10,QChar('0'))]
                                *(double)s[(QString("V%1").arg(i+21,4,10,QChar('0')))]
                                *0.00173  )
                                ));

                    }
                    else
                    {
                        fd1_BN[i]->setIcon(QIcon(QString(":/%1_36x34_3.png").arg(prefix)));
                        fd1_BN[i]->setToolTip(tr("Привод зупинено"));
                    }
                }
            }
            else
            {
                fd1_BN[i]->setIcon(QIcon(QString(":/%1_36x34_0.png").arg(prefix)));
                fd1_BN[i]->setToolTip(tr("Відсутній зв’язок"));
            }
        }
    }
}


void RouMnemo::slotRead()
{
    while (sock->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(sock->pendingDatagramSize());
        QHostAddress addr;
        sock->readDatagram(datagram.data(),datagram.size(),&addr);
        // qDebug() << datagram.data();
#ifndef WIN32
        QFile f("spPresh.log");
        f.open(QIODevice::Append);
#endif
        QList<QHostAddress> vpAddr=QHostInfo::fromName("vipar").addresses();
        if(vpAddr.size()>0)
        {
            if(vpAddr[0].toIPv4Address()==addr.toIPv4Address())
            {
                QStringList val;
                foreach (QString s, datagram.trimmed().split('='))
                         val << s;
                if(val.size()>1)
                {
                    ui->spPresh->setText(val[1]);

#ifndef WIN32
                    f.write(QString("%1 %2:spPresh=%3\n")
                            .arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"))
                            .arg(addr.toString())
                            .arg(ui->spPresh->text()).toLatin1());
#endif
                }
            }

        }


#ifndef WIN32
        f.close();
#endif

    }
}

void RouMnemo::slotPreshQuery()
{
    QUdpSocket qry;
    qry.writeDatagram("?spPresh",QHostAddress::Broadcast,8846); // перепитати про всяк випадок
}




