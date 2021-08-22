
#include "mnemo.h"
#include "ui_mnemo.h"

#include <aulib.h>

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
#include "trendchar/trendchart.h"


#include "dlgvbctrl.h"
#include "dlgpumpctrl.h"

#include "amVater.h"
#include "dlgmassview.h"
#include "dlgflokctl.h"

#define DIFUZ   QStringLiteral("plcdifuz")
#define SATUR   QStringLiteral("plcsatur")
#define VIPARKA QStringLiteral("plcvipar")
#define ROU     QStringLiteral("plcrou")
#define GPI4    QStringLiteral("plcgpi4")

Mnemo::Mnemo(AuBase &source, QWidget *p) : QWidget(p), src(source)
{
    setObjectName("Mnemo");

    difuz=src["plcdifuz"].index();
    satur=src["plcsatur"].index();
    vipar=src["plcvipar"].index();
    fqdrv=src["plcfdmom"].index();


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

    QGridLayout *bxLayout =new QGridLayout(this);
    bxLayout->setMargin(0);
    bxLayout->setVerticalSpacing(1);
    bxLayout->setHorizontalSpacing(1);


    QUiLoader *uil_d= new QUiLoader(this);
    QFile fileui_d(":/mnemo/mnemo_d.ui");
    m_d=uil_d->load(&fileui_d); // завантажити файл із мордою

    // це потрібно для того щоб завантажити графіки з бази
    QFile ttagList(":/text/trclist.txt");
    ttagList.open(QIODevice::ReadOnly);

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

            QString tags=ttagList.readLine().trimmed();
            src.loadTrendChartData(t,tags);

            bxLayout->addWidget(drawLegend(tags,t,src[DIFUZ]) ,3,i);



        }

        d1_Y_01=m_d->findChild<QPushButton*>("d1_Y_01");
        connect(d1_Y_01,SIGNAL(clicked()),this,SLOT(slotCallVbCtrl()));

        vodaSum=m_d->findChild<QLineEdit*>("vodaSum");


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

            QString tags=ttagList.readLine().trimmed();
            src.loadTrendChartData(t,tags);
            bxLayout->addWidget(drawLegend(tags,t,src[SATUR]) ,3,i);

        }

        s1_bleding
        << m_s->findChild<QPushButton*>("s1_Y_01")
        << m_s->findChild<QPushButton*>("s1_Y_02")
        << m_s->findChild<QPushButton*>("s1_Y_03")
        << m_s->findChild<QPushButton*>("s1_Y_04")
        << m_s->findChild<QPushButton*>("s1_Y_05")
        << m_s->findChild<QPushButton*>("s1_Y_06")
        << m_s->findChild<QPushButton*>("s1_Y_10");

        foreach(QPushButton* p,s1_bleding)
            connect(p,SIGNAL(clicked()),this,SLOT(slotCallVbCtrl()));

        QPushButton *p=m_s->findChild<QPushButton*>("s1_cP_01");
        connect(p,SIGNAL(clicked()),this,SLOT(slotCallPumpCtrl()));

        apLe << m_s->findChild<QLineEdit*>("Ai5_i");
        apPb << m_s->findChild<QProgressBar*>("pAi5_i");


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

            QString tags=ttagList.readLine().trimmed();
            src.loadTrendChartData(t,tags);
            bxLayout->addWidget(drawLegend(tags,t,src[VIPARKA]) ,3,i);

        }

        foreach(QSlider *p,m_v->findChildren<QSlider*>())
            v1_SP << p;
    }

    // далі графіків не створюю, треба закрити файл
    ttagList.close();

    resize(m_d->size().width()+m_s->size().width()+m_v->size().width(),980);
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
    connect(t,&QTimer::timeout,this,&Mnemo::updateTrendChart);


    //кнопка управління подачею аміачної води
    connect(m_s->findChild<QPushButton*>("v1_X_06"),SIGNAL(clicked()),this,SLOT(slotCallAmVater()));
    connect(m_d->findChild<QPushButton*>("bn_MassView"),SIGNAL(clicked()),this,SLOT(slotShowMassView()));


    QStringList bnNames ;
    bnNames << "d1_xC_09"
                << "s1_xC_15"
                << "f1_xC_00"
                << "d1_xC_06"
                << "s1_xC_17"
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
        lastIconNames << "";
    }

    xC05=m_s->findChild<QPushButton*>("s1_xC_05");
    xC20=m_s->findChild<QPushButton*>("s1_xC_20");

