
#include "one_mnemo1.h"
#include "ui_one_mnemo1.h"


#include <aulib.h>
#include <trendchar/trendchart.h>


#include <QVBoxLayout>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>
#include <QSlider>
#include <QTimer>

#define CFALL QStringLiteral("plcap0")
#define CF4 QStringLiteral("plccf4")
#define CF5 QStringLiteral("plccf5")

#define CFNOM_OFFSET 4

OneMnemo1::OneMnemo1(AuBase &source, int cfNom, QWidget *p) :
        QWidget(p),
        m_ui(new Ui::one_mnemo1),
        nCf(cfNom),
        src(source),
        trcf(nullptr)
{
    setObjectName("OneMnemo1");

    m_ui->setupUi(this);
    cfIx[0]=src[CF4].index();
    cfIx[1]=src[CF5].index();
    cfIx[2]=src[CFALL].index();

    //connect(&src[CF4],&AuPlc::updateData,this,&OneMnemo1::updateData);
    //connect(&src[CF5],&AuPlc::updateData,this,&OneMnemo1::updateData);

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

    cf[nCf]->setChecked(true);

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

void OneMnemo1::updateData() // слот обновляє дані на мнемосхемі
{
    QString tag;
    AuPlc &s=*qobject_cast<AuPlc*>(sender());
    int j=0;

    foreach(QCheckBox* v,cb)
    {
        SAFE(v->setCheckState(s[cb_name[j]].value()!=0?Qt::Checked: Qt::Unchecked));
        ++j;
    }


    SAFE(m_ui->Nc->setText(s[QStringLiteral("Nc")]));
    SAFE(m_ui->T_all->setText(s[QStringLiteral("T_all")]));

    SAFE(m_ui->Q_all->setText(s[QStringLiteral("Qall")]));


    SAFE(m_ui->Q_reg->setText(s[QStringLiteral("Q_reg")]));
    SAFE(m_ui->T_reg->setText(s[QStringLiteral("T_reg")]));

    SAFE(m_ui->Q_man->setText(s[QStringLiteral("Q_man")]));

    SAFE(m_ui->Gzag->setText(s[QStringLiteral("Gzag")]));

    SAFE(m_ui->Q_prl->setText(s[QStringLiteral("Q_prl")]));
    SAFE(m_ui->T_prl->setText(s[QStringLiteral("T_prl")]));

    SAFE(m_ui->Fott->setText(s[QStringLiteral("Fott")]));
    SAFE(m_ui->T_fug->setText(s[QStringLiteral("T_fug")]));

//        m_ui->Tsir->setText(s[QStringLiteral("Tsir")/1000));

    SAFE(m_ui->Q_pr_zd->setText(s[QStringLiteral("Q_pr_zd")]));
    SAFE(m_ui->Q_pr->setText(s[QStringLiteral("Q_pr")]));
    SAFE(m_ui->T_pr->setText(s[QStringLiteral("T_pr")]));
    SAFE(m_ui->Imp->setText(s[QStringLiteral("Imp")]));
    SAFE(m_ui->T_p->setText(s[QStringLiteral("T_p")]));

    SAFE(m_ui->T_nt->setText(s[QStringLiteral("T_nt")]));

    SAFE(m_ui->Ob->setText(s[QStringLiteral("Ob")]));
    SAFE(m_ui->I_a->setText(s[QStringLiteral("I_a")]));
    SAFE(m_ui->I_b->setText(s[QStringLiteral("I_b")]));

    int Status = 0;
    SAFE(Status=s[QStringLiteral("State")]);

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

void OneMnemo1::updateTrChart() // поновлюємо дані на графічках
{
    QVector<double> v;
    trcf->addPoint(v);
}

void OneMnemo1::changeCf()
{

    if(sender()) // якщо відправлено не самим класом при ініціалізації
    {
        disconnect(&src[cfIx[nCf]],&AuPlc::updateData,this,&OneMnemo1::updateData); // від'єднатися
        nCf=sender()->objectName().split("_").at(1).toInt()-CFNOM_OFFSET; // !!!!!!!!!!!!!!!!!!!!!!!!!!!! корекція під 4 5 центрифуги
    }
    AuPlc &s=src[cfIx[nCf]];
    connect(&s,&AuPlc::updateData,this,&OneMnemo1::updateData);

    if(trcf)
    {
        delete trcf;
    }

    trcf=new TrendChart(m_ui->trcf,600,1);
    QVBoxLayout *vbl=new QVBoxLayout(m_ui->trcf);
    vbl->addWidget(trcf);
    vbl->setMargin(2);
    m_ui->trcf->setLayout(vbl);



    m_ui->cfNom->setText(QString("%1").arg(nCf+CFNOM_OFFSET));

    m_ui->Q_prl_zd->blockSignals(true);
    m_ui->Q_prl_zd->setValue(s[QStringLiteral("Q_prl_zd")]);
    m_ui->Q_prl_zd->blockSignals(false);

    m_ui->Q_k->blockSignals(true);
    m_ui->Q_k->setValue(s[QStringLiteral("Q_k")]);
    m_ui->Q_k->blockSignals(false);

    m_ui->Q_reg_zd->blockSignals(true);
    m_ui->Q_reg_zd->setValue(s[QStringLiteral("Q_reg_zd")]);
    m_ui->Q_reg_zd->blockSignals(false);

    m_ui->Fott_zd->blockSignals(true);
    m_ui->Fott_zd->setValue(s[QStringLiteral("Fott_zd")]);
    m_ui->Fott_zd->blockSignals(false);

    m_ui->T_fug_zd->blockSignals(true);
    m_ui->T_fug_zd->setValue(s[QStringLiteral("T_fug_zd")]);
    m_ui->T_fug_zd->blockSignals(false);

    m_ui->T_sir_zd->blockSignals(true);
    m_ui->T_sir_zd->setValue(s[QStringLiteral("T_sir_zd")]);
    m_ui->T_sir_zd->blockSignals(false);

    m_ui->Tnt_zd->blockSignals(true);
    m_ui->Tnt_zd->setValue(s[QStringLiteral("Tnt_zd")]);
    m_ui->Tnt_zd->blockSignals(false);

    m_ui->Ts_z_zd->blockSignals(true);
    m_ui->Ts_z_zd->setValue(s[QStringLiteral("Ts_z_zd")]);
    m_ui->Ts_z_zd->blockSignals(false);

    m_ui->Ts_b_zd->blockSignals(true);
    m_ui->Ts_b_zd->setValue(s[QStringLiteral("Ts_b_zd")]);
    m_ui->Ts_b_zd->blockSignals(false);


    m_ui->rej_fug->blockSignals(true);
    m_ui->rej_fug->setChecked(s[QStringLiteral("rej_fug")].value()!=0);
    m_ui->rej_fug->setText(s[QStringLiteral("rej_fug")].value()!=0?tr("По відходу"):tr("По часу"));
    m_ui->rej_fug->blockSignals(false);

    m_ui->rej_sgr->blockSignals(true);
    m_ui->rej_sgr->setChecked(s[QStringLiteral("rej_sgr")].value()!=0);
    m_ui->rej_sgr->blockSignals(false);

}

void OneMnemo1::sendValue(int v)
{
    src[cfIx[nCf]][sender()->objectName()]=v;
}

void OneMnemo1::sendValue(double v)
{
    src[cfIx[nCf]][sender()->objectName()]=v;
}

void OneMnemo1::sendValue(bool v)
{
    src[cfIx[nCf]][sender()->objectName()]=v;
}

void OneMnemo1::sendRejSgr(bool v)
{
    src[cfIx[nCf]][QStringLiteral("rej_sgr")]=qint16(v?-1:0);
}
