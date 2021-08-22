#include "apone.h"
#include "ui_apone.h"
#include <QStyleOption>
#include <QPainter>
#include <QTimer>

#include <aulib.h>

ApOne::ApOne(AuPlc &plc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApOne),
    src(plc)
{
    ui->setupUi(this);
    setObjectName("ApOne");

    connect(&src,&AuPlc::updateData,this,&ApOne::slotUpdate);

    QStringList ind;
    ind << "Lt1a_ind"
            << "Uroven_ind"
            << "Tt4a_ind"
            << "Termo_ind"
            << "Pt6a_ind"
            << "Vakuum_ind"
            << "Ey20a_ind"
            << "Moment_ind"
            << "Sy20a_ind"
            << "Ha20a_zad";


    foreach(QLabel *p, findChildren<QLabel*>())
    {
        if(ind.contains(p->objectName()) )
            lcdS << p;
    }

    ind.clear();
    ind
            << "Delta"
            << "Delta_ind"
            << "Dt2a_ind"
            << "Cv_ind";

    foreach(QLabel *p, findChildren<QLabel*>())
    {
        if(ind.contains(p->objectName()) )
            le2 << p;
    }



    ui->lbEy20a_ind->setText(QStringLiteral("ЧП циркулятора ВА%1\nперевантажено!").arg(plc.name().right(1)));
// дані масивів kodState та kodColor
#include "kod_data.h"

}

ApOne::~ApOne()
{
    delete ui;
}

void ApOne::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ApOne::slotUpdate()
{
    foreach (QLineEdit *p , findChildren<QLineEdit*>()) {
        SAFE(p->setText(src[p->objectName()]));
    }

    foreach(QLabel *p, lcdS)
    {
        SAFE(p->setText(src[p->objectName()]));
    }

    foreach(QLabel *p, le2)
    {
        double v=src[p->objectName()];
        SAFE(p->setText(QString::number(v,'f',1)));
    }
/*
    foreach(AuLamp *p,findChildren<AuLamp*>())
    {
        SAFE(p->setChecked(src[p->objectName()]!=0));
    }
*/

    ui->lampEy20a_ind->setChecked(ui->Ey20a_ind->text().toInt()>5);

    qint16 Ey20=0;
    SAFE(Ey20=src[QStringLiteral("Ey20a_ind")]);
    if(Ey20<60)
        ui->lbEy20a_ind->hide();
    else
        ui->lbEy20a_ind->show();

    // вивести назву стадії
    int Kod=0;
    SAFE(Kod=src[QStringLiteral("Kod")]);
    if(Kod<0)
        Kod=0;
    if(Kod>kodState.size()-2)
        Kod=kodState.size()-1;
    ui->Kod->setText(kodState[Kod]);
    ui->Kod->setStyleSheet(kodColor[Kod]);

    // можливо його ще треба буде порахувати

    SAFE(ui->pLt1a_ind->setValue(src[QStringLiteral("Lt1a_ind")]));


}

void ApOne::setText(QString msg)
{
    ui->label->setText(msg);
}

void ApOne::blink(bool off)
{

    if(ui->Ey20a_ind->text().toInt()<=5) // якщо треба блінкати
    {
        ui->lampEy20a_ind->setLampOff(off?QStringLiteral(":/images/redRect.png"):QStringLiteral(":/images/blackRect.png"));
        //ui->lampEy20a_ind->repaint();
    }

    // мигаємо написом "ЧП перевантажено"
    ui->lbEy20a_ind->setStyleSheet(off?QStringLiteral("color: red"):QStringLiteral("color: black"));



}

