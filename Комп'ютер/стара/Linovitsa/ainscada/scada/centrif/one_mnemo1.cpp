
#include "one_mnemo1.h"
#include "ui_one_mnemo1.h"
#include <trendchart.h>

#include <IoNetClient.h>

#include <QVBoxLayout>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>
#include <QSlider>

OneMnemo1::OneMnemo1(IoNetClient &src,QVector<TrendChart*> &masterTrChart, int cfNom, QWidget *p) :
        QWidget(p),
        m_ui(new Ui::one_mnemo1),
        nCf(cfNom),
        s(src),
        trcf(NULL),
        mTrChart(masterTrChart)
{
    m_ui->setupUi(this);

    connect(&s,SIGNAL(updateData(int)),this,SLOT(updateDataRaw(int))); // при отриманні нових даних, засвітити їх на картинці !!!!!!!!!!!!!!!!!!!!!!!!
    //connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
    cb
        << m_ui->Xsgrb
        << m_ui->Xsgrz
        << m_ui->Xshib
        << m_ui->Ypr
        << m_ui->Ysgr
        << m_ui->Ysir
        << m_ui->Ydsp
        << m_ui->Xdsph
        << m_ui->Xdspl
           ;

    foreach (QCheckBox *p, cb) {
        cb_name << p->objectName();
    }

    State <<tr("Ручний режим")  //-1
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
           << tr ("Невизначено")
        ;


    cb_state=findChildren<QCheckBox*>(QRegExp("^cb_"));

    // елементи управління
    connect(m_ui->Q_reg_zd,SIGNAL(valueChanged(double)),this,SLOT(sendValue(double)));
    connect(m_ui->Q_prl_zd,SIGNAL(valueChanged(double)),this,SLOT(sendValue(double)));

    connect(m_ui->Fott_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));
    connect(m_ui->T_fug_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));

    connect(m_ui->T_sir_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));

    connect(m_ui->Q_k,SIGNAL(valueChanged(double)),this,SLOT(sendValue(double)));

    connect(m_ui->Ts_b_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));
    connect(m_ui->Ts_z_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));

    connect(m_ui->Tnt_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));

    connect(m_ui->rej_sgr,SIGNAL(clicked(bool)),this,SLOT(sendRejSgr(bool)));

    //connect(m_ui->cf_1,SIGNAL(clicked()),this,SLOT(changeCf()));
    //connect(m_ui->cf_2,SIGNAL(clicked()),this,SLOT(changeCf()));
    //connect(m_ui->cf_3,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_4,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_5,SIGNAL(clicked()),this,SLOT(changeCf()));

        connect(m_ui->rej_fug,SIGNAL(clicked(bool)),this,SLOT(sendValue(bool)));

    QVector<QRadioButton *> cf;
    cf //<< m_ui->cf_1
       //     << m_ui->cf_2
       //        << m_ui->cf_3
                  << m_ui->cf_4
                     << m_ui->cf_5;

    cf[nCf-1]->setChecked(true);

    changeCf();

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrChart()));
    t->start();

}

OneMnemo1::~OneMnemo1()
{
    //qDebug() << "OneMnemo1: call destructor";
    delete trcf;
    delete m_ui;
}

void OneMnemo1::updateDataRaw(int i) // слот обновляє дані на мнемосхемі
{
     qDebug() << "i=" << i << "nCf=" << nCf;

    if(i==nCf) // якщо моя центрифуга
    {

        int j=0;
        foreach(QCheckBox* v,cb)
        {
            v->setCheckState(s[i]->getValue16(cb_name[j])?Qt::Checked: Qt::Unchecked);
            ++j;
        }


        m_ui->Nc->setText(QString("%1").arg(s[i]->getValue32("Nc")));
        m_ui->T_all->setText(QString("%1").arg(s[i]->getValue16("T_all")));

        m_ui->Q_all->setText(QString("%1").arg(s[i]->getValueFloat("Qall"),4,'f',1));


        m_ui->Q_reg->setText(QString("%1").arg(s[i]->getValueFloat("Q_reg"),4,'f',1));
        m_ui->T_reg->setText(QString("%1").arg(s[i]->getValue16("T_reg")));

        m_ui->Q_man->setText(QString("%1").arg(s[i]->getValueFloat("Q_man"),4,'f',1));

        m_ui->Gzag->setText(QString("%1").arg(s[i]->getValueFloat("Gzag"),4,'f',0));

        m_ui->Q_prl->setText(QString("%1").arg(s[i]->getValueFloat("Q_prl"),4,'f',1));
        m_ui->T_prl->setText(QString("%1").arg(s[i]->getValue16("T_prl")));

        m_ui->Fott->setText(QString("%1").arg(s[i]->getValue16("Fott")));
        m_ui->T_fug->setText(QString("%1").arg(s[i]->getValue16("T_fug")));

//        m_ui->Tsir->setText(QString("%1").arg(s[i]->getValue16("Tsir")/1000));

        m_ui->Q_pr_zd->setText(QString("%1").arg(s[i]->getValueFloat("Q_pr_zd"),4,'f',1));
        m_ui->Q_pr->setText(QString("%1").arg(s[i]->getValueFloat("Q_pr"),4,'f',1));
        m_ui->T_pr->setText(QString("%1").arg(s[i]->getValue16("T_pr")));
        m_ui->Imp->setText(QString("%1").arg(s[i]->getValue16("Imp")));
        m_ui->T_p->setText(QString("%1").arg(s[i]->getValue16("T_p")));

        m_ui->T_nt->setText(QString("%1").arg(s[i]->getValue16("T_nt")));

        m_ui->Ob->setText(QString("%1").arg(s[i]->getValue16("Ob")));
        m_ui->I_a->setText(QString("%1").arg(s[i]->getValueFloat("I_a"),0,'f',0));
        m_ui->I_b->setText(QString("%1").arg(s[i]->getValueFloat("I_b"),0,'f',0));

        int Status = s[i]->getValue16("State");

        if( Status<-1 || Status>17) Status=18;
        m_ui->State->setText(State[Status+1]);

        if(Status!=lastStatus)
        {
            foreach(QCheckBox *v,cb_state)
            {
                if(v->objectName()!="cb_prl")
                    v->setChecked(false);

                switch(Status)
                {
                    case -1:
                        m_ui->cb_man->setChecked(true);
                        break;
                    case 0:
                        m_ui->cb_stop->setChecked(true);
                        break;
                case 2:
                    m_ui->cb_reg->setChecked(true);
                    break;
                case 3:
                    m_ui->cb_wait->setChecked(true);
                    break;
                case 4:
                    m_ui->cb_load->setChecked(true);
                    break;
                case 5:
                    m_ui->cb_fug->setChecked(true);
                    break;
                case 6:
                case 7:
                    m_ui->cb_sir->setChecked(true);
                    break;

                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                    m_ui->cb_pr->setChecked(true);
                    break;
                case 13:
                    m_ui->cb_nt->setChecked(true);
                    break;
                case 14:
                    m_ui->cb_torm->setChecked(true);
                    break;
                case 15:
                    m_ui->cb_vigr->setChecked(true);
                    break;
                default:
                    break;
                }
            }
        }


    }
}

