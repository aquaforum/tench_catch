
#include "gpmnemo.h"
// #include "ui_gpmnemo.h"

#include <IoNetClient.h>

#include "trendchart.h"
#include "panelReg.h"
#include <QtGui>

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

// унікальні діалогові вікна

#define GPI4 (*(src[5]))

#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))

GpMnemo::GpMnemo(QVector<IoNetClient*> &source, QWidget *p) : QWidget(p), src(source)
{

    foreach(IoNetClient *s,src)
    {
        connect(s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
        connect(s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
        connect(s,SIGNAL(updateTags(int)),this,SLOT(updateToolTips(int)));
        qDebug() << "gaspich" << s->objectName();
    }

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



    QUiLoader *uil_gp= new QUiLoader(this);
    QFile fileui_gp(":/gaspich/gpmnemo.ui");
    m_gp=uil_gp->load(&fileui_gp); // завантажити файл із мордою

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
        }
    }


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
            else if(tmp=="v1")
                v1_VC << p;
        }

        foreach(QProgressBar *p,m->findChildren<QProgressBar*>())
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="g1")
                g1_lV << p;
            else if(tmp=="s1")
                s1_lV << p;
            else if(tmp=="v1")
                v1_lV << p;
        }

        foreach(QCheckBox *p,m->findChildren<QCheckBox*>(QRegExp("^(g|s)1_")))
        {
            tmp=p->objectName().split('_')[0];
            if(tmp=="g1")
                g1_Cb << p;
            else if(tmp=="s1")
                s1_Cb << p;
            else if(tmp=="v1")
                v1_Cb << p;

        }

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
                else if(tmp=="v1")
                    v1_bC << p;
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

    connect(m_gp->findChild<QPushButton*>("g1_xC0003"),SIGNAL(clicked()),this,SLOT(slotCallSocalka()));

    QFile errMsg(":/text/gperrormsg.txt");
    errMsg.open(QIODevice::ReadOnly);

    for(;!errMsg.atEnd();)
    {
        gpErrMsg << QString::fromUtf8(errMsg.readLine().trimmed().split('\t').at(1));
    }

    errMsg.close();
    lastErr=-1;
    connect(m_gp->findChild<QPushButton*>("bnErrorLogView"),SIGNAL(clicked()),this,SLOT(slotShowErrorLog()));


}


GpMnemo::~GpMnemo()
{

}

