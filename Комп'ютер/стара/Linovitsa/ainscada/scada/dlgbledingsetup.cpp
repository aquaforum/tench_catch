#include "dlgbledingsetup.h"
#include "ui_dlgbledingsetup.h"
//#include "iodev.h"
//#include <QTimer>
#include <QDateTime>

#include <IoNetClient.h>

#define MAX_BLED 6

#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))
#define APARAT (*(src[3]))
#define CENTRIF (*(src[4]))
#define GASPICH (*(src[5]))
#define ROU (*(src[6]))

dlgBledingSetup::dlgBledingSetup(QVector<IoNetClient*> &source,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgBledingSetup),
    src(source)
{
    ui->setupUi(this);
    // ініціалізація
    cb_e  << ui->en_p_01
            << ui->en_p_02
            << ui->en_p_03
            << ui->en_p_04
            << ui->en_p_05
            << ui->en_p_06;

    cb_k    << ui->kr_p_01
            << ui->kr_p_02
            << ui->kr_p_03
            << ui->kr_p_04
            << ui->kr_p_05
            << ui->kr_p_06 ;

    te_p      << ui->Tp_zd_01
            << ui->Tp_zd_02
            << ui->Tp_zd_03
            << ui->Tp_zd_04
            << ui->Tp_zd_05
            << ui->Tp_zd_06;

    te_m        << ui->Tm_zd_01
            << ui->Tm_zd_02
            << ui->Tm_zd_03
            << ui->Tm_zd_04
            << ui->Tm_zd_05
            << ui->Tm_zd_06;

    QTime t;

    foreach(QTimeEdit* v,te_m)
    {
        t.setHMS(0,0,0,0);


        v->setTime(t.addMSecs(SATUR[0]->getValue16(v->objectName())*60000));
//        qDebug() << v->objectName() << t.addMSecs(SATUR[0]->getValue16(v->objectName())*60000);
        connect(v,SIGNAL(timeChanged(QTime)),this,SLOT(slotSet(QTime)));

    }

    foreach(QTimeEdit* v,te_p)
    {
        t.setHMS(0,0,0,0);
        v->setTime(t.addMSecs(SATUR[0]->getValue16(v->objectName())*1000));
        connect(v,SIGNAL(timeChanged(QTime)),this,SLOT(slotSet(QTime)));

    }

    foreach(QComboBox *v,cb_e)
    {
        v->setCurrentIndex(SATUR[0]->getValue16(v->objectName())?1:0);
        connect(v,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSet(int)));
    }

    QHash<int,int> s;
    s[1440]=0;
    s[720]=1;
    s[480]=2;
    s[360]=3;
    s[240]=4;
    s[180]=5;
    s[120]=6;
    s[60]=7;
    s[30]=8;

    foreach(QComboBox *v,cb_k)
    {
        v->setCurrentIndex(s.contains(SATUR[0]->getValue16(v->objectName()))?s[SATUR[0]->getValue16(v->objectName())]:0);
        connect(v,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSet(int)));
    }

    // синхронізація годинника контролера, це би зробити автоматично......
    QVector<qint16> time;
    QDateTime ct=QDateTime::currentDateTime();
    time << ct.date().year() << ct.date().month() << ct.date().day() << ct.time().hour() << ct.time().minute() << ct.time().second() << -1;
    SATUR[0]->sendValue("yy_s",time);
    updateList();

    // настройка продувок дифузії
    d_cb_e  << ui->d_en_p_01;

    d_cb_k    << ui->d_kr_p_01;

    d_te_p      << ui->d_Tp_zd_01;

    d_te_m        << ui->d_Tm_zd_01;


    ui->d_en_p_01->setCurrentIndex(DIFUZ[0]->getValue16("en_p_01")?1:0);
    connect(ui->d_en_p_01,SIGNAL(currentIndexChanged(int)),this,SLOT(d_slotSet(int)));

    t.setHMS(0,0,0,0);
    ui->d_Tp_zd_01->setTime(t.addMSecs(DIFUZ[0]->getValue16("Tp_zd_01")*1000));
    connect(ui->d_Tp_zd_01,SIGNAL(timeChanged(QTime)),this,SLOT(d_slotSet(QTime)));

    t.setHMS(0,0,0,0);
    ui->d_Tm_zd_01->setTime(t.addMSecs(DIFUZ[0]->getValue16("Tm_zd_01")*60000));
    connect(ui->d_Tm_zd_01,SIGNAL(timeChanged(QTime)),this,SLOT(d_slotSet(QTime)));

    ui->d_kr_p_01->setCurrentIndex(s.contains(DIFUZ[0]->getValue16("kr_p_01"))?s[DIFUZ[0]->getValue16("kr_p_01")]:0);
    connect(ui->d_kr_p_01,SIGNAL(currentIndexChanged(int)),this,SLOT(d_slotSet(int)));

    d_updateList();



}

dlgBledingSetup::~dlgBledingSetup()
{
    delete ui;
}