void OneMnemo1::updateDataScaled() // слот обновляє дані на мнемосхемі
{

}

void OneMnemo1::updateTrChart() // поновлюємо дані на графічках
{
    QVector<double> v;
    trcf->addPoint(v);
    updateDataRaw(nCf);
}

void OneMnemo1::changeCf()
{

    if(sender()) // якщо відправлено не самим класом при ініціалізації
    {
        nCf=sender()->objectName().split("_").at(1).toInt()-3; // !!!!!!!!!!!!!!!!!!!!!!!!!!!! корекція під 4 5 центрифуги
        qDebug() << sender()->objectName();
    }
    qDebug() << "Selected cf" << nCf;

    if(trcf)
    {
        delete trcf;
    }
    trcf=new TrendChart(m_ui->trcf,mTrChart[nCf-1]);

    if(nCf<s.size())
 {

        qDebug()  <<"nCf" << nCf;

    m_ui->cfNom->setText(QString("%1").arg(nCf));

    m_ui->Q_prl_zd->blockSignals(true);
    m_ui->Q_prl_zd->setValue(s[nCf]->getValueFloat("Q_prl_zd"));
    m_ui->Q_prl_zd->blockSignals(false);

    m_ui->Q_k->blockSignals(true);
    m_ui->Q_k->setValue(s[nCf]->getValueFloat("Q_k"));
    m_ui->Q_k->blockSignals(false);

    m_ui->Q_reg_zd->blockSignals(true);
    m_ui->Q_reg_zd->setValue(s[nCf]->getValueFloat("Q_reg_zd"));
    m_ui->Q_reg_zd->blockSignals(false);

    m_ui->Fott_zd->blockSignals(true);
    m_ui->Fott_zd->setValue(s[nCf]->getValue16("Fott_zd"));
    m_ui->Fott_zd->blockSignals(false);

    m_ui->T_fug_zd->blockSignals(true);
    m_ui->T_fug_zd->setValue(s[nCf]->getValue16("T_fug_zd"));
    m_ui->T_fug_zd->blockSignals(false);

    m_ui->T_sir_zd->blockSignals(true);
    m_ui->T_sir_zd->setValue(s[nCf]->getValue16("T_sir_zd"));
    m_ui->T_sir_zd->blockSignals(false);

    m_ui->Tnt_zd->blockSignals(true);
    m_ui->Tnt_zd->setValue(s[nCf]->getValue16("Tnt_zd"));
    m_ui->Tnt_zd->blockSignals(false);

    m_ui->Ts_z_zd->blockSignals(true);
    m_ui->Ts_z_zd->setValue(s[nCf]->getValue16("Ts_z_zd"));
    m_ui->Ts_z_zd->blockSignals(false);

    m_ui->Ts_b_zd->blockSignals(true);
    m_ui->Ts_b_zd->setValue(s[nCf]->getValue16("Ts_b_zd"));
    m_ui->Ts_b_zd->blockSignals(false);

    }

    m_ui->rej_fug->blockSignals(true);
    m_ui->rej_fug->setChecked(s[nCf]->getValue16("rej_fug")!=0);
    m_ui->rej_fug->setText(s[nCf]->getValue16("rej_fug")!=0?tr("По відходу"):tr("По часу"));
    m_ui->rej_fug->blockSignals(false);

    m_ui->rej_sgr->blockSignals(true);
    m_ui->rej_sgr->setChecked(s[nCf]->getValue16("rej_sgr")!=0);
    m_ui->rej_sgr->blockSignals(false);

}

void OneMnemo1::sendValue(int v)
{
    s[nCf]->sendValue(sender()->objectName(),v);
}

void OneMnemo1::sendValue(double v)
{
    s[nCf]->sendValue(sender()->objectName(),v);
}

void OneMnemo1::sendValue(bool v)
{
    s[nCf]->sendValue("rej_fug",qint16(v?-1:0));

    m_ui->rej_fug->setText(v?tr("По відходу"):tr("По часу"));

}

void OneMnemo1::sendRejSgr(bool v)
{
    s[nCf]->sendValue("rej_sgr",qint16(v?-1:0));


}
