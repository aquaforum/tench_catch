#include <aulibdefs.h>
#include <aubase.h>
#include <auplc.h>
#include <autag.h>

#include "aupanelreg.h"
#include "ui_aupanelreg.h"
#include "trendchar/trendchart.h"

#include <QSettings>
#include <QRect>
#include <QDesktopWidget>
#include <QScreen>

#include <QFile>
#include <QDebug>

#include <QTimer>
#include <QCloseEvent>
#include <QDial>

#define IX(X) RegDes[RegNum][Ri::X]

AuPanelReg::AuPanelReg(AuBase &base, quint16 index, int n, QString cfName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuPanelReg),
    myBase(base),
    plcIx(index),
    src(base[index])
{
    ui->setupUi(this);

    // виставити вікно на старий розмір
    QSettings set;
    int w=set.value("ranelReg/width",912).toInt();
    int h=set.value("ranelReg/height",562).toInt();
    QRect pos=QRect((QGuiApplication::screens()[0]->geometry().width()-w)/2,(QGuiApplication::screens()[0]->geometry().height()-h)/2,w,h);
    this->setGeometry(pos);

    ui->Trend->hide();
    ui->RegParm->hide(); // сховати область настройки регулятора

    connect(ui->Exit,&QPushButton::clicked,this,&AuPanelReg::accept);

    connect(ui->Setting,QOverload<bool>::of(&QPushButton::clicked),this,&AuPanelReg::Control);
    connect(ui->Trend,&QPushButton::clicked,this,&AuPanelReg::runTrend);

    // реакції користувацького інтерфейсу
    connect(ui->vsX,QOverload<int>::of(&QSlider::valueChanged),this,QOverload<int>::of(&AuPanelReg::setCtrlValue)); //(int)
    connect(ui->sbX,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setCtrlValue)); //double
    // SP_1
    connect(ui->vsSP_1,QOverload<int>::of(&QSlider::valueChanged),this,QOverload<int>::of(&AuPanelReg::setParmValue));//int
    connect(ui->sbSP_1,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // SP_2
    connect(ui->vsSP_2,QOverload<int>::of(&QSlider::valueChanged),this,QOverload<int>::of(&AuPanelReg::setParmValue));//int
    connect(ui->sbSP_2,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double
    // SP_3
    connect(ui->vsSP_3,QOverload<int>::of(&QSlider::valueChanged),this,QOverload<int>::of(&AuPanelReg::setParmValue));//int
    connect(ui->sbSP_3,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // Kpr
    connect(ui->dialKpr,QOverload<int>::of(&QDial::valueChanged),this,QOverload<int>::of(&AuPanelReg::setParmValue));//int
    connect(ui->sbKpr,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // Ti
    connect(ui->dialTi,QOverload<int>::of(&QDial::valueChanged),this,QOverload<int>::of(&AuPanelReg::setParmValue));//int
    connect(ui->sbTi,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double
    // Td
    connect(ui->dialTd,QOverload<int>::of(&QDial::valueChanged),this,QOverload<int>::of(&AuPanelReg::setParmValue));//int
    connect(ui->sbTd,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double
    // Xmin
    connect(ui->sbXmin,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // Xmax
    connect(ui->sbXmax,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // K_1
    connect(ui->sbK_1,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // K_2
    connect(ui->sbK_2,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // K_3
    connect(ui->sbK_3,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // K_4
    connect(ui->sbK_4,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // Kkor
    connect(ui->sbKkor,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double

    // AM
    connect(ui->cbAM,QOverload<int>::of(&QComboBox::currentIndexChanged),this,QOverload<int>::of(&AuPanelReg::setParmAM));//int
    // Rej
    connect(ui->cbRej,QOverload<int>::of(&QComboBox::currentIndexChanged),this,QOverload<int>::of(&AuPanelReg::setParmRej));//int
    // pRev
    connect(ui->pRev,QOverload<bool>::of(&QRadioButton::clicked),this,&AuPanelReg::setParmRev);//
    connect(ui->rRev,QOverload<bool>::of(&QRadioButton::clicked),this,&AuPanelReg::setParmRev);//
    // Rev
    connect(ui->regRev,&QCheckBox::stateChanged,this,&AuPanelReg::setParmKprSig);//int

    // P0
    connect(ui->sbP0,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,QOverload<double>::of(&AuPanelReg::setParmValue));//double
    connect(ui->slP0,QOverload<int>::of(&QSlider::valueChanged),this,QOverload<int>::of(&AuPanelReg::setParmValue));//int
    // Mode
    connect(ui->regMode,QOverload<bool>::of(&QCheckBox::clicked),this,&AuPanelReg::setParamMode);//bool

    connect(ui->cbInterval,QOverload<int>::of(&QComboBox::currentIndexChanged),this,QOverload<int>::of(&AuPanelReg::updateTrend));//int

    // to
    connect(ui->to,QOverload<int>::of(&QComboBox::currentIndexChanged),this,QOverload<int>::of(&AuPanelReg::setTO));//int


    // хеш для співставлення назв віджетів із індексами
    ctrlSearch["cbAM"]=Ri::AM;
    ctrlSearch["cbRej"]=Ri::Rej;

    ctrlSearch["vsSP_1"]=Ri::SP_1;
    ctrlSearch["sbSP_1"]=Ri::SP_1;

    ctrlSearch["vsSP_2"]=Ri::SP_2;
    ctrlSearch["sbSP_2"]=Ri::SP_2;

    ctrlSearch["vsSP_3"]=Ri::SP_3;
    ctrlSearch["sbSP_3"]=Ri::SP_3;

    ctrlSearch["sbKkor"]=Ri::Kkor;

    ctrlSearch["sbK_1"]=Ri::K_1;
    ctrlSearch["sbK_2"]=Ri::K_2;
    ctrlSearch["sbK_3"]=Ri::K_3;
    ctrlSearch["sbK_4"]=Ri::K_4;

    ctrlSearch["dialKpr"]=Ri::Kpr;
    ctrlSearch["sbKpr"]=Ri::Kpr;

    ctrlSearch["dialTi"]=Ri::TI;
    ctrlSearch["sbTi"]=Ri::TI;

    ctrlSearch["dialTd"]=Ri::Td;
    ctrlSearch["sbTd"]=Ri::Td;

    ctrlSearch["sbXmin"]=Ri::Xmin;
    ctrlSearch["sbXmax"]=Ri::Xmax;

    ctrlSearch["slP0"]=Ri::P0;
    ctrlSearch["sbP0"]=Ri::P0;
    ctrlSearch["cbMode"]=Ri::Mode;

    // завантажити дані
    QFile f(cfName);
    if(f.open(QIODevice::ReadOnly))
    {
        for(int i=0;!f.atEnd();++i)
        {
            QStringList sl=QString::fromUtf8(f.readLine()).trimmed().split("\t"); // читаємо дані із файла
            if(sl.size()> 22) // якщо прочитано всі рядки
            {
                ui->regList->addItem(sl[Ri::Deskritp]); // додати до списку регуляторів
                RegDes << sl; // зберегти
            }
            else
                qDebug() << "Reg load error " << sl.size() << sl ;
        }
    }
    else
        qDebug() << "panelReg: file not open "  << f.errorString();

    // поновлення даних
    connect(&src,&AuPlc::updateData,this,&AuPanelReg::updateData);


    // графік
    tL=new QVBoxLayout(ui->trendChar);
    trChart=nullptr;
    ui->trendChar->setLayout(tL);
    tL->setContentsMargins(4,10,4,30);

    t1 = new QTimer(this);
    t1->setInterval(5000);
    t1->start();
    connect(t1,&QTimer::timeout,this,&AuPanelReg::setGraph);

    connect(&myBase,&AuBase::htrData,this,&AuPanelReg::htrDataLoad);

    ui->regList->setCurrentIndex(n);
    changeReg(n);
    connect(ui->regList,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&AuPanelReg::changeReg);


}

AuPanelReg::~AuPanelReg()
{
    delete ui;
}


void AuPanelReg::closeEvent( QCloseEvent * event) // при закритті зберегти розміри вікна на майбутнє.
{
    QSettings set;
    set.setValue("ranelReg/width",size().width());
    set.setValue("ranelReg/height",size().height());

    event->accept();
}

void AuPanelReg::changeReg(int Index)// зміна регулятор
{
    // 0 - 1
    // 1 - 0.1
    // 2 - 0.01
    const double sbStep[4]={1.0,0.1,0.01,0.001};

    RegNum=Index;
    //qDebug() << "RegNum"         << RegNum << ;

    SAFE(ui->Value_1->setText(src[IX(PV_1)].text()));

    // додано при роботі над тма вакуум-апаратами
    // обов'язкова частина
    SAFE(ui->pbPV_1->setMaximum(src[IX(PV_1)].baseScale()));


    try {

        ui->Value_2->setText(src[IX(PV_2)].text());
        ui->Parametr_2->show();
        SAFE(ui->pbPV_2->setMaximum(src[IX(PV_2)].baseScale()));

    } catch (...) {
        ui->Parametr_2->hide();
    }

    try {
        ui->Value_3->setText(src[IX(PV_3)].text());
        ui->Parametr_3->show();
        SAFE(ui->pbPV_3->setMaximum(src[IX(PV_3)].baseScale()));

    } catch (...) {
        ui->Parametr_3->hide();
    }

    SAFE(ui->Valve->setText(src[IX(X)].text()));

    //ui->min_PV1->setText(QString("%1").arg(src.scaleZero(IX(PV_1]),3,'f',0));
    //ui->max_PV1->setText(QString("%1").arg(src.scaleFull(IX(PV_1]),3,'f',0));

    SAFE(ui->scalePV_1->setScaleMinMax(src[IX(PV_1)].zero(),src[IX(PV_1)].full())); // src.scaleZero(PV_1),src.scaleFull(PV_1)

    // показати-сховати потрібне
    // PV_2

    try {
        ui->scalePV_2->setScaleMinMax(src[IX(PV_2)].zero(),src[IX(PV_2)].full());
        ui->Parametr_2->show();
    }
    catch (...)
    {
        ui->Parametr_2->hide();
    }

    // PV_3
    try {
        ui->scalePV_3->setScaleMinMax(src[IX(PV_3)].zero(),src[IX(PV_3)].full());
        ui->Parametr_3->show();
    }
    catch (...)
    {
        ui->Parametr_3->hide();
    }

    // SPR_1
    try
    {
        src[IX(SPR_1)]; // просто перевірити його наявність. якщо немає попадемо в catch
        ui->leSPR_1->show();
        ui->vsSPR_1->show();
        ui->labelSPR_1->show();
        SAFE(ui->vsSPR_1->setMaximum(src[IX(SPR_1)].baseScale()));

    }
    catch (...)
    {
        ui->leSPR_1->hide();
        ui->vsSPR_1->hide();
        ui->labelSPR_1->hide();
    }

    // SP_2
    try
    {
        src[IX(SP_2)];
        ui->sbSP_2->show();
        ui->sbSP_2->setDecimals(src[IX(SP_2)].deciaml());
        ui->vsSP_2->show();
        SAFE(ui->vsSP_2->setMaximum(src[IX(SP_2)].baseScale()));


    }
    catch (...)
    {
        ui->sbSP_2->hide();
        ui->vsSP_2->hide();
    }

    // SP_3
    try
    {   src[IX(SP_3)];
        ui->sbSP_3->show();
        ui->sbSP_3->setDecimals(src[IX(SP_3)].deciaml());
        ui->vsSP_3->show();
        SAFE(ui->vsSP_3->setMaximum(src[IX(SP_3)].baseScale()));

    }
    catch (...)
    {
        ui->sbSP_3->hide();
        ui->vsSP_3->hide();
    }

    // K_1
    //qDebug() << IX(K_1];
    try
    {
        kk_1=(src[IX(PV_2)].full() -src[IX(PV_2)].zero()) /
              (src[IX(PV_1)].full() -src[IX(PV_1)].zero()) / 100.0;

        ui->sbK_1->blockSignals(true);
        ui->sbK_1->setValue(static_cast<double>(src[IX(K_1)])/kk_1); //треба шкалювати
        ui->sbK_1->blockSignals(false);

        ui->sbK_1->show();
        ui->labelK1->show();
    }
    catch (...)
    {
        ui->sbK_1->hide();
        ui->labelK1->hide();
    }

    // K_2
    try
    {
        ui->sbK_2->blockSignals(true);
        ui->sbK_2->setValue(src[IX(K_2)]); //треба шкалювати

        ui->sbK_2->show();
        ui->labelK2->show();
    }
    catch (...)
    {
        ui->sbK_2->hide();
        ui->labelK2->hide();
    }
    ui->sbK_2->blockSignals(false);

    // K_3
    try
    {
        ui->sbK_3->blockSignals(true);
        ui->sbK_3->setValue(src[IX(K_3)]); //треба шкалювати
        ui->sbK_3->show();
        ui->labelK3->show();
    }
    catch (...)
    {
        ui->sbK_3->hide();
        ui->labelK3->hide();
    }
    ui->sbK_3->blockSignals(false);

    // K_4
    try
    {
        ui->sbK_4->blockSignals(true);
        ui->sbK_4->setValue(src[IX(K_4)]);

        ui->sbK_4->show();
        ui->labelK4->show();
    }
    catch (...)
    {
        ui->sbK_4->hide();
        ui->labelK4->hide();
    }
    ui->sbK_4->blockSignals(false);

    // Kkor
    try
    {
        ui->sbKkor->blockSignals(true);
        ui->sbKkor->setValue(src[IX(Kkor)]);

        ui->sbKkor->show();
        ui->labelKkor->show();
    }
    catch (...)
    {
        ui->sbKkor->hide();
        ui->labelKkor->hide();
    }
    ui->sbKkor->blockSignals(false);

    // cbRej
    //qDebug() << IX(Rej] << src.getTags().contains(IX(Rej]);
    try
    {
        src[IX(Rej)];
        ui->cbRej->show();
        ui->labelRej->show();
    }
    catch (...)
    {
        ui->cbRej->hide();
        ui->labelRej->hide();
    }

//     // SP_1
    try{
        ui->sbSP_1->blockSignals(true);
        ui->sbSP_1->setDecimals(src[IX(SP_1)].deciaml());
        ui->sbSP_1->setSingleStep(sbStep[src[IX(SP_1)].deciaml()] );



        if(src[IX(SP_1)].zero()<src[IX(SP_1)].full())
        {
            // qDebug() << src[IX(SP_1)].zero() << src[IX(SP_1)].full();

            ui->sbSP_1->setMinimum(src[IX(SP_1)].zero());
            ui->sbSP_1->setMaximum(src[IX(SP_1)].full());

        }
        else
        {
            ui->sbSP_1->setMaximum(src[IX(SP_1)].zero());
            ui->sbSP_1->setMinimum(src[IX(SP_1)].full());
        }

        ui->sbSP_1->setValue(src[IX(SP_1)]);
        ui->sbSP_1->blockSignals(false);

        ui->vsSP_1->blockSignals(true);
        ui->vsSP_1->setValue(src[IX(SP_1)].valueReal());
        SAFE(ui->vsSP_1->setMaximum(src[IX(PV_1)].baseScale()));
        ui->vsSP_1->blockSignals(false);
    }
    CATCHALL
            ui->sbSP_1->blockSignals(false);

    // SP_2
    try
    {
        ui->sbSP_2->blockSignals(true);
        ui->sbSP_2->setMinimum(src[IX(SP_2)].zero());
        ui->sbSP_2->setMaximum(src[IX(SP_2)].full());

        ui->sbSP_2->setValue(src[IX(SP_2)]);
        ui->sbSP_2->setDecimals(src[IX(SP_2)].deciaml());
        ui->sbSP_2->setSingleStep(sbStep[src[IX(SP_2)].deciaml()]);

        ui->sbSP_2->blockSignals(false);

        ui->vsSP_2->blockSignals(true);
        ui->vsSP_2->setValue(src[IX(SP_2)].valueReal());
        ui->vsSP_2->blockSignals(false);
    }
    catch(...) {
        ui->sbSP_2->blockSignals(false);
    }

    // SP_3
    try
    {
        src[IX(SP_3)];
        ui->sbSP_3->show();
        ui->sbSP_3->blockSignals(true);

        ui->sbSP_3->setMinimum(src[IX(SP_3)].zero());
        ui->sbSP_3->setMaximum(src[IX(SP_3)].full());

        ui->sbSP_3->setValue(src[IX(SP_3)]);
        ui->sbSP_3->blockSignals(false);

        ui->vsSP_3->show();
        ui->vsSP_3->blockSignals(true);
        ui->vsSP_3->setValue(src[IX(SP_3)].valueReal());
        ui->vsSP_3->blockSignals(false);
    }
    catch (...)
    {
        ui->sbSP_3->hide();
        ui->vsSP_3->hide();
    }

    // параметри надаштуван регулятора вбрані в try {} catch(){} для вакуум-апаратів линовиці
    // там в регуляторіх присутні не всі коефіцієнти

    // Kpr
    try {
        ui->sbKpr->blockSignals(true);
        ui->dialKpr->blockSignals(true);
        ui->regRev->blockSignals(true);

        double t=fabs(src[IX(Kpr)]);
        ui->sbKpr->setValue(t);

        if(t>=0.5)
        {
            if(ui->sbKpr->singleStep()!=0.1)
                ui->sbKpr->setSingleStep(0.1);
        }
        else if( t<0.5 && t>=0.2)
        {
            if(ui->sbKpr->singleStep()!=0.05)
                ui->sbKpr->setSingleStep(0.05);
        }
        else
        {
            if(ui->sbKpr->singleStep()!=0.01)
                ui->sbKpr->setSingleStep(0.01);
        }

        ui->dialKpr->setValue(fabs(t*100.0));
        ui->regRev->setChecked(src[IX(Kpr)].valueReal()<0.0);

        ui->sbKpr->blockSignals(false);
        ui->dialKpr->blockSignals(false);
        ui->regRev->blockSignals(false);
    } catch (...) {
    }




    // KTi
    //qDebug() << "Ti"<< IX(TI] << ":" << src.getValueFloat(IX(TI]);
    try {
        ui->sbTi->blockSignals(true);
        ui->dialTi->blockSignals(true);

        ui->sbTi->setValue(static_cast<double>(src[IX(TI)])/60.0);
        ui->dialTi->setValue(static_cast<double>(src[IX(TI)])*100.0/60.0);

        ui->sbTi->blockSignals(false);
        ui->dialTi->blockSignals(false);

    } catch (...) {
    }

    // KTd
    //qDebug() << "Td"<< IX(Td] << ":" << src[IX(Td]);

    try {
        ui->sbTd->blockSignals(true);
        ui->dialTd->blockSignals(true);

        ui->sbTd->setValue(src[IX(Td)]);
        ui->dialTd->setValue(static_cast<double>(src[IX(Td)])*1000.0);

        ui->dialTd->blockSignals(false);
        ui->sbTd->blockSignals(false);

    } catch (...) {

    }


    // Xmin
    ui->sbXmin->blockSignals(true);
    SAFE(ui->sbXmin->setValue(static_cast<double>(src[IX(Xmin)])*src[IX(Xmin)].k()));
    ui->sbXmin->blockSignals(false);

    // Xmax
    ui->sbXmax->blockSignals(true);
    SAFE(ui->sbXmax->setValue(static_cast<double>(src[IX(Xmax)])*src[IX(Xmax)].k()));
    ui->sbXmax->blockSignals(false);


    // X
    ui->sbX->blockSignals(true);
    SAFE(ui->sbX->setValue(src[IX(X)]));
    ui->sbX->blockSignals(false);

    ui->vsX->blockSignals(true);
    SAFE(ui->vsX->setValue(src[IX(X)].value32()));
    SAFE(ui->vsX->setMaximum(src[IX(PV_1)].baseScale()));
    ui->vsX->blockSignals(false);


    // AM
    try{
        ui->cbAM->blockSignals(true);
        ui->cbAM->setCurrentIndex(src[IX(AM)]?1:0);

        if(src[IX(AM)]) // вимкнути чи ввімкнути управління змінною X
        {
            ui->sbX->blockSignals(true);
            ui->vsX->blockSignals(true);
        }
        else
        {
            ui->sbX->blockSignals(false);
            ui->vsX->blockSignals(false);
        }
    }
    catch (...)
    {
        ui->sbX->blockSignals(false);
        ui->vsX->blockSignals(false);
    }
    ui->cbAM->blockSignals(false);

    // Rej
    try{
        ui->cbRej->show();
        ui->cbRej->blockSignals(true);
        ui->cbRej->setCurrentIndex(src[IX(Rej)]?1:0); // Нафіга тут така довбанути конструпція
    }
    catch (...)
    {
        ui->cbRej->hide();
    }
    ui->cbRej->blockSignals(false);


    // Rev
    try
    {
        if(src[IX(Rev)].value32())
        {
            ui->rRev->blockSignals(true);
            ui->rRev->setChecked(true);
            ui->rRev->blockSignals(false);
        }
        else
        {
            ui->pRev->blockSignals(true);
            ui->pRev->setChecked(true);
            ui->pRev->blockSignals(false);
        }
        ui->gbRev->show();
    }
    catch (...) {
        ui->gbRev->hide();
    }


    // P0
    try
    {
        ui->sbP0->show();
        ui->sbP0->blockSignals(true);
        ui->sbP0->setValue(static_cast<double>(src[IX(P0)])/src[IX(P0)].k());

        ui->slP0->show();
        ui->slP0->blockSignals(true);
        ui->slP0->setValue(src[IX(P0)]);
    }
    catch (...)
    {
        ui->slP0->hide();
        ui->sbP0->hide();
    }
    ui->sbP0->blockSignals(false);
    ui->slP0->blockSignals(false);

    // Mode
    try
    {
        ui->regMode->show();
        ui->regMode->setChecked(src[IX(Mode)]);
    }
    catch (...)
    {
        ui->regMode->hide();

    }

    // To
    try
    {
        ui->to->show();
        ui->to->blockSignals(true);

        int v=src[IX(To)];
        if (v<0) v=0;
        if (v>2) v=2;

        ui->to->setCurrentIndex(v);
    }
    catch (...)
    {
        ui->to->hide();
    }
    ui->to->blockSignals(false);


    double base=4000.0;
    SAFE(base=src[IX(PV_1)].baseScale());

    if(trChart!=nullptr)
        trChart->deleteLater(); // видалити якщо існує.
    trChart= new TrendChart(ui->trendChar,3600,5,base);
    tL->addWidget(trChart);

    updateTrend(ui->cbInterval->currentIndex());

    updateData();
    trChart->addPoint(v); //

    setWindowTitle(ui->regList->currentText());

}


void AuPanelReg::Control(bool v) // відображення-приховувавння частини вікна з настройками регулятора
{
        ui->RegParm->setVisible(v);
}


void AuPanelReg::runTrend()
{

}


void AuPanelReg::updateTrend(int len)
{
    // перезарядити таймер
    int ti[3]={5000,2500,1250};
    quint32 tm[3]={3600,1800,900};

    t1->stop();
    t1->setInterval(ti[len]);


    // QString sQuery="SELECT Dt,%1 FROM %4 WHERE Dt BETWEEN %2 AND %3 ORDER BY Dt";
    QStringList fields;

    for(int i=1;i<9;++i)
    {
        if(RegDes[RegNum][i].size() > 0)
            fields << RegDes[RegNum][i];
        else
            fields << "0";
    }
    // qDebug() << fields.join(',');

    quint32 dt=QDateTime::currentDateTime().toTime_t();

    htrId= myBase.queryHtrData(myBase[plcIx].name(),fields.join(','),dt-tm[len],dt);
    // qDebug() << "htrId " << htrId << " dt " << dt << "dt-tm[len] " << dt-tm[len];
    trChart->fill(0.0); // очистити графік

}


void AuPanelReg::updateData() // поновлення даних у віджетах
{
    v.clear();
    // PV_1
    try {
        ui->lePV_1->setText(src[IX(PV_1)]);

        int t1 = src[IX(PV_1)].value32();
        if(t1<0) t1=0;
        if(t1>4000) t1=4000;
        ui->pbPV_1->setValue(t1);
        v << t1;

    } catch (...) {}

    // PV_2
    try {
        ui->lePV_2->setText(src[IX(PV_2)]);

        int t1 = src[IX(PV_2)].value32();
        if(t1<0) t1=0;
        if(t1>4000) t1=4000;
        ui->pbPV_2->setValue(t1);
        v << t1;

    } catch (...) {
        v << 0.0;
    }

    // PV_3
    try {
        ui->lePV_3->setText(src[IX(PV_3)]);

        int t1 = src[IX(PV_3)].value32();
        if(t1<0) t1=0;
        if(t1>4000) t1=4000;
        ui->pbPV_3->setValue(t1);
        v << t1;

    } catch (...) {
        v << 0.0;
    }


    // SPR_1

    //if(src.getTags().contains(IX(SPR_1]))
    try
    {
        ui->leSPR_1->setText(src[IX(SPR_1)]);

        ui->vsSPR_1->setValue(src[IX(SPR_1)].value32());
        v << src[IX(SPR_1)].value32();
    }
    catch (...){
        v << 0.0;
    }


    // X
    if(ui->cbAM->currentIndex()==1) // якщо в автоматичному режимі то поновити дані
    {
        SAFE(ui->sbX->setValue(src[IX(X)]));
        SAFE(ui->vsX->setValue(src[IX(X)].value32()));
    }
    try {
        v << src[IX(X)].valueReal();
    } catch (...)
    {
            v<< 0.0;
    }

// малювати графіки
//     // SP_1
    try {
        v << src[IX(SP_1)].valueReal();
    } catch (...) {
        v << 0.0;
    }

    // SP_2
    try {
        v << src[IX(SP_2)].valueReal();
    } catch (...)
    {
        v << 0.0;
    }

    // SP_3
    try {
        v << src[IX(SP_3)].valueReal();
    } catch (...) {
        v << 0.0;
    }

    update();
    // qDebug() << "to graph " <<  v;
}


// і тут буде ще купа сигналів від різних контролів
// Ctrl
void AuPanelReg::setCtrlValue(double v)
{
    SAFE(src[IX(X)]=v);

    ui->vsX->blockSignals(true);
    ui->vsX->setValue((double)v/src[IX(X)].k());
    ui->vsX->blockSignals(false);
}


void AuPanelReg::setCtrlValue(int v)
{
    double td=0;
    // qDebug() << " AuPanelReg::setCtrlValue " << IX(X) << v << src[IX(X)].k();

    SAFE(td=v*src[IX(X)].k());

    SAFE(src[IX(X)]=td);

    ui->sbX->blockSignals(true);
    ui->sbX->setValue(td);
    ui->sbX->blockSignals(false);
}



void AuPanelReg::setParmValue(double v)
{
    //qDebug() << "setParmValue(double v)" << sender()->objectName() << ":" << v;


    if(ctrlSearch.contains(sender()->objectName())) // пошукати вадправника
  { // якщо не знайдено
    Ri::Index ix=ctrlSearch[sender()->objectName()];
    //qDebug() << "Tag" << RegDes[RegNum][ix];
    QString tag=RegDes[RegNum][ix];

    switch( ix )
    {
        case Ri::Kpr : // sbKpr
            SAFE(src[tag]=v* (ui->regRev->isChecked()?-1.0:1.0));

            ui->dialKpr->blockSignals(true);
            ui->dialKpr->setValue(v*100.0);
            ui->dialKpr->blockSignals(false);
	    break;
        case Ri::TI : // sbTi
            SAFE(src[tag]=v*60.0);

            ui->dialTi->blockSignals(true);
            ui->dialTi->setValue(abs(v*100.0));
            ui->dialTi->blockSignals(false);
	    break;
        case Ri::Td : // sbTd
            SAFE(src[tag]=v);

            ui->dialTd->blockSignals(true);
            ui->dialTd->setValue(v*1000.0);
            ui->dialTd->blockSignals(false);
	    break;

        case Ri::Xmin: // sbXmin
        case Ri::Xmax: // sbXmax
            SAFE(src[tag]=v/src[tag].k());
	    break;
        case Ri::K_1: // sbK_1
            SAFE(src[tag]=v*kk_1);
            break;

        case Ri::K_2: // sbK_2
        case Ri::K_3: // sbK_3
        case Ri::K_4: // sbK_4
        case Ri::Kkor: // sbKkor
            //qDebug() << "sendPAram" << RegDes[RegNum][ix];
            SAFE(src[tag]=v);
	    break;

        case Ri::SP_1: // sbSP_1
            SAFE(src[tag]=v);

            ui->vsSP_1->blockSignals(true);
            SAFE(ui->vsSP_1->setValue((v-src[tag].b())/src[tag].k()));
            ui->vsSP_1->blockSignals(false);
            // qDebug() << v << ui->vsSP_1->value();
	    break;
        case Ri::SP_2: // sbSP_2
            SAFE(src[tag]=v);

            ui->vsSP_2->blockSignals(true);
            SAFE(ui->vsSP_2->setValue((v-src[tag].b())/src[tag].k()));
            ui->vsSP_2->blockSignals(false);
	    break;
        case Ri::SP_3: // sbSP_3
            SAFE(src[tag]=v);

            ui->vsSP_3->blockSignals(true);
            SAFE(ui->vsSP_3->setValue((v-src[tag].b())/src[tag].k()));
            ui->vsSP_3->blockSignals(false);
	    break;
        case Ri::P0: // sbP0
            SAFE(src[tag]=v*src[tag].k());

            ui->slP0->blockSignals(true);
            SAFE(ui->slP0->setValue(v*src[tag].k()));
            ui->slP0->blockSignals(false);
            break;
	default: // якщо щось незрозуміле то не відправляти
            qDebug() << "Index not fount" << sender()->objectName();
	    break;
    }
    // src.sendValue("Save",qint16(-1));

  }
  else
      qDebug() << "Sender not found" << sender()->objectName();

}


void AuPanelReg::setParmValue(int v)
{
    //qDebug() << "setParmValue(int v)" << sender()->objectName() << ":" << v;

  if(ctrlSearch.contains(sender()->objectName())) // пошукати вадправника
  { // якщо не знайдено
    Ri::Index ix=ctrlSearch[sender()->objectName()];
    //qDebug() << "Tag" << RegDes[RegNum][ix];
    QString tag=RegDes[RegNum][ix];
      switch( ix )
      {
        case Ri::SP_1: // vsSP_1
            //src.sendValue(RegDes[RegNum][ix].split('.')[0],(double)v);
            SAFE(src[tag].setValue(v);)

            ui->sbSP_1->blockSignals(true);
            SAFE(ui->sbSP_1->setValue(static_cast<double>(v)*src[tag].k() + src[tag].b()));
            ui->sbSP_1->blockSignals(false);
        break;
        case Ri::SP_2: // vsSP_2
            SAFE(src[tag].setValue(v));

            ui->sbSP_2->blockSignals(true);
            SAFE(ui->sbSP_2->setValue(static_cast<double>(v)*src[tag].k() + src[tag].b()));
            ui->sbSP_2->blockSignals(false);
        break;
        case Ri::SP_3: // vsSP_3
            SAFE(src[tag].setValue(v));

            ui->sbSP_3->blockSignals(true);
            SAFE(ui->sbSP_3->setValue(static_cast<double>(v)*src[tag].k() + src[tag].b()));
            ui->sbSP_3->blockSignals(false);
        break;
        case Ri::Kpr: // dialKpr
            SAFE(src[tag]=static_cast<double>(v)/100.0*(ui->regRev->isChecked()?-1.0:1.0));

            ui->sbKpr->blockSignals(true);
            ui->sbKpr->setValue(static_cast<double>(v)/100.0);
            ui->sbKpr->blockSignals(false);
        break;
        case Ri::TI: // dialTi
            SAFE(src[tag]=(static_cast<double>(v)/100.0*60.0));

            ui->sbTi->blockSignals(true);
            ui->sbTi->setValue((double)v/100.0);
            ui->sbTi->blockSignals(false);
        break;
        case Ri::Td: // dialTd
            SAFE(src[tag]=static_cast<double>(v)/1000.0);

            ui->sbTd->blockSignals(true);
            ui->sbTd->setValue(static_cast<double>(v)/1000.0);
            ui->sbTd->blockSignals(false);
        break;
        case Ri::P0:
            SAFE(src[tag]=v);

            ui->sbP0->blockSignals(true);
            SAFE(ui->sbP0->setValue((double)v/src[tag].k()));
            ui->sbP0->blockSignals(false);
            break;

    default: // якщо щось незрозуміле то не відправляти
            qDebug() << sender()->objectName();
        break;
    }
      // src.sendValue("Save",qint16(-1));
  }
  else
      qDebug() << "Sender not found" << sender()->objectName();

}


void AuPanelReg::setParmAM(int v)
{
    if(v==1)
    {
        ui->sbX->blockSignals(true);
        ui->vsX->blockSignals(true);
    }
    else
    {
        ui->sbX->blockSignals(false);
        ui->vsX->blockSignals(false);
    }

    SAFE(src[IX(AM)]=qint16(-v));

}


void AuPanelReg::setParmRej(int v)
{
    SAFE(src[IX(Rej)]=qint16(-v));

}


void AuPanelReg::setParmRev()
{
    qint16 v=0;

    if(sender()->objectName()=="rRev")
        v=-1;
    SAFE(src[IX(Rev)]=v);

}

void AuPanelReg::setParmKprSig(int v)
{
    double tmp;
    tmp=ui->sbKpr->value() * (v?-1.0:1.0);
    SAFE(src[IX(Kpr)]=tmp);

}

void AuPanelReg::setParamMode(bool v)
{
    SAFE(src[IX(Mode)]=qint16(v?-1:0));
}

void AuPanelReg::setTO(int v)
{
    SAFE(src[IX(To)]=qint16(v==0?0:-1));
}

void AuPanelReg::setGraph()
{
    trChart->addPoint(v);
    // qDebug() << v;
}

void AuPanelReg::htrDataLoad(qint32 index, QByteArray data, int row, int col)
{
    if(index==htrId)
    {
        trChart->loadData( data,row,col);
    }
    // qDebug() << index << data.size();

    t1->start();
}
