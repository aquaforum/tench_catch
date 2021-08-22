#include "dlgapone.h"
#include "ui_dlgapone.h"
#include "apone.h"
#include "iodev.h"
#include "dlgapsetup.h"

#include <QtGui>
#include <QMessageBox>

DlgApOne::DlgApOne(IoDev &source,QWidget *parent) :
    QDialog(parent),
    src(source),
    ui(new Ui::DlgApOne)
{
    ui->setupUi(this);

    ap=new ApOne(/*src,*/ui->apMnemo);

    connect(ui->bnSetup,SIGNAL(clicked()),this,SLOT(slotCallSetup()));

    connect(ui->bnStop,SIGNAL(clicked()),this,SLOT(slotCommand()));
    connect(ui->bnStart,SIGNAL(clicked()),this,SLOT(slotCommand()));
    connect(ui->bnConcretion,SIGNAL(clicked()),this,SLOT(slotCommand()));
    connect(ui->bnRarefaction_1,SIGNAL(clicked()),this,SLOT(slotCommand()));
    connect(ui->bnRarefaction_2,SIGNAL(clicked()),this,SLOT(slotCommand()));
    connect(ui->bnGrowth,SIGNAL(clicked()),this,SLOT(slotCommand()));

    // заповнення хешу команд

    cmd[ui->bnStop->objectName()]=1;
    cmd[ui->bnStart->objectName()]=3;
    cmd[ui->bnConcretion->objectName()]=4;
    cmd[ui->bnRarefaction_1->objectName()]=8;
    cmd[ui->bnRarefaction_2->objectName()]=14;
    cmd[ui->bnGrowth->objectName()]=20;


    lastKor=src.getValueScaled("Kor");
    ui->sb_Kor->setValue(lastKor);
    ui->dl_Kor->setValue(lastKor);

    connect(ui->sb_Kor,SIGNAL(valueChanged(int)),this,SLOT(slotSetKor(int)));
    connect(ui->dl_Kor,SIGNAL(valueChanged(int)),this,SLOT(slotSetKor(int)));


    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateData()));

    state
            << tr("Невизначено") //0
            << tr("Очікування")  // 1
            << tr("Очікування")  // 2
            << tr("Набір")       // 3
            << tr("Згущення")    // 4
            << tr("Затравка")    // 5
            << tr("Згущення")    // 6
            << tr("Поличка")     // 7
            << tr("Розкачака 1") // 8
            << tr("Згущення")    // 9
            << tr("Розкачка 2")  // 10
            << tr("Згущення")    // 11
            << tr("Розкачка 3")  // 12
            << tr("Згущення")    // 13

            << tr("Розкачака 4") // 14
            << tr("Згущення")    // 15
            << tr("Розкачка 5")  // 16
            << tr("Згущення")    // 17
            << tr("Розкачка 6")  // 18
            << tr("Згущення")    // 19

            << tr("Ріст 1")      // 20
            << tr("Ріст 2")      // 21
            << tr("Ріст 3")      // 22
            << tr("Уварювання")  // 23
            << tr("Вивантаження") // 24
    << tr("Розкачака 1") // 25
    << tr("Згущення")    // 26
    << tr("Розкачка 2")  // 27
    << tr("Згущення")    // 28
    << tr("Розкачка 3")  // 29
    << tr("Згущення") ;   // 30

}

DlgApOne::~DlgApOne()
{
    delete ui;
}

void DlgApOne::changeEvent(QEvent *e)
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


void DlgApOne::slotCallSetup()
{
        DlgApSetup p(src,this);
        p.setWindowTitle(QString(tr("Параметри апарату №%1")).arg(windowTitle().right(1)));
        p.exec();
}

void DlgApOne::slotCommand()
{

    if(sender()->objectName()=="bnStop")
    {
        if(QMessageBox::warning(this,tr("Попередження"),tr("Зупинити ?"),
            QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes)
        {
            src.sendValue("State",cmd[sender()->objectName()]);
        }

    }
    else
    {
        src.sendValue("State",cmd[sender()->objectName()]);

    }

}
void DlgApOne::slotSetKor(int v)
{
    if(sender()->objectName()=="sb_Kor")
    {
        ui->dl_Kor->blockSignals(true);
        ui->dl_Kor->setValue(v);
        ui->dl_Kor->blockSignals(false);
    }
    else
    {
        ui->sb_Kor->blockSignals(true);
        ui->sb_Kor->setValue(v);
        ui->sb_Kor->blockSignals(false);
    }
    lastKor=v;

    src.sendValueScaled("Kor",(double)v);
   qDebug() << "kor = " << src.getValueScaled("Kor");
}


void DlgApOne::updateData()
{
    int sta=src.getValue16("State");
    if(sta>-1 && sta<31)
        ui->le_State->setText(state[sta]);
    else
        ui->le_State->setText(tr("Невизначено"));

    if(lastKor!=src.getValueScaled("Kor"))
    {
        lastKor=src.getValueScaled("Kor");
        ui->dl_Kor->blockSignals(true);
        ui->dl_Kor->setValue(lastKor);
        ui->dl_Kor->blockSignals(false);
        ui->sb_Kor->blockSignals(true);
        ui->sb_Kor->setValue(lastKor);
        ui->sb_Kor->blockSignals(false);
    }
    ap->updateData(src);

}

void DlgApOne::slotSendProd(int v)
{
    src.sendValue("prod",qint16(v));
}

