#include "roumnemo.h"
#include "ui_roumnemo.h"


#include <IoNetClient.h>

#include "trendchart.h"
#include "panelReg.h"
#include <QtGui>

#include <QVBoxLayout>
#include <QPalette>
#include <QTimer>
#include <QUiLoader>
#include <QFile>

#include <QPixmap>

#include <panelReg.h>
#include <QRegExp>
#include <trendchart.h>
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


#include "dlglimit.h"

#include <QDebug>

#include <QPalette>

#define ROU (*(src[6]))

#define GPI4 (*(src[5]))

#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))


#define APARAT (*(src[3]))

#define FQDRV (*(src[7]))


RouMnemo::RouMnemo(QVector<IoNetClient*> &source,QWidget *parent) :
    QWidget(parent),src(source),
    ui(new Ui::RouMnemo)
{
    ui->setupUi(this);

    connect(src[6],SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(src[6],SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
    connect(src[6],SIGNAL(updateTags(int)),this,SLOT(updateToolTips(int)));

    connect(src[2],SIGNAL(updateDataRaw()),this,SLOT(updateViparka()));

    connect(src[7],SIGNAL(updateDataRaw()),this,SLOT(slotFqDrvUpdate()));


    foreach (QLineEdit *p, findChildren<QLineEdit*>(QRegExp("^r1_"))) {
       le << p;
    }


    foreach (QCheckBox *p, findChildren<QCheckBox*>()) {
        cb << p;
    }
    foreach (QPushButton *p, findChildren<QPushButton*>(QRegExp("^r1_xC"))) {
        connect(p,SIGNAL(clicked()),this,SLOT(slotCallReg()));
        qDebug() << "------------" << p->objectName();
    }

    foreach (QProgressBar *p, findChildren<QProgressBar*>(QRegExp("^v1"))) {
       pb_v1 << p;
    }

    foreach (QLineEdit *p, findChildren<QLineEdit*>(QRegExp("^v1_"))) {
        le_v1 << p;
    }


    {
    QFrame *frame=ui->trend1;

        TrendChart *t=new TrendChart(this);
    QVBoxLayout *vbl = new QVBoxLayout(frame);
    vbl->addWidget(t);
    vbl->setMargin(2);
    frame->setLayout(vbl);
    trc << t;
    }
    {
    QFrame *frame=ui->trend2;

        TrendChart *t=new TrendChart(this);
    QVBoxLayout *vbl = new QVBoxLayout(frame);
    vbl->addWidget(t);
    vbl->setMargin(2);
    frame->setLayout(vbl);
    trc << t;
    }

    QTimer *t = new QTimer(this);
    t->setInterval(5000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrendChart()));

    connect(ui->bnY0001,SIGNAL(pressed()),this,SLOT(slotOpen()));
    connect(ui->bnY0002,SIGNAL(pressed()),this,SLOT(slotOpen()));

    connect(ui->bnY0001,SIGNAL(released()),this,SLOT(slotClose()));
    connect(ui->bnY0002,SIGNAL(released()),this,SLOT(slotClose()));




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

}

RouMnemo::~RouMnemo()
{
    delete ui;
}


void RouMnemo::updateDataRaw() // слот обновляє дані на мнемосхемі
{
    foreach(QCheckBox *p,cb)
    {
        p->setChecked(ROU[0]->getValue16(p->objectName().split("_")[1]));
    }
    ui->r2_V0017->setText(QString("%1").arg(ROU[0]->getValueFloat("V0017"),0,'f',0));
    ui->v2_V_35->setText(QString("%1").arg(VIPARKA[0]->getValueScaled("V_35"),0,'f',1));
    ui->v2_V_36->setText(QString("%1").arg(VIPARKA[0]->getValueScaled("V_36"),0,'f',1));

    if(ROU[0]->getValue16("X0001"))
        ui->r2_X0001->hide();
    else
        ui->r2_X0001->show();

    if(ROU[0]->getValue16("X0016"))
    {
        ui->fX0016->setFrameStyle(QFrame::Box);
        ui->nX0016->setFrameStyle(QFrame::NoFrame);

    }
    else
    {
        ui->fX0016->setFrameStyle(QFrame::NoFrame);
        ui->nX0016->setFrameStyle(QFrame::Box);

    }

}

void RouMnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{
    foreach (QLineEdit *p, le) {
        int dp=0;
        if(ROU[0]->scaleFull(p->objectName().split("_")[1])>50)
            dp=0;
        else
            dp=1;
        p->setText(QString("%1").arg( ROU[0]->getValueScaled(p->objectName().split("_")[1]),0,'f',dp));
    }

    for (int i=0;i<3;++i)
    {
        ap_le[i]->setText(QString("%1").arg(APARAT[i]->getValueFloat("Lt1a_ind"),0,'f',0));
        ap_le[i+3]->setText(QString("%1").arg(APARAT[i]->getValueFloat("Pt6a_ind"),0,'f',0));
        ap_pb[i]->setValue(APARAT[i]->getValueFloat("Lt1a_ind"));
    }

    ui->r2_V0004->setText(QString("%1").arg(ROU[0]->getValueScaled("V0004"),0,'f',2));

    ui->SP50003->setText(QString("%1").arg(ROU[0]->getValueScaled("SP50003"),0,'f',0));
    ui->SP70003->setText(QString("%1").arg(ROU[0]->getValueScaled("SP70003"),0,'f',2));



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
            int iX=tags[0].toInt(); // перший елемент - номер індексу сервера
            int iD=tags[1].toInt(); // індекс iodev
            tags.pop_front();
            tags.pop_front();

            if(tags.size()>0) // якщо там щось є тоді
            {
                v.clear();
                foreach(QString tag,tags)
                    v<< (*src[iX])[iD]->getValueFloat(tag);
                trc[i++]->addPoint(v);
            }

        }
        ttagList.close();
    }
    //qDebug() << "-";
}

