#include "mainform.h"



#include <QDebug>

#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>
#include <QStackedWidget>
#include <QSettings>
#include <QStyleFactory>

#include <aulib.h>

// #include "sparamdialog.h"
#include "about.h"

#include "alertviewdialog.h"
#include "dlgbledingsetup.h"

#include "centrif/cfmnemo.h"
#include "centrif/one_mnemo1.h"
#include "centrif/report.h"

#include "fabreport.h"

#include "gaspich/gpmnemo.h"
#include "gaspich/gpreport.h"

#include "rou/roumnemo.h"
#include "BigPumpsMon/bigpumpsmon.h"
#include "sparamdialog.h"

#include "fabmnemo.h"
#include "tmaaparat/apmainform.h"

#include "mainwidget.h"

#include "boiler2/mnemoboiler2.h"

/*
#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))
#define APARAT (*(src[3]))
#define CENTRIF (*(src[4]))
#define GASPICH (*(src[5]))
#define ROU (*(src[6]))
*/

#define FABMNEMO 0
#define APMNEMO 1
#define CFMNEMO 2
#define GPMNEMO 3
#define ROUMNEMO 4
#define BOILER2MNEMO 5

mMainForm::mMainForm(AuBase &source,QWidget *p): QWidget(p)
,src(source),m_ui(new Ui::Form)
  ,currMnemo(nullptr)
{

    m_ui->setupUi(this);
    
    connect (m_ui->Exit,SIGNAL(clicked()),p,SLOT(close()));

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->setObjectName("Clock_timer");
    t->start();

    connect(t,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(m_ui->About,SIGNAL(clicked()),this,SLOT(about()));
    connect(m_ui->bnParm,SIGNAL(clicked()),this,SLOT(setupParm()));

    connect(m_ui->bn_showAlert,SIGNAL(clicked()),this,SLOT(showAlert()));
    connect(m_ui->bnBleding,SIGNAL(clicked()),this,SLOT(showBleding()));

    // кнопки навігації
    hPos[m_ui->bnDif]=0;
    hPos[m_ui->bnMdef]=1120;
    hPos[m_ui->bn2sat]=2540;
    hPos[m_ui->bnVip]=3400;

    // готуємо лайоут
    currVbl = new QVBoxLayout(m_ui->frameMnemo);
    currVbl->setMargin(0);
    m_ui->frameMnemo->setLayout(currVbl);
    // завантажую гловну мнемосему

    QSettings set;


    switch (set.value("subMnemo",0).toInt()) {
    default:
//    case APMNEMO: // цього тут бути не повинно тому обробляємо як головна мнемосхеми
//    case FABMNEMO:
        fab=new fabMnemo(src,m_ui->frameMnemo);
        fab->horizontalScrollBar()->setValue(set.value("posMnemo",0).toInt());
        fab->horizontalScrollBar()->setStyle(QStyleFactory::create("windows"));
        //qDebug() << QStyleFactory::keys();

        lastBn=m_ui->bnDif;
        currMnemo=fab;
        break;
    case CFMNEMO:
        currMnemo= new CfMnemo(src,this,m_ui->frameMnemo);
        lastBn=m_ui->bnCentrif;
        break;
    case GPMNEMO:
        currMnemo=new GpMnemo(src,m_ui->frameMnemo);
        lastBn=m_ui->bnGaspich;
        break;
    case ROUMNEMO:
        currMnemo=new RouMnemo(src,m_ui->frameMnemo);
        lastBn=m_ui->bnRou;
        break;
    case BOILER2MNEMO:
        currMnemo=new MnemoBoiler2(src,m_ui->frameMnemo);
        lastBn=m_ui->bnBoiler2;
        break;
    }

   currVbl->addWidget(currMnemo);


   // підключити кнопки навігації
   connect(m_ui->bnCentrif,&QPushButton::clicked,this,&mMainForm::slotShowCentrif);

   connect(m_ui->bnDif,&QPushButton::clicked,this,&mMainForm::slotSetPos);
   connect(m_ui->bnMdef,&QPushButton::clicked,this,&mMainForm::slotSetPos);
   connect(m_ui->bn2sat,&QPushButton::clicked,this,&mMainForm::slotSetPos);
   connect(m_ui->bnVip,&QPushButton::clicked,this,&mMainForm::slotSetPos);

   connect(m_ui->bnRou,&QPushButton::clicked,this,&mMainForm::slotShowRou);
   connect(m_ui->bnGaspich,&QPushButton::clicked,this,&mMainForm::slotShowGaspich);

   connect(m_ui->bnBoiler2,&QPushButton::clicked,this,&mMainForm::showBoiler2Mnemo);

   connect(m_ui->bnReportFab,&QPushButton::clicked,this,&mMainForm::slotShowFabReport);
   connect(m_ui->bnReport,&QPushButton::clicked,this,&mMainForm::slotShowCfReport);

   connect(m_ui->bnBigPumps,&QPushButton::clicked,this,&mMainForm::slotShowBp);

   connect(m_ui->bnAparat,&QPushButton::clicked,qobject_cast<MainWidget*>(p),&MainWidget::slotShowAparat);

   connect(m_ui->Trend,&QPushButton::clicked,this,&mMainForm::trRun);

   connect(&source,&AuBase::Alert,m_ui->AlertBrowser,&QTextEdit::setText);

}

mMainForm::~mMainForm()
{
    delete m_ui;
}


void mMainForm::timeOut()
{
    QDateTime tm;
    
    tm=QDateTime::currentDateTime();
    
    m_ui->lineEDate->setText(tm.toString("dd:MM:yyyy"));
    m_ui->lineETime->setText(tm.toString("hh:mm:ss"));
    
}

void mMainForm::about()
{
    aboutDlg *ab=new aboutDlg(this);
    ab->exec();
    delete ab;
}


void mMainForm::trRun()
{
    AuHistory htrDialog(src,this,":/history.ui");
    if(htrDialog.exec()==QDialog::Accepted)
    {
       // qDebug() << htrDialog.trendFields();
       pTrw  = new AuTrendView(src,htrDialog.trendFields(),parentWidget());
       pTrw->setText(htrDialog.text());

       qobject_cast<MainWidget*>(parentWidget())->slotShowTrend(pTrw);

    }

}

void mMainForm::showMe()
{
    //((QStackedWidget*)parentWidget())->setCurrentWidget(this) ;
    //((QStackedWidget*)parentWidget())->removeWidget(pTrw);
    pTrw->deleteLater();
}

void mMainForm::setupParm()
{

    SParamDialog d(src,this);
    d.exec();
}


void mMainForm::slotAlert(QString v)
{
    QString a=QString("%1 | %2").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")).arg(v);
    m_ui->AlertBrowser->clear();
    m_ui->AlertBrowser->insertPlainText( a );

    alertList.push(a);          // зберегти поточний
    for(;alertList.size()>100;) // якщо більше 100
    {
        alertList.pop_front();  // видалити зайві
    }
}

void mMainForm::showAlert()
{
    AlertViewDialog d(alertList,this);
    d.exec();
}

void mMainForm::showBleding()
{
    dlgBledingSetup p(src,this);
    p.exec();
}



// прокрутка по горизонталі
void mMainForm::slotSetPos()
{

    if(fab!=currMnemo) // якщо поточний кадр не мнемосхема завода
    {
        if(lastBn!=sender())
        {
            currVbl->removeWidget(currMnemo);
            currMnemo->deleteLater();

            // завантажую гловну мнемосему
           fab=new fabMnemo(src,m_ui->frameMnemo);
           fab->horizontalScrollBar()->setStyle(QStyleFactory::create("windows"));

           currVbl->addWidget(fab);
           currMnemo=fab;

        }
       lastBn=sender();
    }
    fab->horizontalScrollBar()->setValue(hPos[sender()]);
    QSettings s;
    s.setValue("subMnemo",FABMNEMO);
    s.setValue("posMnemo",fab->horizontalScrollBar()->value());


}



void mMainForm::slotShowCentrif()
{
    if(lastBn!=sender())
    {
        currVbl->removeWidget(currMnemo);
        currMnemo->deleteLater();

        currMnemo=new CfMnemo(src,this,m_ui->frameMnemo);
        currVbl->addWidget(currMnemo);
        QSettings s;
        s.setValue("subMnemo",CFMNEMO);

    }
    lastBn=sender();
}

void mMainForm::showOneMnemo1() // ця функція слугє для виводу мнемосхеми одиночної центрифуги
{

    currVbl->removeWidget(currMnemo);
    currMnemo->deleteLater();
    currMnemo= new OneMnemo1(src,sender()->objectName().right(1).toInt()-4,m_ui->frameMnemo);
    currVbl->addWidget(currMnemo);
    lastBn=sender();
}


void mMainForm::slotShowCfReport()
{
    currVbl->removeWidget(currMnemo);
    currMnemo->deleteLater();
    currMnemo= new GpReport(src,m_ui->frameMnemo);
    currVbl->addWidget(currMnemo);
    lastBn=sender();

}


void mMainForm::slotShowFabReport()
{
    if(lastBn!=sender())
    {
        currVbl->removeWidget(currMnemo);
        currMnemo->deleteLater();
        currMnemo= new FabReport(src,m_ui->frameMnemo);
        currVbl->addWidget(currMnemo);
    }
    lastBn=sender();
}

void mMainForm::slotShowGaspich()
{
    if(lastBn!=sender())
    {
        currVbl->removeWidget(currMnemo);
        currMnemo->deleteLater();
        currMnemo= new GpMnemo(src,m_ui->frameMnemo);
        currVbl->addWidget(currMnemo);
        QSettings s;
        s.setValue("subMnemo",GPMNEMO);

    }
    lastBn=sender();
}

void mMainForm::slotShowRou()
{
    if(lastBn!=sender())
    {
        currVbl->removeWidget(currMnemo);
        currMnemo->deleteLater();
        currMnemo=new RouMnemo(src,m_ui->frameMnemo);
        currVbl->addWidget(currMnemo);
        QSettings s;
        s.setValue("subMnemo",ROUMNEMO);
    }
    lastBn=sender();
}

void mMainForm::showBoiler2Mnemo()
{
    if(lastBn!=sender())
    {
        currVbl->removeWidget(currMnemo);
        currMnemo->deleteLater();
        currMnemo=new MnemoBoiler2(src,m_ui->frameMnemo);
        currVbl->addWidget(currMnemo);
        QSettings s;
        s.setValue("subMnemo",BOILER2MNEMO);
    }
    lastBn=sender();
}


void mMainForm::slotShowBp()
{
    BigPumpsMon pb(src,this);
    pb.exec();
}
