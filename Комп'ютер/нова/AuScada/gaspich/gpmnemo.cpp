
#include "gpmnemo.h"
#include "mnemo_gp.h"

#include <aulib.h>
#include <trendchar/trendchart.h>

#include <QVBoxLayout>
#include <QPalette>
#include <QTimer>
#include <QUiLoader>
#include <QFile>
#include <QDir>

#include <QPixmap>


#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>
#include <QSlider>

#include "dialogsetup.h"
#include "setupgpi4.h"

#include "dialogsovalka.h"
#include "dlgerrlogview.h"


#define GPI4    QStringLiteral("plcgpi4")
#define DIFUZ   QStringLiteral("plcdifuz")
#define SATUR   QStringLiteral("plcsatur")
#define VIPARKA QStringLiteral("plcvipar")
#define ROU     QStringLiteral("plcrou")

GpMnemo::GpMnemo(AuBase &source, QWidget *p) : QWidget(p), src(source)
{
    setObjectName("GpMnemo");

    gaspich=src[GPI4].index();
    satur=src[SATUR].index();
    vipar=src[VIPARKA].index();

    connect(&src[gaspich],&AuPlc::updateData,this,&GpMnemo::updateData);
    connect(&src[satur],&AuPlc::updateData,this,&GpMnemo::updateDataSatur);
    //connect(&src[vipar],&AuPlc::updateData,this,&GpMnemo::updateDataVipar);


    QStringList lTexts;
    lTexts << tr("Тракт газу")
           << tr("Тракт вапна");

    QGridLayout *bxLayout =new QGridLayout;
    bxLayout->setMargin(0);
    bxLayout->setVerticalSpacing(1);
    bxLayout->setHorizontalSpacing(1);

    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);



    //QUiLoader *uil_gp= new QUiLoader(this);
    //QFile fileui_gp(":/gaspich/gpmnemo.ui");
    //m_gp=uil_gp->load(&fileui_gp); // завантажити файл із мордою

    m_gp= new mnemo_gp(this);

    QFile ttagList(":/text/gplist.txt");
    ttagList.open(QIODevice::ReadOnly);

    //setStyleSheet(m_gp->styleSheet());

    if(m_gp)
    {
        bxLayout->addWidget(m_gp,0,0,1,2);

        for(int i=0;i<2;++i)
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
            // frame->parentWidget()->layout()->addWidget(drawLegend(tags,t,src[gaspich]));
            bxLayout->addWidget(drawLegend(tags,t,src[gaspich]) ,3,i);

        }
    }
    ttagList.close();


//    resize(m_d->size().width()+m_s->size().width()+m_v->size().width(),960);
    resize(m_gp->size().width(),900);
    setLayout(bxLayout);


    QVector<QWidget*> sGpMnemo;
