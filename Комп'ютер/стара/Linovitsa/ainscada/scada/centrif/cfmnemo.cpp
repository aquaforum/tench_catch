
#include "cfmnemo.h"
#include <QUiLoader>
#include <QFile>

#include <trendchart.h>

#include <IoNetClient.h>

#include <QVBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

#include <QPalette>
#include <QList>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>
#include <QSlider>

#include "../mainform.h"

CfMnemo::CfMnemo(IoNetClient &src, QWidget *prn) : QWidget(prn),s(src)
{

//    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    connect(&s,SIGNAL(updateData(int)),this,SLOT(updateDataRaw(int)));

    QGridLayout *bxLayout =new QGridLayout;
    bxLayout->setMargin(0);
    bxLayout->setVerticalSpacing(1);
    bxLayout->setHorizontalSpacing(1);

    QUiLoader *uil_cf= new QUiLoader(this);
    QFile fileui_cf(":/cfmnemo/cfmnemo.ui");
    ui=uil_cf->load(&fileui_cf); // завантажити файл із мордою


    bxLayout->addWidget(ui,0,0,1,1);

    resize(ui->size().width(),900);
    setLayout(bxLayout);


    QVector<QColor> clr;
    clr << Qt::green << Qt::red << Qt::yellow << Qt::cyan << Qt::black << Qt::black << Qt::black << Qt::black;

    for(int i=4;i<6;++i)
    {

        QFrame *trc=ui->findChild<QFrame*>(QString("trcf_%1").arg(i,2,10,QChar('0')));
        TrendChart *t=new TrendChart(trc,600,1);
        trcf<< t;
        t->setColor(clr);
        QVBoxLayout *vbl=new QVBoxLayout(trc);
        vbl->addWidget(t);
        vbl->setMargin(2);
        trc->setLayout(vbl);
    }

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrChart()));
    t->start();

    mMainForm *pMf=qobject_cast<mMainForm*>(prn);
    // кнопки виклику мнемосхем одиночної центрифуги
    foreach(QPushButton *p,ui->findChildren<QPushButton*>(QRegExp("^bn_Cf")) )
    {
        connect(p,SIGNAL(clicked()),pMf,SLOT(showOneMnemo1()));
    }

/*
    connect(ui->findChild<QDoubleSpinBox*>("nQ_k"),SIGNAL(valueChanged(double)),this,SLOT(slotSetnQ(double)) );
    connect(ui->findChild<QSpinBox*>("nTnt_k"),SIGNAL(valueChanged(int)),this,SLOT(slotSetnTnt(int)));
    connect(ui->findChild<QSpinBox*>("nTsgr_k"),SIGNAL(valueChanged(int)),this,SLOT(slotSetnTsgr(int)));

    QTimer::singleShot(3000,this,SLOT(updatenCor()));
*/

    State <<tr("Тест")  //-1
         <<tr("Зупинено")  //0
         <<tr("Запуск")  //1
         <<tr("Промивка сит")  //2
         <<tr("Очікування")  //3
         <<tr("Завантаження")  //4
         <<tr("Фугування")  //5
         <<tr("Пром. сиропом")  //6
         <<tr("Пром. сиропом")  //7
         <<tr("Промивка цукру")  //8
         <<tr("Промивка цукру")  //9
         <<tr("Промивка цукру")  //10
         <<tr("Промивка цукру")  //11
         <<tr("Промивка цукру")  //12
         <<tr("Сушіння")  //13
         <<tr("Гальмування")  //14
         <<tr("Вивантаження")  //15
         <<tr("Синхронізація")  //16
        ;

    // це відноситься до загальних параметрів
    le_cm = findChildren<QLineEdit*>(QRegExp("^le"));
    pb_cm = findChildren<QProgressBar*>();




    cb_co //<< findChildren<QCheckBox*>(QRegExp("^c1_"))
          //  << findChildren<QCheckBox*>(QRegExp("^c2_"))
          //  << findChildren<QCheckBox*>(QRegExp("^c3_"))
            << findChildren<QCheckBox*>(QRegExp("^c4_"))
            << findChildren<QCheckBox*>(QRegExp("^c5_"))
               ;

    le_co //<< findChildren<QLineEdit*>(QRegExp("^c1_(?!State)"))
          //  << findChildren<QLineEdit*>(QRegExp("^c2_(?!State)"))
    //<< findChildren<QLineEdit*>(QRegExp("^c3_(?!State)"))
    << findChildren<QLineEdit*>(QRegExp("^c4_(?!State)"))
    << findChildren<QLineEdit*>(QRegExp("^c5_(?!State)"))
            ;

    State_co //<< ui->findChild<QLineEdit*>("c1_State")
             //<< ui->findChild<QLineEdit*>("c2_State")
             //<< ui->findChild<QLineEdit*>("c3_State")
             << ui->findChild<QLineEdit*>("c4_State")
             << ui->findChild<QLineEdit*>("c5_State")
                ;

}

