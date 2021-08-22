#include "keyboardwin.h"
#include "ui_keyboardwin.h"
#include <QString>

KeyboardWin::KeyboardWin(double currentValue,double minValue,double maxValue,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyboardWin),
    val(currentValue),
    min(minValue),
    max(maxValue)
{
    ui->setupUi(this);
    ui->sbValue->setMinimum(min);
    ui->sbValue->setMaximum(max);
    ui->sbValue->setValue(val);

    ui->labelMax->setText(QString().number(max));
    ui->labelMin->setText(QString().number(min));

    connect(ui->bn_0,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_1,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_2,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_3,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_4,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_5,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_6,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_7,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_8,SIGNAL(clicked()),this,SLOT(digitClicked()));
    connect(ui->bn_9,SIGNAL(clicked()),this,SLOT(digitClicked()));

    connect(ui->bn_dot,SIGNAL(clicked()),this,SLOT(dotClicked()));
    connect(ui->bn_bs,SIGNAL(clicked()),this,SLOT(bsClicked()));
    connect(ui->bn_sign,SIGNAL(clicked()),this,SLOT(signClicked()));
    connect(ui->bn_Enter,SIGNAL(clicked()),this,SLOT(enterClicked()));


}

KeyboardWin::~KeyboardWin()
{
    delete ui;
}

void KeyboardWin::changeEvent(QEvent *e)
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

void KeyboardWin::digitClicked()
{
    sVal+=sender()->objectName().right(1);

    if(sVal.split(".").size()>1) // є десяткова частина
        ui->sbValue->setDecimals(sVal.split(".")[1].size());
    else
        ui->sbValue->setDecimals(0);

    ui->sbValue->setValue(sVal.toDouble());

}
void KeyboardWin::dotClicked()
{
    sVal+='.';
    ui->sbValue->setValue(sVal.toDouble());
}

void KeyboardWin::bsClicked()
{
    sVal=sVal.left(sVal.size()-1);
    if(sVal.split(".").size()>1) // є десяткова частина
        ui->sbValue->setDecimals(sVal.split(".")[1].size());
    else
        ui->sbValue->setDecimals(0);

    ui->sbValue->setValue(sVal.toDouble());

}

void KeyboardWin::signClicked()
{
    ui->sbValue->setValue(-ui->sbValue->value());
    sVal=QString().number(ui->sbValue->value());
}

void KeyboardWin::enterClicked()
{
    val=ui->sbValue->value();
    accept();
}


