
#include "mnemo.h"
#include "ui_mnemo.h"

#include <IoNetClient.h>

#include "trendchart.h"
#include "panelReg.h"
#include <QtGui>

// унікальні діалогові вікна
#include <QVBoxLayout>
#include <QPalette>
#include <QTimer>
#include <QUiLoader>
#include <QFile>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>
#include <QSlider>
#include <QSpinBox>
#include <QUdpSocket>


#include "dlgvbctrl.h"
#include "dlgpumpctrl.h"

#include "amVater.h"
#include "dlgmassview.h"


#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))

#define FQDRV (*(src[7]))

Mnemo::Mnemo(QVector<IoNetClient*> &source, QWidget *p) : QWidget(p), src(source)
{
    foreach(IoNetClient *s,src)
    {
        if(s->objectName()!="fqdrvmon"){
        connect(s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
        connect(s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
        connect(s,SIGNAL(updateTags(int)),this,SLOT(updateToolTips(int)));
        }
        else
        {
            connect(s,SIGNAL(updateDataRaw()),this,SLOT(slotFqDrvUpdate())); // при отриманні нових даних, засвітити їх на картинці
        }

    }

    QStringList lTexts;
    lTexts
            << tr("Співвідношення стружка-вода") // 0
            << tr("Рівні дифузії") // 1
            << tr("Відкачка дифузійного соку") //2


            << tr("Вапно на осн. дефекацію") // 3
            << tr("Відкачка з Х.Д.") // 4
            << tr("pH 1 сатурації")  // 5
            << tr("Подача соку на 2 сатурацію")  // 6
            << tr("pH 2 сатурації") // 7

            << tr("Рівень в 0 корпусі")  //8
            << tr("Рівень в 1 корпусі")  // 9
            << tr("Рівень в 2 корпусі")  //10
            << tr("Рівень в 3 корпусі")  // 11
            << tr("Рівень в 4,5 корпусі")  // 12
               ;

    QGridLayout *bxLayout =new QGridLayout;
    bxLayout->setMargin(0);
    bxLayout->setVerticalSpacing(1);
    bxLayout->setHorizontalSpacing(1);


    QUiLoader *uil_d= new QUiLoader(this);
    QFile fileui_d(":/mnemo/mnemo_d.ui");
    m_d=uil_d->load(&fileui_d); // завантажити файл із мордою

    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);

    if(m_d)
    {
        bxLayout->addWidget(m_d,0,0,1,3);

        for(int i=0;i<3;++i)
        {
            // створити підпис графіка
            QLabel *lab=new QLabel(this);
            lab->setText(lTexts[i]);
            lab->setFont(font);
            lab->setMaximumHeight(15);
            bxLayout->addWidget(lab,1,i);

            // створити рамку, взагалі треба якось переробити клас TrendChar на роботу у фреймі.
            QFrame *frame =new QFrame(this);
            frame->setFrameShape(QFrame::StyledPanel);
            bxLayout->addWidget(frame,2,i);

            // Створити сам графік
            TrendChart *t=new TrendChart(this);
            QVBoxLayout *vbl = new QVBoxLayout(frame);
            vbl->addWidget(t);
            vbl->setMargin(2);
            frame->setLayout(vbl);
            trc << t;
        }

        d1_Y_01=m_d->findChild<QPushButton*>("d1_Y_01");
        connect(d1_Y_01,SIGNAL(clicked()),this,SLOT(slotCallVbCtrl()));


    }


    QUiLoader *uil_s= new QUiLoader(this);
    QFile fileui_s(":/mnemo/mnemo_s.ui");
    m_s=uil_s->load(&fileui_s); // завантажити файл із мордою

    if(m_s)
    {
        bxLayout->addWidget(m_s,0,3,1,5);

        for(int i=3;i<8;++i)
        {
            QLabel *lab=new QLabel(this);
            lab->setFont(font);
            lab->setText(lTexts[i]);
            lab->setMaximumHeight(15);
            bxLayout->addWidget(lab,1,i);

            QFrame *frame =new QFrame(this);
            frame->setFrameShape(QFrame::StyledPanel);
            bxLayout->addWidget(frame,2,i);
            // Створити сам графік
            TrendChart *t=new TrendChart(this);
            QVBoxLayout *vbl = new QVBoxLayout(frame);
            vbl->addWidget(t);
            vbl->setMargin(2);
            frame->setLayout(vbl);
            trc << t;
        }

        s1_bleding
        << m_s->findChild<QPushButton*>("s1_Y_01")
        << m_s->findChild<QPushButton*>("s1_Y_02")
        << m_s->findChild<QPushButton*>("s1_Y_03")
        << m_s->findChild<QPushButton*>("s1_Y_04")
        << m_s->findChild<QPushButton*>("s1_Y_05")
        << m_s->findChild<QPushButton*>("s1_Y_06");

        foreach(QPushButton* p,s1_bleding)
            connect(p,SIGNAL(clicked()),this,SLOT(slotCallVbCtrl()));

        QPushButton *p=m_s->findChild<QPushButton*>("s1_cP_01");
        connect(p,SIGNAL(clicked()),this,SLOT(slotCallPumpCtrl()));


    }

    QUiLoader *uil_v= new QUiLoader(this);
    QFile fileui_v(":/mnemo/mnemo_v.ui");
    m_v=uil_v->load(&fileui_v); // завантажити файл із мордою

    if(m_v)
    {

        bxLayout->addWidget(m_v,0,8,1,5);
        for(int i=8;i<13;++i)
        {
            QLabel *lab=new QLabel(this);
            lab->setFont(font);

            lab->setText(lTexts[i]);
            lab->setMaximumHeight(15);
            bxLayout->addWidget(lab,1,i);

            QFrame *frame =new QFrame(this);
            frame->setFrameShape(QFrame::StyledPanel);
            bxLayout->addWidget(frame,2,i);

            // Створити сам графік
            TrendChart *t=new TrendChart(this);
            QVBoxLayout *vbl = new QVBoxLayout(frame);
            vbl->addWidget(t);
            vbl->setMargin(2);
            frame->setLayout(vbl);
            trc << t;
        }

        foreach(QSlider *p,m_v->findChildren<QSlider*>())
            v1_SP << p;
    }
    resize(m_d->size().width()+m_s->size().width()+m_v->size().width(),960);
    setLayout(bxLayout);

    QVector<QWidget*> sMnemo;
    sMnemo << m_d << m_s << m_v;
    foreach(QWidget* m,sMnemo)
    {
        QString tmp; // цюди буду записувати що куди складувати.
        // пройтися на предмет елементів управління
        foreach(QLineEdit *p,m->findChildren<QLineEdit*>())
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="d1")
                d1_VC << p;
            else if(tmp=="s1")
                s1_VC << p;
            else if(tmp=="v1")
                v1_VC << p;
        }

        foreach(QProgressBar *p,m->findChildren<QProgressBar*>())
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="d1")
                d1_lV << p;
            else if(tmp=="s1")
                s1_lV << p;
            else if(tmp=="v1")
                v1_lV << p;
        }

        foreach(QCheckBox *p,m->findChildren<QCheckBox*>())
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="d1")
                d1_Cb << p;
            else if(tmp=="s1")
                s1_Cb << p;
            else if(tmp=="v1")
                v1_Cb << p;
        }

        foreach(QPushButton *p,m->findChildren<QPushButton*>())
        {
            tmp=p->objectName().split('_')[0];
            if(p->objectName().split('_')[1]=="xC") // якщо кнопки регуляторів
            {
                if(tmp=="d1")
                    d1_bC << p;
                else if(tmp=="s1")
                    s1_bC << p;
                else if(tmp=="v1")
                    v1_bC << p;
                connect(p,SIGNAL(clicked()),this,SLOT(slotCallReg()));
            }
        }


    }

    QTimer *t = new QTimer(this);
    t->setInterval(5000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrendChart()));


    //кнопка управління подачею аміачної води
    connect(m_s->findChild<QPushButton*>("v1_X_06"),SIGNAL(clicked()),this,SLOT(slotCallAmVater()));
    connect(m_d->findChild<QPushButton*>("bn_MassView"),SIGNAL(clicked()),this,SLOT(slotShowMassView()));


    QStringList bnNames ;
    bnNames << "d1_xC_09"
                << "s1_xC_15"
                << "f1_xC_00"
                << "d1_xC_06"
                << "f1_xC_01"
                << "f1_xC_02"
                << ""
                << "v1_xC_01"
                << "v1_xC_07"
                << ""
                << "s1_xC_18"
                << "s1_xC_12"
                << ""
                << "s1_xC_08"
                << "s1_xC_13"
                << "d1_xC_13";

    foreach (QString p, bnNames) {
        if(p.size()>0)
        {
            QPushButton *bn=findChild<QPushButton*>(p);
            if(bn)
            {
                fd1_BN << bn;
            }
            else
            {
                qDebug() << p << "!!!! Not Found";
                fd1_BN << NULL;
            }
        }
        else
        {
            fd1_BN << NULL;
        }
    }

    xC05=m_s->findChild<QPushButton*>("s1_xC_05");
    xC20=m_s->findChild<QPushButton*>("s1_xC_20");

