#include "apmainform.h"
#include "ui_apmainform.h"
#include <aulib.h>

#include <QStackedWidget>
#include <QDateTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QDateTime>
#include <QTextBrowser>

#include "mainwidget.h"

#include "tmaaparat/apcommon.h"
#include "tmaaparat/apsingle.h"
#include "tmaaparat/aptrendselect.h"
#include "tmaaparat/apvatrendselect.h"
#include "tmaaparat/apzagtrendselect.h"
#include "tmaaparat/errormanager.h"
#include "tmaaparat/apsetupcommon.h"
#include "tmaaparat/apsetupone.h"

#define TAP1 QStringLiteral("tmaap1")
#define TAP2 QStringLiteral("tmaap2")
#define TAP3 QStringLiteral("tmaap3")
#define TAPA QStringLiteral("tmaob")

ApMainForm::ApMainForm(AuBase &base, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApMainForm),
    src(base),
    currentMnemo(nullptr)

{
    ui->setupUi(this);
    ui->lbAlert->setText(QStringLiteral(""));

    // connect(ui->bnReturn,&QPushButton::clicked,this,&ApMainForm::slotShowFab);
    connect(ui->bnReturn,&QPushButton::clicked,qobject_cast<MainWidget*>(parent),&MainWidget::slotShowMain);

    //vbl=new QVBoxLayout(ui->frameMnemo);
    //vbl->setMargin(0);
    //ui->frameMnemo->setLayout(vbl);
    currentMnemo =new ApCommon(src);
    //vbl->addWidget(currentMnemo);
    // ui->frameMnemo->takeWidget()->deleteLater();
    ui->frameMnemo->setWidget(currentMnemo);



    connect(ui->bnMain,&QPushButton::clicked,this,&ApMainForm::slotChangeMnemo);
    connect(ui->bnVA1,&QPushButton::clicked,this,&ApMainForm::slotChangeMnemo);
    connect(ui->bnVA2,&QPushButton::clicked,this,&ApMainForm::slotChangeMnemo);
    connect(ui->bnVA3,&QPushButton::clicked,this,&ApMainForm::slotChangeMnemo);

    connect(ui->bnTrend,&QPushButton::clicked,this,&ApMainForm::slotChangeMnemo);
    connect(ui->bnAlert,&QPushButton::clicked,this,&ApMainForm::slotChangeMnemo);
    connect(ui->bnSetup,&QPushButton::clicked,this,&ApMainForm::slotChangeMnemo);

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    connect(t,&QTimer::timeout,this, &ApMainForm::slotSetTime);
    t->start();    

    ErrorManager *i1=new ErrorManager(src[TAP1],":/text/vaalert.list",this);
    connect(i1,&ErrorManager::AlertON,this,&ApMainForm::addAlert);
    connect(i1,&ErrorManager::AlertOFF,this,&ApMainForm::commitAlert);
    i1->setObjectName(QStringLiteral("BA1"));

    ErrorManager *i2=new ErrorManager(src[TAP2],":/text/vaalert.list",this);
    connect(i2,&ErrorManager::AlertON,this,&ApMainForm::addAlert);
    connect(i2,&ErrorManager::AlertOFF,this,&ApMainForm::commitAlert);
    i2->setObjectName(QStringLiteral("BA2"));

    ErrorManager *i3=new ErrorManager(src[TAP3],":/text/vaalert.list",this);
    connect(i3,&ErrorManager::AlertON,this,&ApMainForm::addAlert);
    connect(i3,&ErrorManager::AlertOFF,this,&ApMainForm::commitAlert);
    i3->setObjectName(QStringLiteral("BA3"));


}

ApMainForm::~ApMainForm()
{
    delete ui;
}


void ApMainForm::trRun()
{
    AuHistory htrDialog(src,this,":/history.ui");
    if(htrDialog.exec()==QDialog::Accepted)
    {
       // qDebug() << htrDialog.trendFields();
       AuTrendView *pTrw;
       pTrw  = new AuTrendView(src,htrDialog.trendFields(),parentWidget());
       pTrw->setText(htrDialog.text());

       qobject_cast<MainWidget*>(parentWidget())->slotShowTrend(pTrw);

    }

}
void ApMainForm::slotSetTime()
{
    QDateTime cdt=QDateTime::currentDateTime();
    ui->lbDate->setText(cdt.toString("dd/MM/yy"));
    ui->lbTime->setText(cdt.toString("hh:mm:ss"));
}