void GpMnemo::updateToolTips(int v)
{
    qDebug() << "updateToolTips(" << v  << ")" << "sender" << sender()->objectName();
    IoNetClient &s=*qobject_cast<IoNetClient*>(sender());

    // заповнити поля toolTip
    if(sender()->objectName()=="gaspich")
    {
        foreach(QLineEdit *p,g1_VC)
        {
            p->setToolTip(s[v]->getText()[p->objectName().right(5)]);
        }

        // заповнити поля toolTip
        foreach(QProgressBar *p,g1_lV)
        {
            p->setToolTip(s[v]->getText()[p->objectName().right(5)]);
        }

        foreach (QCheckBox *p, g1_Cb) {
            p->setToolTip(s[v]->getText()[p->objectName().right(5)]);
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
    else if(sender()->objectName()=="vipar")
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

void GpMnemo::updateDataRaw()
{
    IoNetClient &s=*qobject_cast<IoNetClient*>(sender());

    QVector<QProgressBar*> pb;
    QVector<QCheckBox*> cb;


    if(sender()->objectName()=="gaspich")
    {
        pb=g1_lV;
        cb=g1_Cb;

{
            QLabel *p=m_gp->findChild<QLabel*>("g1_Y0009");
        bool  Y0009,Y0010,Y0011;

        Y0009=s[0]->getValue16("Y0009");
        Y0010=s[0]->getValue16("Y0010");
        Y0011=s[0]->getValue16("Y0011");

        if(Y0011)
        {
            if(Y0009)
                p->setPixmap(QPixmap(":/gaspich/mnemo/arrow_up_big_on.png"));
            if(Y0010)
                p->setPixmap(QPixmap(":/gaspich/mnemo/arrow_down_big_on.png"));
        }
        else
        {
            if(Y0009)
                p->setPixmap(QPixmap(":/gaspich/mnemo/arrow_up_small_on.png"));
            else if(Y0010)
                p->setPixmap(QPixmap(":/gaspich/mnemo/arrow_down_small_on.png"));
            else
                p->setPixmap(QPixmap());
        }
}
     foreach(QLineEdit *p,m_gp->findChildren<QLineEdit*>(QRegExp("^g2_")))
     {
         p->setText(QString("%1").arg(s[0]->getValue32(p->objectName().split('_').at(1))/1000));

     }
     {
         QLineEdit *p=m_gp->findChild<QLineEdit*>("ga_Nc");
         p->setText(QString("%1").arg(s[0]->getValue32("Nc")));
     }
     {
         QLineEdit *p=m_gp->findChild<QLineEdit*>("ga_State");
         int State,Statecl,Error;
         State=s[0]->getValue16("State");
         Statecl=s[0]->getValue16("Statecl");
         Error=s[0]->getValue16("Error");

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
         else if(s[0]->getValue16("X0019"))
         {
             p->setText(tr("Ручний режим"));
         }
         else if(s[0]->getValue16("X0020"))
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

/*
         {
             QLineEdit *p=m_gp->findChild<QLineEdit*>("ga_cntX0018");
             p->setText(QString("%1").arg(s[0]->getValue16("cntX0018")));
             QPalette pal;
             pal.setColor(QPalette::Base,s[0]->getValue16("almX0018")==0?Qt::white:Qt::red);
             p->setPalette(pal);
         }
*/

     }

    }
    else if(sender()->objectName()=="satur")
    {
        pb=s1_lV;
        cb=s1_Cb;

        foreach(QSlider*p,s1_SP)
            p->setValue(s[0]->getValueFloat(p->objectName().split("_").at(1)));
    }
    else if(sender()->objectName()=="vipar")
    {
        pb=v1_lV;
        cb=v1_Cb;

    }


    foreach(QProgressBar *p,pb)
    {
        QString tag=p->objectName().right(p->objectName().split('_')[0]=="s1"?4:5);

        double val=s[0]->getValueFloat(tag);
        if(val>4000.0) val=4000.0;
        if(val<0.0) val=0.0;
        p->setValue(val);
        p->setToolTip(QString("%1").arg(s[0]->getValueScaled(tag),0,'f',2));

    }

    foreach(QCheckBox *p,cb)
    {
        QString tag=p->objectName().split('_')[0]=="s1"?p->objectName().right(5):p->objectName().split('_')[1];
        //p->setChecked(s[0]->getValue16(p->objectName().split('_').at(1)));
        p->setChecked(s[0]->getValue16(tag));
    }


}



void GpMnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{


    int fw=3,pr=0;
    QString tag;
    double d;

    IoNetClient &s=*qobject_cast<IoNetClient*>(sender());

    QVector<QLineEdit*> le;

    if(sender()->objectName()=="gaspich")
    {
        le=g1_VC;

        {
            QLineEdit *p=m_gp->findChild<QLineEdit*>("ga_dozGcl");
            p->setText(QString("%1").arg(s[0]->getValueScaled("dozGcl"),0,'f',1));

        }

        {
            QLineEdit *p=m_gp->findChild<QLineEdit*>("ga_tmpGcl");
            p->setText(QString("%1").arg(s[0]->getValueScaled("tmpGcl"),0,'f',1));

        }

    }
    else if(sender()->objectName()=="satur")
    {
        le=s1_VC;
    }
    else if(sender()->objectName()=="vipar")
    {
        le=v1_VC;
    }


    foreach(QLineEdit *p,le)
    {
        if(p->objectName().split('_').size()>2)
        {
            tag=p->objectName().right(4);
        }
        else {
            tag=p->objectName().split('_').at(1);
        }
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
void GpMnemo::updateTrendChart() // поновлення графіків
{

    QVector<double> v;
    QFile ttagList(":/text/gplist.txt");

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



void GpMnemo::slotCallReg()
{

    QString tmp=sender()->objectName().split('_')[0];
    if(tmp=="g1")
    {

            qDebug() << sender()->objectName();
            QCoreApplication::setApplicationName("gaspich");
            RpanelReg p(*GPI4[0],0,this,":/text/reg_g1.txt","trend");
            p.exec();



    }
    else if(tmp=="s1")
    {
        QCoreApplication::setApplicationName("satur");
        RpanelReg p(*SATUR[0],sender()->objectName().right(2).toInt()-1,this,":/text/reg_s1.txt","trend");
        p.exec();
    }
}


void GpMnemo::slotSetAM(int v )
{
    GPI4[0]->sendValue("AM",qint16(-v));
}

void GpMnemo::slotSetup()
{
    DialogSetup p(*GPI4[0],this);
    p.exec();
}


void GpMnemo::slotControl()
{
    SetupGpi4 p(*GPI4[0],this);
    p.exec();
}


void GpMnemo::slotCallSocalka()
{
    DialogSovalka p(*GPI4[0],this);
    p.exec();

}

void GpMnemo::slotShowErrorLog()
{
       DlgErrLogView p(this);
       p.exec();

}
