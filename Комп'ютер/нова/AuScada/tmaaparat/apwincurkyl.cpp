#include "apwincurkyl.h"
#include "ui_apwincurkyl.h"
#include <aulib.h>
#include "valve_mask.h"

// визначити необхідні теги
#define AMSW QStringLiteral("Ap_k20")
#define VAL QStringLiteral("K20_out")
#define STATE QStringLiteral("Ey20a_ind")
#define CONTROL QStringLiteral("Ha20a_zad")

//  це міститься в запакованому виді,
#define FAULT QStringLiteral("M1_A_in")
#define REMOTE QStringLiteral("M1_R_in")

ApWinCurkyl::ApWinCurkyl(AuPlc &plc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApWinCurkyl),
    myPlc(plc),
    AM(false)
{
    ui->setupUi(this);

    // виставити вікно в координати миші
    QRect g=geometry();
    g.setX(QCursor::pos().x()-g.width()/2);
    g.setY(QCursor::pos().y()-g.height()/2);
    setGeometry(g);

    SAFE(AM=myPlc[AMSW]);
    SAFE(ui->Ha20a_zad->setValue(myPlc[CONTROL]));
    SAFE(ui->cbVal->setChecked(myPlc[VAL]));

    connect(&myPlc,&AuPlc::updateData,this,&ApWinCurkyl::slotUpdate);

    connect(ui->bnAM,&QPushButton::clicked,this,&ApWinCurkyl::slotSetAm);
    connect(ui->cbVal,&QCheckBox::clicked,this,QOverload<bool>::of(&ApWinCurkyl::slotSetVal));
    connect(ui->Ha20a_zad,QOverload<int>::of(&QSpinBox::valueChanged),this,QOverload<int>::of(&ApWinCurkyl::slotSetVal));

    slotSetAm(); // пофарбувати кнопку
}

ApWinCurkyl::~ApWinCurkyl()
{
    delete ui;
}

void ApWinCurkyl::slotUpdate()
{
    qint16 st=0;
    SAFE(st=myPlc[STATE]);
    ui->lbState->setText(st>5?QStringLiteral("Увімкнено"):QStringLiteral("Вимкнено"));
    ui->lbState->setStyleSheet(st>5?QStringLiteral("color: green;"):QStringLiteral("color: red;"));

    SAFE(ui->M1_r->setChecked(myPlc[REMOTE]));

    ui->M1_r->setChecked(((qint16)myPlc[QStringLiteral("St_i2")])&MASK_M1_R_in);

    if((qint16)myPlc[QStringLiteral("St_i1")]&MASK_M1_A_in)
    {
        ui->lbM1_A_in->show();
        ui->lbM1_A_in->setText(QStringLiteral("Аварія ЧП"));
    }
    else
    {
        ui->lbM1_A_in->hide();
        ui->lbM1_A_in->setText(QStringLiteral(""));
    }

}

void ApWinCurkyl::slotSetAm()
{
    SAFE(AM=myPlc[AMSW]);

    if(sender()!=0) // якщо цесигнал від кнопки, тоді перемкнути його
    {
        AM^=true; // переключити
        SAFE(myPlc[AMSW]=AM;)
    }
    ui->cbVal->blockSignals(AM);
    ui->Ha20a_zad->blockSignals(AM);

    ui->bnAM->setStyleSheet(AM?QStringLiteral("background-color: green;"):QStringLiteral("background-color: red;"));
    ui->bnAM->setText(AM?QStringLiteral("Автомат"):QStringLiteral("Ручний"));
}

void ApWinCurkyl::slotSetVal(bool v)
{
    SAFE(myPlc[VAL]=v);
}

void ApWinCurkyl::slotSetVal(int v)
{
    SAFE(myPlc[CONTROL]=v);
}