void dlgBledingSetup::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void dlgBledingSetup::slotSet(int v)
{
    if(sender()->objectName().left(2)=="en")
    {
        SATUR[0]->sendValue(sender()->objectName(),qint16(-v));
    }
    else
    {
        QVector<qint16> vi;
        vi << 1440 << 720 << 480 << 360 << 240  << 180 << 120 << 60 << 30;
        SATUR[0]->sendValue(sender()->objectName(),vi[v]);
    }
    SATUR[0]->sendValue("Save",qint16(-1));
    updateList();
}

void dlgBledingSetup::slotSet(QTime v)
{
    qint16 t=0;
    if(sender()->objectName().left(2)=="Tm")
        t= QTime(0,0,0,0).msecsTo(v)/60000;
    else
        t= QTime(0,0,0,0).msecsTo(v)/1000;

    SATUR[0]->sendValue(sender()->objectName(),t);
    SATUR[0]->sendValue("Save",qint16(-1));

    //qDebug() << sender()->objectName() << t;
    updateList();
}

void dlgBledingSetup::updateList()
{
    QStringList tankName        ;
    tankName << tr("6-та зона ПДБМ")
            << tr("Гарячий дефекатор")
            << tr("Змішувач після ПДБМ")
            << tr("1-й сатуратор")
            << tr("2-й сатуратор")
            << tr("Відстійник 2-сат.");

    QVector<qint16> vi;
    vi << 1440 << 720 << 480 << 360 << 240  << 180 << 120 << 60 << 30;


    QVector<int> time;
    QVector<QString> name;
    for(int i=0;i<MAX_BLED;++i)
    {
        if(cb_e[i]->currentIndex()) // перевірити чи включена продувка
        {
            int start = QTime(0,0,0,0).msecsTo(te_m[i]->time())/60000; // отримати час пуску
            int kr = vi[cb_k[i]->currentIndex()];  // отримати кратність
            for(int j=0;j<1440/kr;++j) // крутить в циклі
            {
                int k;
                for(k=0;k<time.size();++k) // знайти наше місце
                {
                    if(time[k]>start)
                        break;
                }
                // вставити знайдені значення

                time.insert(k,start);
                name.insert(k,tankName[i]);
                // розрахувати
                start+=kr;
                start %=1440;
            }
        }
    }
    ui->teList->clear();
    for(int i=0;i<time.size();++i)
    {
        ui->teList->append(QString("%1. %2:%3\t%4").arg(i+1,3).arg(time[i]/60,2,10,QChar('0')).arg(time[i]%60,2,10,QChar('0')).arg(name[i]));
    }

}


void dlgBledingSetup::d_updateList()
{
    QStringList tankName        ;
    tankName << tr("Пульполовушка Ж.П. води");

    QVector<qint16> vi;
    vi << 1440 << 720 << 480 << 360 << 240  << 180 << 120 << 60 << 30;




    QVector<int> time;
    QVector<QString> name;
    for(int i=0;i<tankName.size();++i)
    {
        if(d_cb_e[i]->currentIndex()) // перевірити чи включена продувка
        {
            int start = QTime(0,0,0,0).msecsTo(d_te_m[i]->time())/60000; // отримати час пуску
            int kr = vi[d_cb_k[i]->currentIndex()];  // отримати кратність
            for(int j=0;j<1440/kr;++j) // крутить в циклі
            {
                int k;
                for(k=0;k<time.size();++k) // знайти наше місце
                {
                    if(time[k]>start)
                        break;
                }
                // вставити знайдені значення

                time.insert(k,start);
                name.insert(k,tankName[i]);
                // розрахувати
                start+=kr;
                start %=1440;
            }
        }
    }
    ui->d_teList->clear();
    for(int i=0;i<time.size();++i)
    {
        ui->d_teList->append(QString("%1. %2:%3\t%4").arg(i+1,3).arg(time[i]/60,2,10,QChar('0')).arg(time[i]%60,2,10,QChar('0')).arg(name[i]));
    }

}

void dlgBledingSetup::d_slotSet(int v)
{
    QString tag=sender()->objectName().right(sender()->objectName().size()-2);

    if(tag=="en_p_01")
    {
        DIFUZ[0]->sendValue(tag,qint16(-v));
        qDebug() << tag<< -v;
    }
    else
    {
        QVector<qint16> vi;
        vi << 1440 << 720 << 480 << 360 << 240  << 180 << 120 << 60 << 30;
        DIFUZ[0]->sendValue(tag,vi[v]);
    }
    DIFUZ[0]->sendValue("Save",qint16(-1));
    d_updateList();
}

void dlgBledingSetup::d_slotSet(QTime v)
{
    QString tag=sender()->objectName().right(sender()->objectName().size()-2);

    qint16 t=0;
    if(tag=="Tm_zd_01")
        t= QTime(0,0,0,0).msecsTo(v)/60000;
    else
        t= QTime(0,0,0,0).msecsTo(v)/1000;

    DIFUZ[0]->sendValue(tag,t);
    DIFUZ[0]->sendValue("Save",qint16(-1));

    //qDebug() << sender()->objectName() << t;
    d_updateList();
}
