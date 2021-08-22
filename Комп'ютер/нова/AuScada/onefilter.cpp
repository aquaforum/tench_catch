// #include <QTime>

#include "onefilter.h"
#include "ui_onefilter.h"

#include <QTimer>

#include <aulib.h>


OneFilter::OneFilter(AuPlc &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::OneFilter),
    n(1),
    s(src)
{
    m_ui->setupUi(this);

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateData()));

    connect(m_ui->bx_Am,SIGNAL(currentIndexChanged(int)),this,SLOT(setAm(int)));


}

OneFilter::~OneFilter()
{
    delete m_ui;
}

void OneFilter::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

 void OneFilter::setFn(int i)
 {
     n=i;
     m_ui->le_Nf->setText(QString("%1").arg((i)));

     // ініціалізувати перемикач режиму роботи.
     int v=s.getValue16(QString("Am_%1_").arg(n));
     m_ui->bx_Am->setCurrentIndex(v?1:0);

     if(v)
    {
        disconnect(m_ui->bn_Vl_1_1,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_2,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_3,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_4,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_5,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_6,SIGNAL(clicked()),this,SLOT(setValve()));
    }
    else
    {
        connect(m_ui->bn_Vl_1_1,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_2,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_3,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_4,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_5,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_6,SIGNAL(clicked()),this,SLOT(setValve()));
    }

     m_ui->Tozad_zd->setTime(QTime().addMSecs(s.getValue32(QString("Tosad_%1_zd").arg(i))));
     connect(m_ui->Tozad_zd,SIGNAL(timeChanged(QTime)),this,SLOT(setTosad(QTime)));

     m_ui->Rskyd->setCurrentIndex(s.getValue16(QString("Rskud_%1)").arg(n))?1:0);
     connect(m_ui->Rskyd,SIGNAL(currentIndexChanged(int)),this,SLOT(setRskyd(int)));


}


 void OneFilter::updateData()
 {
        qint16 mode;
        mode=s.getValue16(QString("Amr_%1_").arg(n));

        m_ui->le_Amr->setText(mode?tr("Дистанційне"):tr("Місцеве"));

        if(mode==0 && m_ui->bx_Am->currentIndex()>0 ) // в локальному режимі
        {
            m_ui->bx_Am->setCurrentIndex(0); // вимкнути автомат
        }

        m_ui->cb_Cl_1_1->setChecked(s.getValue16(QString("Cl_%1_1").arg(n)));
        m_ui->cb_Cl_1_2->setChecked(s.getValue16(QString("Cl_%1_2").arg(n)));
        m_ui->cb_Cl_1_3->setChecked(s.getValue16(QString("Cl_%1_3").arg(n)));
        m_ui->cb_Cl_1_4->setChecked(s.getValue16(QString("Cl_%1_4").arg(n)));
        m_ui->cb_Cl_1_5->setChecked(s.getValue16(QString("Cl_%1_5").arg(n)));
        m_ui->cb_Cl_1_6->setChecked(s.getValue16(QString("Cl_%1_6").arg(n)));

            m_ui->bn_Vl_1_1->setIcon(QIcon(QPixmap(s.getValue16(QString("Vl_%1_1").arg(n))
                                                   ?":/pict/lib/lamp_green_16x16.png":":/pict/lib/lamp_off_16x16.png")));

            m_ui->bn_Vl_1_2->setIcon(QIcon(QPixmap(s.getValue16(QString("Vl_%1_2").arg(n))
                                                   ?":/pict/lib/lamp_green_16x16.png":":/pict/lib/lamp_off_16x16.png")));

            m_ui->bn_Vl_1_3->setIcon(QIcon(QPixmap(s.getValue16(QString("Vl_%1_3").arg(n))
                                                   ?":/pict/lib/lamp_green_16x16.png":":/pict/lib/lamp_off_16x16.png")));

            m_ui->bn_Vl_1_4->setIcon(QIcon(QPixmap(s.getValue16(QString("Vl_%1_4").arg(n))
                                                   ?":/pict/lib/lamp_green_16x16.png":":/pict/lib/lamp_off_16x16.png")));

            m_ui->bn_Vl_1_5->setIcon(QIcon(QPixmap(s.getValue16(QString("Vl_%1_5").arg(n))
                                                   ?":/pict/lib/lamp_green_16x16.png":":/pict/lib/lamp_off_16x16.png")));

            m_ui->bn_Vl_1_6->setIcon(QIcon(QPixmap(s.getValue16(QString("Vl_%1_6").arg(n))
                                                   ?":/pict/lib/lamp_green_16x16.png":":/pict/lib/lamp_off_16x16.png")));


 }

void OneFilter::setAm(int v)
{
    if(v)
    {
        disconnect(m_ui->bn_Vl_1_1,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_2,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_3,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_4,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_5,SIGNAL(clicked()),this,SLOT(setValve()));
        disconnect(m_ui->bn_Vl_1_6,SIGNAL(clicked()),this,SLOT(setValve()));

        s.sendValue(QString("Vl_%1_1").arg(n),qint16(0)); // відправити команду на клапан
        s.sendValue(QString("Vl_%1_2").arg(n),qint16(0)); // відправити команду на клапан
        s.sendValue(QString("Vl_%1_3").arg(n),qint16(0)); // відправити команду на клапан
        s.sendValue(QString("Vl_%1_4").arg(n),qint16(0)); // відправити команду на клапан
        s.sendValue(QString("Vl_%1_5").arg(n),qint16(0)); // відправити команду на клапан
        s.sendValue(QString("Vl_%1_6").arg(n),qint16(0)); // відправити команду на клапан

    }
    else
    {
        connect(m_ui->bn_Vl_1_1,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_2,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_3,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_4,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_5,SIGNAL(clicked()),this,SLOT(setValve()));
        connect(m_ui->bn_Vl_1_6,SIGNAL(clicked()),this,SLOT(setValve()));
    }
        s.sendValue(QString("Am_%1_").arg(n),qint16(v?-1:0));

}

void OneFilter::setValve()
{

    int f=sender()->objectName().split("_").at(3).toInt();
    int v=s.getValue16(QString("Vl_%1_%2").arg(n).arg(f));
    qDebug() << sender()->objectName() << "f" << f << "v" << v;

    s.sendValue(QString("Vl_%1_%2").arg(n).arg(f),qint16(v?0:-1)); // відправити команду на клапан
}

void OneFilter::setTosad(QTime v)
{
    qint32 val=QTime(0,0,0,0).msecsTo(v);
    //qDebug() << "setTosad" << val << v;

    s.sendValue(QString("Tosad_%1_zd").arg(n),val);

}

void OneFilter::setRskyd(int v)
{
    qint16 val=-v;
    s.sendValue(QString("Rskyd_%1_").arg(n),val);
}