//    sGpMnemo << m_d << m_s << m_v;
    sGpMnemo << m_gp ;

    foreach(QWidget* m,sGpMnemo)
    {
        QString tmp; // цюди буду записувати що куди складувати.
        // пройтися на предмет елементів управління
        foreach(QLineEdit *p,m->findChildren<QLineEdit*>())
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="g1")
                g1_VC << p;
            else if(tmp=="s1")
                s1_VC << p;
            //else if(tmp=="v1")
            //    v1_VC << p;
        }

        foreach(QProgressBar *p,m->findChildren<QProgressBar*>())
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="g1")
                g1_lV << p;
            else if(tmp=="s1")
                s1_lV << p;
            //else if(tmp=="v1")
            //    v1_lV << p;
        }

        foreach(auto *p,m->findChildren<AuLamp*>(QRegularExpression("^(g|s)1_")))
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="g1")
                g1_Cb << p;
            else if(tmp=="s1")
                s1_Cb << p;
            //else if(tmp=="v1")
            //    v1_Cb << p;

        }
        //qDebug() << g1_Cb;

        foreach(QPushButton *p,m->findChildren<QPushButton*>())
        {

            if(p->objectName().split('_').size()>1)
            {
            tmp=p->objectName().split('_')[0];

            if(p->objectName().split('_')[1].left(2) =="xC") // якщо кнопки регуляторів
            {
                if(tmp=="g1")
                    g1_bC << p;
                else if(tmp=="s1")
                    s1_bC << p;
                // else if(tmp=="v1")
                //    v1_bC << p;
                connect(p,SIGNAL(clicked()),this,SLOT(slotCallReg()));
                //qDebug() << p->objectName();
            }
            }
        }

    }
    /*Всілякі ксатомні штуки*/

    foreach(QSlider *p,m_gp->findChildren<QSlider*>())
        s1_SP << p;



    State=m_gp->findChild<QLineEdit*>("gg_State");

    lsState << tr("Виключено") // 0
        << tr("Запуск") // 1
        << tr("Тест тари палива") // 2
        << tr("Паливо сигнал") // 3
        << tr("Паливо завантаження") // 4
        << tr("Паливо витримка") // 5
        << tr("Тест тари каменю") // 6
        << tr("Камінь сигнал") // 7
        << tr("Камінь завантаження") // 8
        << tr("Камінь витримка") // 9
        << tr("Очікування вивантаження шихти") // 10
        << tr("Вивантаження шихти") // 11
        << tr("Вивантаження шихти") // 12
        << tr("Фініш"); // 13

    QTimer *t = new QTimer(this);
    t->setInterval(5000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrendChart()));

    {
        QPushButton *p= m_gp->findChild<QPushButton*>("bnSetup");
        if(p)
            connect(p,SIGNAL(clicked()),this,SLOT(slotSetup()));
    }

    connect(m_gp->findChild<QPushButton*>("bnControl"),SIGNAL(clicked()),this,SLOT(slotControl()));

    //connect(m_gp->findChild<QPushButton*>("g1_xC0003"),SIGNAL(clicked()),this,SLOT(slotCallSocalka()));

    QFile errMsg(":/text/gperrormsg.txt");
    errMsg.open(QIODevice::ReadOnly);

    for(;!errMsg.atEnd();)
    {
        gpErrMsg << QString::fromUtf8(errMsg.readLine().trimmed().split('\t').at(1));
    }

    errMsg.close();
    lastErr=-1;
    connect(m_gp->findChild<QPushButton*>("bnErrorLogView"),SIGNAL(clicked()),this,SLOT(slotShowErrorLog()));

    updateToolTips();


}


GpMnemo::~GpMnemo()
{

}


QFrame* GpMnemo::drawLegend(QString tags, TrendChart *trChar, AuPlc &plc)
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
void GpMnemo::updateToolTips()
{
    QString tag;

    // заповнити поля toolTip

    foreach(QLineEdit *p,g1_VC)
    {
        tag=p->objectName().split('_').at(1);
        SAFE(p->setToolTip(src[gaspich][tag].text()));
    }

    // заповнити поля toolTip
    foreach(QProgressBar *p,g1_lV)
    {
        //p->setToolTip(s[v]->getText()[p->objectName().right(5)]);
        tag=p->objectName().right(5);
        SAFE(p->setToolTip(src[gaspich][tag].text()));
    }

    foreach (auto *p, g1_Cb) {
        //p->setToolTip(s[v]->getText()[p->objectName().right(5)]);
        tag=p->objectName().split('_').at(1);
        SAFE(p->setToolTip(src[gaspich][tag].text()));
    }

    // заповнити поля toolTip
    foreach(QLineEdit *p,s1_VC)
    {
        tag=p->objectName().right(4);
        SAFE(p->setToolTip(src[satur][tag].text()));
    }

    foreach(QProgressBar *p,s1_lV)
    {
        tag=p->objectName().right(4);
        //qDebug() << p->objectName() << tag << i;
        SAFE(p->setToolTip(src[satur][tag].text()));
    }

/*
    // заповнити поля toolTip
    foreach(QLineEdit *p,v1_VC)
    {
        tag=p->objectName().right(p->objectName().size()-3);
        SAFE(p->setToolTip(src[vipar][tag].text()));
    }

    foreach(QProgressBar *p,v1_lV)
    {
        tag=p->objectName().right(p->objectName().size()-3);
        SAFE(p->setToolTip(src[vipar][tag].text()));
    }
*/

}