// передача завдання на тиск
    spPresh=m_v->findChild<QSpinBox*>("spPresh");
    if(spPresh)
    {
        QCoreApplication::setApplicationName("viparka");
        QSettings s;
        if(s.value("/ioserv/hostname","localhost").toString()=="localhost") // якщо хост == localhost тоді можна передавати
        {
            spPresh->setValue(s.value("spPresh",160).toInt());
            connect(spPresh,SIGNAL(valueChanged(int)),this,SLOT(slotSendSpPresh(int)));

            sock=new QUdpSocket(this);
            sock->bind(8846);
            connect(sock,SIGNAL(readyRead()),this,SLOT(slotSpPreshSend()));

            QUdpSocket sockS;
            sockS.writeDatagram(QString("spPresh=%1\n").arg(spPresh->value()).toLatin1(),QHostAddress::Broadcast,8845);

        }
        else
        {
            spPresh->hide(); // інакше сховати його
            m_v->findChild<QLabel*>("label_9")->hide();
        }

    }

    // Рекомендації по відкачці з холодного
    ss_REC=m_s->findChild<QLabel*>("ss_REC");
    ss_SUM << m_d->findChild<QLineEdit*>("d1_V_15")
           << m_s->findChild<QLineEdit*>("s1_V_03")
           << m_s->findChild<QLineEdit*>("s1_V_04")
           << m_s->findChild<QLineEdit*>("s1_V_06")
           << m_s->findChild<QLineEdit*>("s1_V_07")
           << m_s->findChild<QLineEdit*>("s1_V_09");


    V10=m_v->findChild<QLineEdit*>("va_V_10");

}