CfMnemo::~CfMnemo()
{
    delete ui;
}


void CfMnemo::updateDataRaw(int i)
{
    QPalette pal_w,pal_y,pal_r;

    pal_w.setColor(QPalette::Base,Qt::white);
    pal_y.setColor(QPalette::Base,Qt::yellow);
    pal_r.setColor(QPalette::Base,Qt::red);


    if(i==0) // якщо сигнал від контролера всіх центрифуг
    {

        foreach(QProgressBar *v,pb_cm)
        {
            v->setValue(s[0]->getValueFloat(v->objectName().split("_").at(1)));
        }



    }
    else// if(i<3)
    {

        foreach (QCheckBox* p, cb_co[i-1])
        {
            // qDebug() << "Check box update" << i << p->objectName() ;
            p->setChecked(s[i]->getValue16(p->objectName().right(p->objectName().size()-3)));
        }
        foreach(QLineEdit *p,le_co[i-1])
        {
            p->setText(QString("%1").arg(s[i]->getValueFloat(p->objectName().split('_').at(1)),4,'f',0));
            //qDebug() << p->objectName() << s[i]->getValueFloat(p->objectName().split('_').at(1));
        }

        int St=s[i]->getValue16("State")+1;

        if(St>-1 && St < 18 )
            State_co[i-1]->setText(State[St]);
        else
            State_co[i-1]->setText(tr("Невизначено"));
    }


}

void CfMnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{
    // це загальні параметри
    foreach(QLineEdit*p,le_cm)
    {

        p->setText(QString("%1").arg(s[0]->getValueScaled(p->objectName().split("_").at(1)),3,'f',0));
        //qDebug() << p->objectName() << p->text();
    }
}

void CfMnemo::updateTrChart() // поновлюємо дані на графічках
{



    for(int i=1;i<3;++i)
    {
        double sgr;
        if( s[i]->getValue16("Xsgrb") && !s[i]->getValue16("Xsgrz")) //Формування змінної Sgr_t для відображення графіку положення сегрегатора на панелі*)
            sgr=400.0;
        else if( s[i]->getValue16("Xsgrz") && !s[i]->getValue16("Xsgrb") ) // X_sgr_z AND NOT X_sgr_b THEN
            sgr=3600.0;
        else
            sgr=2000.0;
        //qDebug() << "tags size" << s[i]->getTags().keys().size();

        QVector<double> v;
        v<< (double)s[i]->getValue16("Ob") *3.33 << s[i]->getValueFloat("Qall")*40.0 << s[i]->getValue16("Fott")*40 << sgr ;
        v << 0.0 << 0.0;
        trcf[i-1]->addPoint(v);

    }


}



void CfMnemo::slotSetnQ(double v)
{
    s[0]->sendValue("nQ_k",v);
}

void CfMnemo::slotSetnTnt(int v)
{
    s[0]->sendValue("nTnt_k",qint16(v));
}

void CfMnemo::slotSetnTsgr(int v)
{
    s[0]->sendValue("nTsgr_k",qint16(v));
}

void CfMnemo::updatenCor()
{
    {
    QDoubleSpinBox *p=ui->findChild<QDoubleSpinBox*>("nQ_k");
    p->blockSignals(true);
    p->setValue(s[0]->getValueFloat("nQ_k"));
    p->blockSignals(false);
    }
    {
        QSpinBox *p=ui->findChild<QSpinBox*>("nTnt_k");
        p->blockSignals(true);
        p->setValue(s[0]->getValue16("nTnt_k"));
        p->blockSignals(false);
    }

    {
        QSpinBox *p=ui->findChild<QSpinBox*>("nTsgr_k");
        p->blockSignals(true);
        p->setValue(s[0]->getValue16("nTsgr_k"));
        p->blockSignals(false);
    }


}


