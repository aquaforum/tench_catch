
#include "mnemo.h"
#include "ui_mnemo.h"

#include <IoNetClient.h>

#include "trendchart.h"
#include "panelReg.h"

// унікальні діалогові вікна
#include "dlgpumpctrl.h"
#include "dlgvbctrl.h"
#include <QVBoxLayout>
#include <QPalette>
#include <QTimer>

Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);

    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateTags(int)),this,SLOT(updateToolTips(int)));


    // це кнопки виклику панелі регулятора
    QVector<QPushButton*> pbc;
    pbc
            << m_ui->s_cX_01
            << m_ui->s_cX_02
            << m_ui->s_cX_03
            << m_ui->s_cX_04
            << m_ui->s_cX_05
/*            << m_ui->s_cX_06 */
            << m_ui->s_cX_07
            << m_ui->s_cX_08
            << m_ui->s_cX_09
            << m_ui->s_cX_10
            << m_ui->s_cX_11
  /*          << m_ui->s_cX_12 */
            << m_ui->s_cX_13
            << m_ui->s_cX_14
  /*          << m_ui->s_cX_15 */
            << m_ui->s_cI_08
            << m_ui->s_cI_12
            << m_ui->s_cI_13
            << m_ui->s_cI_15
               ;

    foreach(QPushButton *p,pbc)
    {
        connect(p,SIGNAL(clicked()),this,SLOT(slotCallReg()));
    }

    // в даному випадку це кнопки управління продувкою
    s_Y    << m_ui->s_Y_01
            << m_ui->s_Y_02
            << m_ui->s_Y_03
            << m_ui->s_Y_04
            << m_ui->s_Y_05
            << m_ui->s_Y_06;


    foreach(QPushButton *p,s_Y)
    {
        connect(p,SIGNAL(clicked()),this,SLOT(slotCallVb()));
    }
    // інші кнопки
    connect(m_ui->s_cP_01,SIGNAL(clicked()),this,SLOT(slotCallPumpCtrl()));

    // цифрові індикатори
    le      << m_ui->s_V_01
            << m_ui->s_V_02
            << m_ui->s_V_03
            << m_ui->s_V_04
            << m_ui->s_V_05
            << m_ui->s_V_06
            << m_ui->s_V_07
            << m_ui->s_V_08
            << m_ui->s_V_09
            << m_ui->s_V_10
            << m_ui->s_V_11
            << m_ui->s_V_12
            << m_ui->s_V_13
            << m_ui->s_V_14
            << m_ui->s_V_15
            << m_ui->s_V_16
            << m_ui->s_V_17
            << m_ui->s_V_18
            << m_ui->s_V_19
            << m_ui->s_V_20
            << m_ui->s_V_21
            << m_ui->s_V_22
            << m_ui->s_V_23
            << m_ui->s_V_24
            << m_ui->s_V_25
            << m_ui->s_V_26
            << m_ui->s_V_27
            << m_ui->s_V_28
            << m_ui->s_V_29
            << m_ui->s_V_30
            << m_ui->s_V_31
            << m_ui->s_V_32
/*            << m_ui->s_V_33
            << m_ui->s_V_34 */
            << m_ui->s_V_35
            << m_ui->s_V_36
            << m_ui->s_V_37
            << m_ui->s_V_38
            << m_ui->s_V_39
/*            << m_ui->s_V_40*/
            << m_ui->s_V_41
            << m_ui->s_V_42
            << m_ui->s_V_43
            << m_ui->s_V_44
            << m_ui->s_V_45
/*            << m_ui->s_V_46*/
/*        le_X  колись це було окремо а зараз в цьому немає ніякого сенсу томі
 *        примкнув його до цифрових індикаторів */
            << m_ui->s_C_01
            << m_ui->s_C_02
            << m_ui->s_C_03
            << m_ui->s_C_04
            << m_ui->s_C_05
/*            << m_ui->s_C_06 */
            << m_ui->s_C_07
            << m_ui->s_C_08
            << m_ui->s_C_09
            << m_ui->s_C_10
            << m_ui->s_C_11
            << m_ui->s_C_12
            << m_ui->s_C_13
            << m_ui->s_C_14
            << m_ui->s_C_15;

    // вітові індикатори, фактично чекбокси
        cb
                << m_ui->s_AM_01
                << m_ui->s_AM_02
                << m_ui->s_AM_03
                << m_ui->s_AM_04
                << m_ui->s_AM_05
/*                << m_ui->s_AM_06 */
                << m_ui->s_AM_07
                << m_ui->s_AM_08
                << m_ui->s_AM_09
                << m_ui->s_AM_10
                << m_ui->s_AM_11
                << m_ui->s_AM_12
                << m_ui->s_AM_13
                << m_ui->s_AM_14
                << m_ui->s_AM_15
                << m_ui->s_AM_PUMP_01
                << m_ui->s_en_p_01
                << m_ui->s_en_p_02
                << m_ui->s_en_p_03
                << m_ui->s_en_p_04
                << m_ui->s_en_p_05
                << m_ui->s_en_p_06
                << m_ui->s_X_01
                << m_ui->s_X_02
                << m_ui->s_X_03
                << m_ui->s_X_04
                << m_ui->s_X_05
                << m_ui->s_X_06
                   ;

    // прогресбари
        pb
            << m_ui->s_pV_12
            << m_ui->s_pV_13
            << m_ui->s_pV_14
            << m_ui->s_pV_15
            << m_ui->s_pV_16
            << m_ui->s_pV_41
            << m_ui->s_pV_42
            << m_ui->s_pV_43
            << m_ui->s_pV_45