// передача завдання на тиск
    spPresh=m_v->findChild<QSpinBox*>("spPresh");
    /*
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
    */


    // Рекомендації по відкачці з холодного
    ss_REC=m_s->findChild<QLabel*>("ss_REC");
    ss_SUM << m_d->findChild<QLineEdit*>("d1_V_15")
           << m_s->findChild<QLineEdit*>("s1_V_03")
           << m_s->findChild<QLineEdit*>("s1_V_04")
           << m_s->findChild<QLineEdit*>("s1_V_06")
           << m_s->findChild<QLineEdit*>("s1_V_07")
           << m_s->findChild<QLineEdit*>("s1_V_09");


    V10=m_v->findChild<QLineEdit*>("va_V_10");

    // кнопки для частотників підключених по езернету
    bnFqMbEth[&src["fddiff1"]] = m_d->findChild<QPushButton*>("d1_xC_10");
    bnFqMbEth[&src["fdvip1"]] = m_v->findChild<QPushButton*>("v1_xC_16");

    updateToolTips();

    frDrv=&src[fqdrv];

    // цей код перенесено сюди з початку програми тому що інакше він починає виводити дані в неістуючі елементи
    connect(&src[difuz],&AuPlc::updateData,this,&Mnemo::updateDifuz);
    connect(&src[satur],&AuPlc::updateData,this,&Mnemo::updateSatur);
    connect(&src[vipar],&AuPlc::updateData,this,&Mnemo::updateVipar);
    connect(&src[fqdrv],&AuPlc::updateData,this,&Mnemo::updateFqdrv);

    connect(&src["tmaob"],&AuPlc::updateData,this,&Mnemo::updateApOb);

    // частотники підключені по Езернету
    connect(&src["fddiff1"],&AuPlc::updateData,this,&Mnemo::updateFqMb);
    connect(&src["fdvip1"],&AuPlc::updateData,this,&Mnemo::updateFqMb);

}


Mnemo::~Mnemo()
{

}

QFrame* Mnemo::drawLegend(QString tags, TrendChart *trChar, AuPlc &plc)
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
                text.truncate(20);

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


void Mnemo::updateToolTips()// заповнити поля toolTip
{
    QString tag;
    // дифузія
    foreach(QLineEdit *p,d1_VC)
    {
        tag=p->objectName().remove(0,3); // приготувати ім'я тега
        p->setToolTip(src[difuz][tag].text());
    }

    // заповнити поля toolTip
    foreach(QProgressBar *p,d1_lV)
    {
        tag=p->objectName().remove(0,4); // приготувати ім'я тега
        p->setToolTip(src[difuz][tag].text());
    }

    // сатурація
    foreach(QLineEdit *p,s1_VC)
    {
        tag=p->objectName().remove(0,3); // приготувати ім'я тега
        p->setToolTip(src[satur][tag].text());
    }

    // заповнити поля toolTip
    foreach(QProgressBar *p,s1_lV)
    {
        tag=p->objectName().remove(0,4); // приготувати ім'я тега
        p->setToolTip(src[satur][tag].text());
    }

    // випарка
    foreach(QLineEdit *p,v1_VC)
    {
        tag=p->objectName().remove(0,3); // приготувати ім'я тега
        p->setToolTip(src[vipar][tag].text());
    }

    // заповнити поля toolTip
    foreach(QProgressBar *p,v1_lV)
    {
        tag=p->objectName().remove(0,4); // приготувати ім'я тега
        p->setToolTip(src[vipar][tag].text());
    }

    // ще щось

}

void Mnemo::updateDifuz()
{
    // qDebug() << "updateDifuz()";
    AuPlc &s=*qobject_cast<AuPlc*>(sender());

    updatePlc(s,d1_VC,d1_lV,d1_Cb);

    SAFE( d1_Y_01->setIcon(QIcon(QPixmap(s[QStringLiteral("Y_01")]!=0
                                     ?":/pict/lib/valve_green_h_20x32.png":":/pict/lib/valve_off_h_20x32.png")));
    )
    {
        QCheckBox *p=m_d->findChild<QCheckBox*>("d1_en_p_01");
        SAFE(
        p->setIcon(QIcon(QPixmap(s[QStringLiteral("en_p_01")]!=0
                                         ?":/pict/lib/valve_green_20x32.png":":/pict/lib/valve_off_20x32.png")));
        )
    }

    double vS=0;
    SAFE(
        vS=s["V_14"]+s["V_29"];
                )
    vodaSum->setText(QString::number(vS,'f',0));
}

