#include "apsgkl.h"
#include "ui_apsgkl.h"
#include <aulib.h>
#include <QCursor>
#include <QRect>
#include <QFile>
#include <QRegExp>

#include <QDebug>

// список параметрів які сюди передаються в змінній tags
// Команда,кінцевик,Перемикач_АМ

#define VAL myTags[0]
#define COIL myTags[1]
#define SWAM myTags[2]

ApSgKl::ApSgKl(AuPlc &plc, QStringList &tags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApSgKl),
    myPlc(plc),
    plcBM((new BitMask(plc,this))[0]),
    myTags(tags)
{
    ui->setupUi(this);


    // виставити вікно в координати миші
    QRect g=geometry();
    g.setX(QCursor::pos().x()-g.width()/2);
    g.setY(QCursor::pos().y()-g.height()/2);
    setGeometry(g);

    //qDebug() << tags;
    SAFE(ui->lbName->setText(myPlc[VAL].text()));
    SAFE(ui->cbVal->setChecked(myPlc[VAL]));

    connect(&myPlc,&AuPlc::updateData,this,&ApSgKl::slotUpdate);
    connect(ui->bnAM,&QPushButton::clicked,this,&ApSgKl::slotSetAm);
    connect(ui->cbVal,&QCheckBox::clicked,this,&ApSgKl::slotSetVal);

    slotSetAm();

    if(myTags.size()<3) // якщо якусь фігню передали
        reject();
}

ApSgKl::~ApSgKl()
{
    delete ui;
}

void ApSgKl::slotUpdate()
{
    bool val=false;
    SAFE(val=plcBM[COIL]);
    ui->lbState->setText(val?QStringLiteral("Відкрито"):QStringLiteral("Закрито"));
    ui->lbState->setStyleSheet(val?QStringLiteral("color: green;"):QStringLiteral("color: red;"));
    // qDebug() << VAL << (bool)myPlc[VAL];

}

void ApSgKl::slotSetAm()
{
    SAFE(AM=myPlc[SWAM]);

    if(sender()!=0) // якщо цесигнал від кнопки, тоді перемкнути його
    {
        AM^=true; // переключити
        SAFE(myPlc[SWAM]=AM;)
    }
    ui->cbVal->blockSignals(AM);
    ui->bnAM->setStyleSheet(AM?QStringLiteral("background-color: green;"):QStringLiteral("background-color: red;"));
    ui->bnAM->setText(AM?QStringLiteral("Автомат"):QStringLiteral("Ручний"));
}

void ApSgKl::slotSetVal(bool v)
{
    SAFE(myPlc[VAL]=v;) // як тут все просто
}