void GpMnemo::updateData()
{

    QString tag;
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    updatePlc(s,g1_VC,g1_lV,g1_Cb);

    QLabel *pY9=m_gp->findChild<QLabel*>("g1_Y0009");
    bool  Y0009,Y0010,Y0011;

    SAFE(Y0009=s[QStringLiteral("Y0009")]);
    SAFE(Y0010=s[QStringLiteral("Y0010")]);
    SAFE(Y0011=s[QStringLiteral("Y0011")]);

    //qDebug() <<  Y0009<< Y0010<< Y0011;



    if(Y0011)
    {
        if(Y0009)
            pY9->setPixmap(QPixmap(":/gaspich/mnemo/arrow_up_big_on.png"));
        if(Y0010)
            pY9->setPixmap(QPixmap(":/gaspich/mnemo/arrow_down_big_on.png"));
    }
    else
    {
        if(Y0009)
            pY9->setPixmap(QPixmap(":/gaspich/mnemo/arrow_up_small_on.png"));
        else if(Y0010)
            pY9->setPixmap(QPixmap(":/gaspich/mnemo/arrow_down_small_on.png"));
        else
            pY9->setPixmap(QPixmap());
    }


     foreach(QLineEdit *p,m_gp->findChildren<QLineEdit*>(QRegularExpression("^g2_")))
     {
         tag=p->objectName().split('_').at(1);
         //SAFE( quint32 v=s[tag]/1000;
         //p->setText(QString::number(v)));
         SAFE(p->setText(s[tag]));
     }

     SAFE(m_gp->findChild<QLineEdit*>("ga_Nc")
        ->setText(s[QStringLiteral("Nc")]));

     QLineEdit *p=m_gp->findChild<QLineEdit*>("ga_State");
     int State=0,Statecl=0,Error=0;
     SAFE(State=s[QStringLiteral("State")]);
     SAFE(Statecl=s[QStringLiteral("Statecl")]);
     SAFE(Error=s[QStringLiteral("Error")]);

     QPalette pal;

     if(lastErr !=Error)
     {
        p->setToolTip(gpErrMsg[Error]);

        if(Error==0)
        {
            pal.setColor(QPalette::Base,Qt::white);
        }
#ifndef WIN32
        else
        {


            QFile errLog( QDir::homePath()+"/gperror.log");
            if(errLog.open(QIODevice::Append))
            {
                errLog.write(QString("%1\t%2\n").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
                             .arg(gpErrMsg[Error]).toUtf8());
                errLog.close();
            }

            pal.setColor(QPalette::Base,Qt::red);
        }
#else
        else
        {
            pal.setColor(QPalette::Base,Qt::red);
        }
#endif
        p->setPalette(pal);

     }

     lastErr=Error;



     if(Error>0)
     {
         p->setText(tr("Аварійне блокування"));
     }
     else if(s[QStringLiteral("X0019")]!=0)
     {
         p->setText(tr("Ручний режим"));
     }
     else if(s[QStringLiteral("X0020")]!=0)
     {

         if(Statecl>0 && Statecl<4)
         {
             p->setText(tr("Завантаження вугілля"));
         }
         else
         {

         switch (State) {
         case -1:
             p->setText(tr("Аварійне блокування!"));

             break;
         case 0:
             p->setText(tr("Очікування"));


             break;
         case 1:
             p->setText(tr("Запуск"));
             break;
         case 2:
             p->setText(tr("Завантаження вугілля"));
             break;
         case 3:
         case 4:
         case 5:
         case 6:
             p->setText(tr("Завантаження шихти"));
             break;

         case 7:
             p->setText(tr("Підйом ляди"));
             break;

         case 8:
         case 9:
             p->setText(tr("Підйом вагонетки"));
             break;
         case 10:
             p->setText(tr("Висипання шихти"));
             break;
         case 11:
         case 12:
             p->setText(tr("Опускання вагонетки"));
             break;
         case 13:
             p->setText(tr("Очікування підйому"));
             break;

         case 14:
             p->setText(tr("Очікування опускання"));
             break;

         default:
             p->setText(tr("Невизначено"));
             break;
         }
         }
     }
     else
     {
         p->setText("Виключено");
     }

     m_gp->findChild<QLineEdit*>("ga_cntX0018")
        ->setText(s[QStringLiteral("cntX0018")]);
     QPalette pal1;
     pal1.setColor(QPalette::Base,s[QStringLiteral("almX0018")].value()==0?Qt::white:Qt::red);
     p->setPalette(pal);


//---------------------------------------------------------
     SAFE(
     m_gp->findChild<QLineEdit*>("ga_dozGcl")
        ->setText(s[QStringLiteral("dozGcl")]);
    )
     SAFE(
     m_gp->findChild<QLineEdit*>("ga_tmpGcl")
        ->setText(s[QStringLiteral("tmpGcl")]);
     )

}