/*            << m_ui->s_pV_46 */;

        // тренди
    QVector<QFrame *> f ;
         f  << m_ui->s_Tr_01
            << m_ui->s_Tr_02
            << m_ui->s_Tr_03
            << m_ui->s_Tr_04
            << m_ui->s_Tr_05;

    for(int i=0;i<5;++i)
    {
        TrendChart *t=new TrendChart(this);
        QVBoxLayout *vbl = new QVBoxLayout(f[i]);
        vbl->addWidget(t);
        f[i]->setLayout(vbl);
        trc << t;
    }

    // це сильно константне рішення
    QStringList stl;
    // 1 вапно на дефекацію
    stl
            << "V_04"
            << "V_03"
            << "V_01" ;
    trChTags << stl;

    // 2 відкачка з холодного
    stl.clear();
    stl
            << "V_11"
            << "C_15"
            << "SP_15"
            << "V_14"
            << "SP2_15"
            << "Spr_15";


    trChTags << stl;

      // 3 pH 1 сатурації
    stl.clear();
    stl
            << "V_21"
            << "C_04"
            << "SP_04";

    trChTags << stl;

    //4 сік на дефекатор 2 сат.
    stl.clear();
    stl
            << "V_08"
            << "C_12"
            << "SP_12"
            << "V_41"
            << "Spr_12"
            << "SP2_12";

    trChTags << stl;

    //5 pH 2 сатурації
    stl.clear();
    stl
            << "V_22"
            << "C_14"
            << "SP_14";

    trChTags << stl;



    QTimer *t = new QTimer(this);
    t->setInterval(5000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrendChart()));
}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateToolTips(int v)
{
    qDebug() << "updateToolTips(" << v  << ")";
    // заповнити поля toolTip
    foreach(QLineEdit *p,le)
    {
        p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-2)]);
    }

    // заповнити поля toolTip
    foreach(QProgressBar *p,pb)
    {
        p->setToolTip(s[v]->getText()[p->objectName().right(p->objectName().size()-2)]);
    }
    // поновити підписи до графічків
     for(int i=0;i<5;++i)
     {

         QStringList sTt;
         int j=0;
         foreach(QString p,trChTags[i])
         {
             sTt << QString("<FONT SIZE=\"+1\" COLOR=#%1%2%3>---%4</FONT>")
                     .arg(trc[i]->color(j).red(),2,16,QChar('0'))
                     .arg(trc[i]->color(j).green(),2,16,QChar('0'))
                     .arg(trc[i]->color(j).blue(),2,16,QChar('0'))
                     .arg(s[v]->getText()[p]);
             ++j;
         }
         qDebug() <<i << sTt;
         trc[i]->setToolTip(sTt.join("<br>"));
     }
}

void Mnemo::updateDataRaw()
{
    // вивести дані на прогресбари
    foreach(QProgressBar *p,pb)
    {
        int t = s[0]->getValueFloat(p->objectName().right(p->objectName().size()-3));
        if(t<0) t=0;
        if(t>4000) t=4000;
        p->setValue(t);
    }
    // вивести дані на чекбокси
    foreach(QCheckBox *p,cb)
    {
        p->setChecked(s[0]->getValue16(p->objectName().right(p->objectName().size()-2)));
    }


/* залишу це на майбутнє, можливо колись доведеться розкоментувати
    QPalette pal_w,pal_y;
    pal_w.setColor(QPalette::Base,Qt::white);
    pal_y.setColor(QPalette::Base,Qt::yellow);

    foreach(QLineEdit *p,le_X)
    {
        p->setPalette(s[0]->getValue16(QString("Amr_%1").arg(p->objectName().right(2)))?pal_w:pal_y);
    }*/

    // це кнопки управління продувкою
    foreach(QPushButton* p,s_Y)
    {
        p->setIcon(QIcon(QPixmap(s[0]->getValue16(p->objectName().right(p->objectName().size()-2))
                ?":/valves/valve_green_20x32.png":":/valves/valve_off_20x32.png")));
    }

    m_ui->s_cP_01->setIcon(QIcon(QPixmap(s[0]->getValue16("X_08")
                                ?":/butons/pict/lib/pump_green_26x30.png":":/butons/pict/lib/pump_off_26x30.png")));
}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{
    int l=0,f=0; // довжина та кількість знаків після коми
    foreach(QLineEdit *p,le)
    {
        double range=
                s[0]->scaleFull(p->objectName().right(p->objectName().size()-2))
                -
                s[0]->scaleZero(p->objectName().right(p->objectName().size()-2));

        // qDebug() << p->objectName() << range;

        if( range >50.0)
        {
            l=3;
            f=0;
        } else if(range > 20.0)
        {
            l=4;
            f=1;

        }
        else
        {
            l=3;
            f=2;
        }
        p->setText(QString("%1").arg(s[0]->getValueScaled(p->objectName().right(p->objectName().size()-2)),l,'f',f));
    }
/* цього також немає
    foreach(QLineEdit *p,le_X)
    {
        p->setText(QString("%1").arg(s[0]->getValueScaled(p->objectName().right(p->objectName().size()-2)),3,'f',0));
    } */

}

void Mnemo::updateTrendChart() // поновлення графіків
{
    QVector<double> v;
    int i=0;
    foreach(QStringList str,trChTags)
    {
        //qDebug() << str;
        v.clear();
        foreach(QString t,str)
        {
            v << s[0]->getValueFloat(t);
        }
        trc[i++]->addPoint(v);

    }
}


void Mnemo::slotCallReg()
{
    RpanelReg p(*s[0],sender()->objectName().right(2).toInt()-1,this);
    p.exec();
}

void Mnemo::slotCallVb()
{
    dlgVbCtrl p(*s[0],sender()->objectName().right(2).toInt(),this);
    p.exec();
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

    dlgPumpCtrl p(*s[0],v,this);
    p.exec();
}