Mnemo::~Mnemo()
{

}

void Mnemo::updateToolTips(int v)
{
    qDebug() << "updateToolTips(" << v  << ")" << "sender" << sender()->objectName();
    IoNetClient &s=*qobject_cast<IoNetClient*>(sender());

    // заповнити поля toolTip
    if(sender()->objectName()=="difuz")
    {
        foreach(QLineEdit *p,d1_VC)
        {
            p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-3)]);
        }

        // заповнити поля toolTip
        foreach(QProgressBar *p,d1_lV)
        {
            p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-3)]);
        }

    }
    else if(sender()->objectName()=="satur")
    {
        if(v==0)
        {
            foreach(QLineEdit *p,s1_VC)
            {
                p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-3)]);
            }

            // заповнити поля toolTip
            foreach(QProgressBar *p,s1_lV)
            {
                p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-3)]);
            }

        }
    }
    else if(sender()->objectName()=="viparka")
    {
        foreach(QLineEdit *p,v1_VC)
        {
            p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-3)]);
        }

        // заповнити поля toolTip
        foreach(QProgressBar *p,v1_lV)
        {
            p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-3)]);
        }
    }


}

void Mnemo::updateDataRaw()
{
    IoNetClient &s=*qobject_cast<IoNetClient*>(sender());

    QVector<QProgressBar*> pb;
    QVector<QCheckBox*> cb;


    if(sender()->objectName()=="difuz")
    {
        pb=d1_lV;
        cb=d1_Cb;

        d1_Y_01->setIcon(QIcon(QPixmap(s[0]->getValue16("Y_01")
                                         ?":/pict/lib/valve_green_h_20x32.png":":/pict/lib/valve_off_h_20x32.png")));

    }
    else if(sender()->objectName()=="satur")
    {
        pb=s1_lV;
        cb=s1_Cb;

        foreach(QPushButton *p,s1_bleding)
        {
            //qDebug() << p->objectName();
            p->setIcon(QIcon(QPixmap(s[0]->getValue16(p->objectName().right(p->objectName().size()-3))
                                             ?":/pict/lib/valve_green_20x32.png":":/pict/lib/valve_off_20x32.png")));
        }

        if(s[0]->getValue16("X_09")==0)
        {
            xC05->show();
            xC20->hide();
        }
        else
        {
            xC05->hide();
            xC20->show();
        }



    }
    else if(sender()->objectName()=="viparka")
    {
        pb=v1_lV;
        cb=v1_Cb;
        foreach(QSlider*p,v1_SP)
        {
            p->setValue(s[0]->getValueFloat(p->objectName().right(p->objectName().size()-3)));
        }

        {
            QPushButton *p=m_s->findChild<QPushButton*>("v1_X_06");
            p->setIcon(QIcon(QPixmap(s[0]->getValue16("Y_01")
                                         ?":/pict/lib/valve_green_20x32.png":":/pict/lib/valve_off_20x32.png")));
        }

        {
            QLineEdit *p = m_s->findChild<QLineEdit*>("va_V_41");
            p->setText(QString("%1").arg(s[0]->getValueFloat("V_41"),0,'f',1));
        }
        {
            QLineEdit *p = m_s->findChild<QLineEdit*>("va_V_16");
            p->setText(QString("%1").arg(s[0]->getValueFloat("V_16"),0,'f',1));

        }
        {
            QLineEdit *p = m_v->findChild<QLineEdit*>("va_V_52");
            p->setText(QString("%1").arg(s[0]->getValueFloat("V_52"),0,'f',1));
        }

        double r=1.0+VIPARKA[0]->getValueFloat("V_10")/(30.0*40.0);
        r=120.0*(1.0-1.0/r)-2.0;
        V10->setText(QString("%1").arg(r,0,'f',1));

    }


    foreach(QProgressBar *p,pb)
    {
        int v=s[0]->getValueFloat(p->objectName().right(p->objectName().size()-4));
        if(v<0) v=0;
        if(v>4000) v= 4000;
        p->setValue(v);
        //p->update();
    }

    foreach(QCheckBox *p,cb)
    {
        p->setChecked(s[0]->getValue16(p->objectName().right(p->objectName().size()-3)));
    }
    update();
}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{

    int fw=3,pr=0;
    QString tag;
    double d;

    IoNetClient &s=*qobject_cast<IoNetClient*>(sender());

    QVector<QLineEdit*> le;

    if(sender()->objectName()=="difuz")
    {
        le=d1_VC;
    }
    else if(sender()->objectName()=="satur")
    {
        le=s1_VC;
        xC20->setToolTip(QString(tr("Частота: %1Гц\nСтрум: %2А\nНапруга: %3В"))
                         .arg(s[0]->getValueScaled("V_49"),0,'f',1)
                .arg(s[0]->getValueScaled("V_48"),0,'f',1)
                .arg(s[0]->getValueScaled("V_47"),0,'f',1));
        // рекомендації по відкачці з холодного
        double rec=0.0;
        foreach(QLineEdit *p,ss_SUM)
        {
            rec+=p->text().toDouble();
        }
        ss_REC->setText(QString("%1").arg(rec,3,'f',0));
    }
    else if(sender()->objectName()=="viparka")
    {
        le=v1_VC;
    }


    foreach(QLineEdit *p,le)
    {
        tag=p->objectName().right(p->objectName().size()-3);
        d=fabs(s[0]->scaleFull(tag)-s[0]->scaleZero(tag));
        if(d<=10.0)
        {
            fw=5;
            pr=2;
        }
        else if(d<=50.0)
        {
            fw=4;
            pr=1;
        }
        else
        {
            fw=3;
            pr=0;
        }
        p->setText(QString("%1").arg(s[0]->getValueScaled(tag),fw,'f',pr));
    }


}