void GpMnemo::updateDataSatur()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    updatePlc(s,s1_VC,s1_lV,s1_Cb);

}

/*
void GpMnemo::updateDataVipar()
{
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    updatePlc(s,v1_VC,v1_lV,v1_Cb);

}
*/

/* Дана функція для виводу графіків на тренди оперує файлом наступного змісту
 * i j t t t t t t t
 * i індекс IoNet
 * j індекс AuPlc
 * t t t t - список тегів
 * всі поля мають бути розділені табуляторами
 */
void GpMnemo::updateTrendChart() // поновлення графіків
{

    QVector<double> v;
    QFile ttagList(":/text/gplist.txt");

    if(ttagList.open(QIODevice::ReadOnly))
    {
        int i=0;
        for(;!ttagList.atEnd();)
        {
            QStringList tags=QString::fromLatin1(ttagList.readLine()).trimmed().split(QRegularExpression("\\s+")); // прочитати рядок
            QString plcName =tags[0]; // перший елемент - назва контролера
            tags.pop_front();

            if(tags.size()>0) // якщо там щось є тоді
            {
                v.clear();
                SAFE(
                foreach(QString tag,tags)
                    v<< src[plcName][tag].valueReal();
                )
                trc[i++]->addPoint(v);
            }

        }
        ttagList.close();
    }
    //qDebug() << "-";

}



void GpMnemo::slotCallReg()
{

    QString tmp=sender()->objectName().split('_')[0];
    if(tmp=="g1")
    {
        AuPanelReg p(src,gaspich,0,":/text/reg_g1.txt",this);
        p.exec();
    }
    else if(tmp=="s1")
    {
        AuPanelReg p(src,satur,0,":/text/reg_s1.txt",this);
        p.exec();
    }
}


/*
void GpMnemo::slotSetAM(int v )
{
    //GPI4[0]->sendValue("AM",qint16(-v));
}
*/


void GpMnemo::slotSetup()
{
    DialogSetup p(src[GPI4],this);
    p.exec();
}


void GpMnemo::slotControl()
{
    SetupGpi4 p(src[GPI4],this);
    p.exec();
}


void GpMnemo::slotCallSocalka()
{
    //DialogSovalka p(src[GPI4],this);
    //p.exec();

}

void GpMnemo::slotShowErrorLog()
{
       DlgErrLogView p(this);
       p.exec();

}
void GpMnemo::updatePlc(AuPlc &plc, QVector<QLineEdit *> &le, QVector<QProgressBar *> &pb, QVector<AuLamp *> &cb)
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
        //qDebug() << p->objectName() << tag << plc[tag].valueReal() ;

        SAFE(p->setValue(plc[tag].valueReal()));
        SAFE(p->setToolTip(plc[tag]));
        p->update();
    }

    foreach(auto *p,cb) // вивести лампочки
    {
        tag=p->objectName().remove(0,3);
        // qDebug() << p;
        SAFE(p->setChecked(plc[tag].value()!=0));
        //p->setChecked(plc[tag].value()!=0);
    }

}