void RouMnemo::updateToolTips(int)
{

}

void RouMnemo::slotCallReg()
{

    int idX=sender()->objectName().right(2).toInt()-1;
    QCoreApplication::setApplicationName("rou");

    RpanelReg p(*ROU[0],idX,this,":/text/reg_rou.txt","trend");
    p.exec();


}


void RouMnemo::slotOpen()
{
    QString tag = sender()->objectName().right(5);
    ROU[0]->sendValue(tag,qint16(-1));
    qDebug() << "------!!!!!!!!!--------"<<  sender()->objectName().right(5) ;

}

void RouMnemo::slotClose()
{
    QString tag = sender()->objectName().right(5);
    ROU[0]->sendValue(tag,qint16(0));
    qDebug() << "------&&&&&&&&&&&&--------"<<  sender()->objectName().right(5) ;


}

void RouMnemo::updateViparka()
{
    foreach (QLineEdit *p , le_v1) {
        p->setText(QString("%1").arg(VIPARKA[0]->getValueScaled(p->objectName().right(4)),0,'f',0));
    }
    foreach (QProgressBar *p, pb_v1) {
        p->setValue(VIPARKA[0]->getValueFloat(p->objectName().right(4)));
    }

}


void RouMnemo::setupLimit()
{
    DlgLimit p(*ROU[0],this);

    p.exec();

}

void RouMnemo::slotFqDrvUpdate()
{
    QPushButton *fd1_BN=ui->v1_xC_16;

    int i=7;
    {
        bool Link,Run,Alarm;
        Link=FQDRV[0]->getValue16(QString("Link%1").arg(i+1,4,10,QChar('0')));
        Run=FQDRV[0]->getValue16(QString("Run%1").arg(i+1,4,10,QChar('0')));
        Alarm=FQDRV[0]->getValue16(QString("Alarm%1").arg(i+1,4,10,QChar('0')));
        if(fd1_BN)
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
                    fd1_BN->setIcon(QIcon(QString(":/%1_36x34_1.png").arg(prefix)));
                    fd1_BN->setToolTip(tr("Аварійна зупинка"));
                }
                else
                {
                    if(Run)
                    {
                        fd1_BN->setIcon(QIcon(QString(":/%1_36x34_2.png").arg(prefix)));
                        fd1_BN->setToolTip(QString(tr("Робота\n"
                                                "Частота: %1Гц\n"
                                                "Струм: %2А\n"
                                                "Напруга: %3В")
                                .arg(FQDRV[0]->getValueScaled(QString("V%1").arg(i+1,4,10,QChar('0'))))
                                .arg(FQDRV[0]->getValueScaled(QString("V%1").arg(i+21,4,10,QChar('0'))))
                                .arg(FQDRV[0]->getValueScaled(QString("V%1").arg(i+41,4,10,QChar('0'))))
                                ));

                    }
                    else
                    {
                        fd1_BN->setIcon(QIcon(QString(":/%1_36x34_3.png").arg(prefix)));
                        fd1_BN->setToolTip(tr("Привод зупинено"));
                    }
                }
            }
            else
            {
                fd1_BN->setIcon(QIcon(QString(":/%1_36x34_0.png").arg(prefix)));
                fd1_BN->setToolTip(tr("Відсутній зв’язок"));
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