/* Дана функція для виводу графіків на тренди оперує файлом наступного змісту
 * i j t t t t t t t
 * i індекс IoNet
 * j індекс IoDev
 * t t t t - список тегів
 * всі поля мають бути розділені табуляторами
 */
void Mnemo::updateTrendChart() // поновлення графіків
{

    QVector<double> v;
    QFile ttagList(":/text/trclist.txt");

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


void Mnemo::slotCallReg()
{
    QString tmp=sender()->objectName().split('_')[0];
    if(tmp=="d1")
    {
        QCoreApplication::setApplicationName("difuz");
        RpanelReg p(*DIFUZ[0],sender()->objectName().right(2).toInt()-1,this,":/text/reg_d1.txt","trend");
        p.exec();
    }
    else if(tmp=="s1")
    {
        QCoreApplication::setApplicationName("satur");
        RpanelReg p(*SATUR[0],sender()->objectName().right(2).toInt()-1,this,":/text/reg_s1.txt","trend");
        p.exec();
    }
    else if(tmp=="v1")
    {
        QCoreApplication::setApplicationName("viparka");
        int idX=sender()->objectName().right(2).toInt()-1;
        if(idX>15) idX--;
        RpanelReg p(*VIPARKA[0],idX,this,":/text/reg_v1.txt","trend");
        p.exec();
    }
}

void Mnemo::slotCallVbCtrl()
{
    if(sender()->objectName().left(2)=="s1")
    {
        dlgVbCtrl p(*SATUR[0],sender()->objectName().split("_").at(2).toInt(),this);
        p.exec();
    }
    else if(sender()->objectName().left(2)=="d1")
    {
        dlgVbCtrl p(*DIFUZ[0],sender()->objectName().split("_").at(2).toInt(),this);
        p.exec();

    }

}


void Mnemo::slotCallPumpCtrl()
{
    QStringList v;
        v << "V_15"          // параметер
          << "X_08"          // сигнал робота
          << "Y_07"          // Управління
          << "AM_PUMP_01"    // режим Авт-Руч
          << "min_V_17"      // мінімум
          << "max_V_17"      // максимум
          << "X_07";         // перемикач місцеве-дистанційне

    dlgPumpCtrl p(*SATUR[0],v,this);
    p.exec();
}


void Mnemo::slotCallAmVater()
{
    QStringList tags;
    tags << "Y_01" << "X_06" << "V_06" << "V_01" << "SPV_01" << "DSPV_01" << "SPV_02" << "DSPV_02" << "AMV_01";
    RAmVater p(*VIPARKA[0],tags,this);
    p.exec();
}



void Mnemo::slotShowMassView()
{
    DlgMassView p(*DIFUZ[0],this);
    p.exec();

}

void Mnemo::slotFqDrvUpdate()
{
    for(int i=0;i<fd1_BN.size();++i)
    {
        bool Link,Run,Alarm;
        Link=FQDRV[0]->getValue16(QString("Link%1").arg(i+1,4,10,QChar('0')));
        Run=FQDRV[0]->getValue16(QString("Run%1").arg(i+1,4,10,QChar('0')));
        Alarm=FQDRV[0]->getValue16(QString("Alarm%1").arg(i+1,4,10,QChar('0')));
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
                                .arg(FQDRV[0]->getValueScaled(QString("V%1").arg(i+1,4,10,QChar('0'))))
                                .arg(FQDRV[0]->getValueScaled(QString("V%1").arg(i+21,4,10,QChar('0'))))
                                .arg(FQDRV[0]->getValueScaled(QString("V%1").arg(i+41,4,10,QChar('0'))))
                                .arg(FQDRV[0]->getValueScaled(QString("V%1").arg(i+41,4,10,QChar('0')))
                                *FQDRV[0]->getValueScaled(QString("V%1").arg(i+21,4,10,QChar('0')))
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

    // частотник на подачі соку на утфельний підігрівач.

    QVector<QPushButton*> ipBn;
    ipBn << m_d->findChild<QPushButton*>("d1_xC_10")
         << m_v->findChild<QPushButton*>("v1_xC_16");

    for(int i=2;i<4;++i)
    {
        QPushButton *d1_xC_10=ipBn[i-2];
        if(d1_xC_10)
        {
            QString prefix="pr";
            bool Link,Run,Alarm;
            quint16 State;
            State=FQDRV[i]->getValue16("State");
            Link=true; //FQDRV[2]->getValue16(QString("Link%1").arg(i+1,4,10,QChar('0')));
            Run=State&0x1;
            Alarm=State&0x8000;
            if(Link)
            {
                if(Alarm)
                {
                    d1_xC_10->setIcon(QIcon(QString(":/%1_36x34_1.png").arg(prefix)));
                    d1_xC_10->setToolTip(tr("Аварійна зупинка"));
                }
                else
                {
                    if(Run)
                    {
                        d1_xC_10->setIcon(QIcon(QString(":/%1_36x34_2.png").arg(prefix)));
                        d1_xC_10->setToolTip(QString(tr("Робота\n"
                                                "Частота: %1Гц\n"
                                                "Струм: %2А\n"
                                                "Напруга: %3В\n"
                                                         "Потужність: %4кВт")
                                .arg((double)FQDRV[i]->getValue16 ("V0001")/100.0)
                                .arg((double)FQDRV[i]->getValue16 ("V0002")/10.0))
                                .arg(FQDRV[i]->getValue16 ("V0003")/10.0)
                                .arg(State,4,16,QChar('0'))
                                );

                    }
                    else
                    {
                        d1_xC_10->setIcon(QIcon(QString(":/%1_36x34_3.png").arg(prefix)));
                        d1_xC_10->setToolTip(QString(tr("Привод зупинено %1")).arg(State,4,16,QChar('0')));
                    }
                }
            }
            else
            {
                d1_xC_10->setIcon(QIcon(QString(":/%1_36x34_0.png").arg(prefix)));
                d1_xC_10->setToolTip(tr("Відсутній зв’язок"));
            }
        }

    }
}


void Mnemo::slotSendSpPresh(int v)
{
    QCoreApplication::setApplicationName("viparka");
    QSettings s;
    s.setValue("spPresh",v);

    // відправка по мережі
    QUdpSocket sockS;
    sockS.writeDatagram(QString("spPresh=%1\n").arg(v).toLatin1(),QHostAddress::Broadcast,8845);
    // запис лога в файл
    QFile f("spPresh.log");
    f.open(QIODevice::Append);
    f.write(QString("%1 spPresh = %2\n").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"))
            .arg(v).toLatin1());
    f.close();


}

void Mnemo::slotSpPreshSend()
{
    while (sock->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(sock->pendingDatagramSize());
        QHostAddress addr;
        sock->readDatagram(datagram.data(),datagram.size(),&addr);
        if(datagram.trimmed()=="?spPresh")
        {
            QUdpSocket sockS;
            sockS.writeDatagram(QString("spPresh=%1\n").arg(spPresh->value()).toLatin1(),QHostAddress::Broadcast,8845);

        }
    }
}



