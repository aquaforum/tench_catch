#include "apsetupone.h"
#include "ui_apsetupone.h"

#include <aulib.h>

ApSetupOne::ApSetupOne(AuPlc &src, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApSetupOne),
    plc(src)

{
    ui->setupUi(this);

    foreach(QDoubleSpinBox *p,this->findChildren<QDoubleSpinBox*>())
    {
        QString tagName=p->objectName();
        bool d=false;
        tagName.right(1).toInt(&d);
        if(d)
           tagName=tagName.left(tagName.size()-2);

        SAFE(p->setValue(src[tagName]));
        connect(p,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&ApSetupOne::slotSetValue);
    }


    QTime Tm_zatr(0,src["Tm_zatr_min_zad"],src["Tm_zatr_sec_zad"]);
//    qDebug() << Tm_zatr << src["Tm_zatr_min_zad"].value() << src["Tm_zatr_sec_zad"].value() ;

    ui->Tm_zatr_zad->setTime(Tm_zatr);
    connect(ui->Tm_zatr_zad,&QTimeEdit::timeChanged,this,&ApSetupOne::slotSetTime);

    QList<QCheckBox*> cb;
    cb << ui->Water_avt
       << ui->Sy20a_sw
       << ui->Slow_par
       << ui->Plan_g;

    foreach(QCheckBox *p, cb)
    {
        p->setChecked(plc[p->objectName()]);
        connect(p,&QCheckBox::clicked,this,&ApSetupOne::slotSetBool);
    }




    SAFE (
    if(plc["Temp_vak"])
        ui->Temp_vak_0->setChecked(true);
    else
        ui->Temp_vak_1->setChecked(true);
        )

    bool Temp_vak=plc["Temp_vak"];
    foreach(AuLamp *p,this->findChildren<AuLamp*>(QRegularExpression("Temp_vak_")))
    {
        p->setChecked(p->objectName().right(1)=="0"?Temp_vak:!Temp_vak);
        // qDebug() << p->objectName();
    }

    connect(ui->Temp_vak_0,&QRadioButton::clicked,this,&ApSetupOne::slotSetTempVak);
    connect(ui->Temp_vak_1,&QRadioButton::clicked,this,&ApSetupOne::slotSetTempVak);

    bool Dt_mt=plc["Dt_mt"];
    ui->Dt_mt_00->setChecked(Dt_mt);
    ui->Dt_mt_01->setChecked(!Dt_mt);
    connect(ui->Dt_mt_0,&QRadioButton::clicked,this,&ApSetupOne::slotSetDtMt);
    connect(ui->Dt_mt_1,&QRadioButton::clicked,this,&ApSetupOne::slotSetDtMt);

        SAFE (
        if(plc["Dt_mt"])
            ui->Dt_mt_0->setChecked(true);
        else
            ui->Dt_mt_1->setChecked(true);
            )

            SAFE (
            if(plc["K17_sel"])
                ui->K17_sel_0->setChecked(true);
            else
                ui->K17_sel_1->setChecked(true);
                )

}

ApSetupOne::~ApSetupOne()
{
    delete ui;
}


void ApSetupOne::slotSetValue(double v)
{
    QString tagName=sender()->objectName();
    bool d=false;
    tagName.right(1).toInt(&d);
    if(d)
       tagName=tagName.left(tagName.size()-2);

    SAFE(plc[tagName]=v);
}


void ApSetupOne::slotSetTime(const QTime &v)
{
    plc["Tm_zatr_min_zad"]=v.minute();
    plc["Tm_zatr_sec_zad"]=v.second();
}

void ApSetupOne::slotSetBool(bool v)
{
    SAFE(plc[sender()->objectName()]=qint16(v?-1:0));
}


void ApSetupOne::slotSetDtMt(bool)
{
    bool Dt_mt=ui->Dt_mt_0->isChecked();
    ui->Dt_mt_00->setChecked(Dt_mt);
    ui->Dt_mt_01->setChecked(!Dt_mt);
    SAFE(plc["Dt_mt"]=Dt_mt);

}

void ApSetupOne::slotSetTempVak(bool)
{
    bool Temp_vak=ui->Temp_vak_0->isChecked();

    foreach(AuLamp *p,this->findChildren<AuLamp*>(QRegularExpression("Temp_vak_")))
    {
        p->setChecked(p->objectName().right(1)=="0"?Temp_vak:!Temp_vak);
        // qDebug() << p->objectName();
    }

    SAFE(plc["Temp_vak"]=Temp_vak);
}