void Mnemo::updateSatur()
{
    QString tag;
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    updatePlc(s,s1_VC,s1_lV,s1_Cb);

            foreach(QPushButton *p,s1_bleding)
            {
                //qDebug() << p->objectName();
                tag=p->objectName().right(p->objectName().size()-3);
                SAFE(
                p->setIcon(QIcon(QPixmap(s[tag]!=0
                                                 ?":/pict/lib/valve_green_20x32.png":":/pict/lib/valve_off_20x32.png")));
                        )
            }
            SAFE(
            if(s[QStringLiteral("X_09")]==0)
            {
                xC05->show();
                xC20->hide();
            }
            else
            {
                xC05->hide();
                xC20->show();
            }
            )



        // Частотник газодувки
        SAFE(
        xC20->setToolTip(QString(tr("Частота: %1Гц\nСтрум: %2А\nНапруга: %3В"))
                .arg(s[QStringLiteral("V_49")],0,'f',1)
                .arg(s[QStringLiteral("V_48")],0,'f',1)
                .arg(s[QStringLiteral("V_47")],0,'f',1));
        )

// рекомендації по відкачці з холодного
        double rec=0.0;
        foreach(QLineEdit *p,ss_SUM)
        {
            rec+=p->text().toDouble();
        }
        ss_REC->setText(QString("%1").arg(rec,3,'f',0));


        SAFE()
}

void Mnemo::updateVipar()
{
    QString tag;
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    updatePlc(s,v1_VC,v1_lV,v1_Cb);

    foreach(QSlider*p,v1_SP)
    {
        tag=p->objectName().right(p->objectName().size()-3);
        SAFE(p->setValue(s[tag].valueReal()));
    }

    {
        QPushButton *p=m_s->findChild<QPushButton*>("v1_X_06");
        SAFE(p->setIcon(QIcon(QPixmap(s[QStringLiteral("Y_01")]!=0
                                     ?":/pict/lib/valve_green_20x32.png":":/pict/lib/valve_off_20x32.png"))));
    }

    {
        QLineEdit *p = m_s->findChild<QLineEdit*>("va_V_41");
        SAFE(p->setText(s[QStringLiteral("V_41")]));
    }
    {
        QLineEdit *p = m_s->findChild<QLineEdit*>("va_V_16");
        SAFE(p->setText(s[QStringLiteral("V_16")]));

    }
    {
        QLineEdit *p = m_v->findChild<QLineEdit*>("va_V_52");
        SAFE(p->setText(s[QStringLiteral("V_52")]));
    }

    // концентрація сиропу на виході
    //double r=1.0;
    //SAFE(r+=s[QStringLiteral("V_10")]/(30.0*40.0));
    //r=120.0*(1.0-1.0/r)-2.0;
    V10->setText(s[QStringLiteral("V_10")]);


}

void Mnemo::updatePlc(AuPlc &plc, QVector<QLineEdit *> &le, QVector<QProgressBar *> &pb, QVector<QCheckBox *> &cb)
{
    QString tag;
    foreach(QLineEdit *p,le) // вивести цифрові значення
    {
        tag=p->objectName().remove(0,3); // приготувати ім'я тега
        SAFE(p->setText(plc[tag]));
    }

    foreach(QProgressBar* p, pb) // вивсети стовбчики
    {
        tag=p->objectName().remove(0,4);
        //qDebug() << tag << "=" << plc[tag].valueReal();
        double v=plc[tag].valueReal();
        if(v>plc[tag].baseScale()) v=plc[tag].baseScale();
        SAFE(p->setValue(v));
    }
    foreach(QCheckBox *p,cb) // вивести лампочки
    {
        tag=p->objectName().remove(0,3);
        SAFE(p->setChecked(plc[tag].value()!=0));
    }
    // update();

}