void ApMainForm::slotShowAlert(QString msg)
{
    ui->lbAlert->setText(msg);
}

// перемикання мнемосхеми
void ApMainForm::slotChangeMnemo()
{
    if(currentMnemo!=nullptr)
    {
        // vbl->removeWidget(currentMnemo);
           // currentMnemo->deleteLater() ; //
    }

    QString btnName=sender()->objectName();

    if(btnName=="bnMain")
    {
        currentMnemo =new ApCommon(src,ui->frameMnemo);
        ui->lbMnemoName->setText(QStringLiteral("[Головний кадр]"));
    }
    else if(btnName=="bnTrend")
    {
        currentMnemo= new ApTrendSelect(src,this,ui->frameMnemo);
        ui->lbMnemoName->setText(QStringLiteral("[Тренди]"));
    }
    else if(btnName=="bnAp1" || btnName=="bnAp2" || btnName=="bnAp3")
    {
        currentMnemo= new  ApVaTrendSelect(src,ui->frameMnemo);
        currentMnemo->setObjectName(btnName); // передати назву апарата
        ui->lbMnemoName->setText(QStringLiteral("[Тренди вакуум-апарату %1]").arg(btnName.right(1)));
    }
    else if(btnName=="bnAp4" )
    {
        currentMnemo= new  ApZagTrendSelect(src,ui->frameMnemo);
        ui->lbMnemoName->setText(QStringLiteral("[Тренди загального обладнання]"));
    }
    else if(btnName=="bnVA1" || btnName=="bnVA2" || btnName=="bnVA3" )
    {
        currentMnemo= new ApSingle(src[QStringLiteral("tmaap%1").arg(sender()->objectName().right(1))],src,ui->frameMnemo);
        ui->lbMnemoName->setText(QStringLiteral("[Вакуум-апарат №%1]").arg(sender()->objectName().right(1)));
    }
    else if(btnName=="bnAlert")
    {
        QTextBrowser *tb=new QTextBrowser(ui->frameMnemo);
        currentMnemo= tb;
        tb->setGeometry(0,0,ui->frameMnemo->geometry().width(),ui->frameMnemo->geometry().height());

        foreach (QString s, lastAlert) {
            tb->insertPlainText(s);
        }
    }
    else if(btnName=="bnSetup")
    {
        currentMnemo= new ApSetupCommon(src,this,ui->frameMnemo);
        ui->lbMnemoName->setText(QStringLiteral("[Налаштування]"));
    }
    else if(btnName=="bnApSetup_1" ||
            btnName=="bnApSetup_2" ||
            btnName=="bnApSetup_3" )
    {
        currentMnemo=new ApSetupOne(src[QStringLiteral("tmaap%1").arg(sender()->objectName().right(1))],ui->frameMnemo);
        ui->lbMnemoName->setText(QStringLiteral("[Налаштування ВА%1]").arg(sender()->objectName().right(1)));
    }

    else
    {
        qDebug() << sender();
    }
    ui->frameMnemo->setWidget(currentMnemo);

    // vbl->addWidget(currentMnemo);

    // qDebug() << ui->frameMnemo->children();


}

void ApMainForm::addAlert(qint32 ix,QString msg)
{
    QDateTime cdt=QDateTime::currentDateTime();
    ixAlert[ix]=QStringLiteral("%1 %2\t%3")
                             .arg(cdt.toString("hh:mm:ss"))
                             .arg(sender()->objectName()).arg(msg);

    ui->lbAlert->setText(ixAlert[ix]);

    ui->lbAlert->setStyleSheet(QStringLiteral("color: yellow"));
    lastIx=ix;
}

void ApMainForm::commitAlert(qint32 ix)
{
    QDateTime cdt=QDateTime::currentDateTime();

    if(ixAlert.contains(ix))
    {
        ixAlert[ix].insert(9,cdt.toString("hh:mm:ss "));
        ixAlert[ix].append("\n");
        lastAlert << ixAlert[ix];

        ixAlert.remove(ix);

        while(lastAlert.size()>100)
            lastAlert.removeAt(0);
    }
    if(ix==lastIx) //
        ui->lbAlert->setStyleSheet(QStringLiteral("color: rgb(145, 145, 0)"));

}