void Mnemo::updateApOb()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());

    foreach(QLineEdit *p, apLe)
    {
        SAFE(p->setText(s[p->objectName()]));
    }

    foreach(QProgressBar *p,apPb)
    {
        SAFE(p->setValue(s[p->objectName().right(p->objectName().size()-1)]));
    }
}
/* Дана функція для виводу графіків на тренди оперує файлом наступного змісту
 * i j t t t t t t t
 * i індекс IoNet
 * j індекс AuPlc
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
            QStringList tags=QString::fromLatin1(ttagList.readLine()).trimmed().split(QRegExp("\\s+")); // прочитати рядок

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


void Mnemo::slotCallReg()
{
    QString tmp=sender()->objectName().split('_')[0];
    if(tmp=="d1")
    {
        AuPanelReg p(src,difuz,sender()->objectName().right(2).toInt()-1,":/text/reg_d1.txt",this);
        p.exec();
    }
    else if(tmp=="s1")
    {
        int regNo=sender()->objectName().right(2).toInt()-1;
        if(regNo<21)
        {
            AuPanelReg p(src,satur,regNo,":/text/reg_s1.txt",this);
            p.exec();
        }
        else
        {
            QStringList tags;
            if(regNo==21)
            {
                tags << "V_51"
                     << "C_22"
                     << "AM_22"
                     << "KC_22"
                     << "1 сатурація"
                     << "Y_15";
            }
            else
            {
                tags << "V_08"
                     << "C_23"
                     << "AM_23"
                     << "KC_23"
                     << "2 сатурація"
                     << "Y_16";
            }
            DlgFlokCtl dlg(src[satur],tags, this);
            dlg.exec();

        }
    }
    else if(tmp=="v1")
    {
        int idX=sender()->objectName().right(2).toInt()-1;
        if(idX>15) idX--;
        AuPanelReg p(src,vipar,idX,":/text/reg_v1.txt",this);
        p.exec();
    }    

}

void Mnemo::slotCallVbCtrl()
{
    if(sender()->objectName().left(2)=="s1")
    {
        dlgVbCtrl p(src[SATUR],sender()->objectName().split("_").at(2).toInt(),this);
        p.exec();
    }
    else if(sender()->objectName().left(2)=="d1")
    {
        dlgVbCtrl p(src[DIFUZ],sender()->objectName().split("_").at(2).toInt(),this);
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

    dlgPumpCtrl p(src[SATUR],v,this);
    p.exec();
}


void Mnemo::slotCallAmVater()
{

    QStringList tags;
    tags << "Y_01" << "X_06" << "V_06" << "V_01" << "SPV_01" << "DSPV_01" << "SPV_02" << "DSPV_02" << "AMV_01";
    RAmVater p(src[VIPARKA],tags,this);
    p.exec();

}



void Mnemo::slotShowMassView()
{
    DlgMassView p(src[DIFUZ],this);
    p.exec();

}

void Mnemo::updateFqdrv()
{
    AuPlc &s=frDrv[0]; // *qobject_cast<AuPlc*>(sender());
    QString iconName="";

    for(int i=0;i<fd1_BN.size();++i)
    {
        bool Link,Run,Alarm;
        SAFE(
        Link=s[QString("Link%1").arg(i+1,4,10,QChar('0'))];
        Run=s[QString("Run%1").arg(i+1,4,10,QChar('0'))];
        Alarm=s[QString("Alarm%1").arg(i+1,4,10,QChar('0'))];
        )

        if(fd1_BN[i])
        {
            // qDebug() << fd1_BN[i]->objectName();

            QString prefix="pr";
            switch (i) {
            case 0:
            // case 4:
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
                    iconName=QString(":/%1_36x34_1.png").arg(prefix);
                    fd1_BN[i]->setToolTip(tr("Аварійна зупинка"));
                }
                else
                {

                    if(Run)
                    {
                        iconName=QString(":/%1_36x34_2.png").arg(prefix);
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
                        iconName=QString(":/%1_36x34_3.png").arg(prefix);
                        fd1_BN[i]->setToolTip(tr("Привод зупинено"));
                    }

                }

            }
            else
            {
                iconName=QString(":/%1_36x34_0.png").arg(prefix);
                fd1_BN[i]->setToolTip(tr("Відсутній зв’язок"));
            }

            if(lastIconNames[i]!=iconName)
            {
                fd1_BN[i]->setIcon(QIcon(iconName));
            }
            lastIconNames[i]=iconName;


        }
    }

}

void Mnemo::updateFqMb()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());

    QPushButton *d1_xC_10=bnFqMbEth[sender()];

    if(d1_xC_10)
    {
        QString prefix="pr";
        bool Link,Run,Alarm;
        qint16 State;
        State=s["State"].value();
        Link=s.isOk();// [QString("Link%1").arg(i+1,4,10,QChar('0'))];
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
                            .arg((double)s["V0001"]/100.0)
                            .arg((double)s["V0002"]/10.0))
                            .arg((double)s["V0003"]/10.0)
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


void Mnemo::slotSendSpPresh(int v)
{

    QSettings s;
    s.setValue("spPresh",v);

    // відправка по мережі
    QUdpSocket sockS;
    sockS.writeDatagram(QString("spPresh=%1\n").arg(v).toLatin1(),QHostAddress::Broadcast,8845);
    // запис лога в файл
/*
    QFile f("spPresh.log");
    f.open(QIODevice::Append);
    f.write(QString("%1 spPresh = %2\n").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"))
            .arg(v).toLatin1());
    f.close();
*/
}

void Mnemo::slotSpPreshSend() // запит на spPresh
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



