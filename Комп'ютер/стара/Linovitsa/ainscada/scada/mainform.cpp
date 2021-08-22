#include "mainform.h"

#include "mnemo.h"

#include "aparat/apmnemo.h"

#include <QDebug>

#include <QMessageBox>
#include <QScrollBar>

#include <IoNetClient.h>

#include "sparamdialog.h"
#include "about.h"
#include "uhistory.h"
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
#include "BigPumpsMon/xudpclient.h"


#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))
#define APARAT (*(src[3]))
#define CENTRIF (*(src[4]))
#define GASPICH (*(src[5]))
#define ROU (*(src[6]))


mMainForm::mMainForm(QVector<IoNetClient*> &source,QWidget *p): QWidget(p)
,src(source),m_ui(new Ui::Form),oM1(NULL),mRep(NULL),mFRep(NULL)
{

    m_ui->setupUi(this);
    
    connect (m_ui->Exit,SIGNAL(clicked()),p,SLOT(close()));

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->setObjectName("Clock_timer");
    
    connect(t,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(m_ui->About,SIGNAL(clicked()),this,SLOT(about()));
    connect(m_ui->Trend,SIGNAL(clicked()),this,SLOT(trRun()));
    connect(m_ui->bnParm,SIGNAL(clicked()),this,SLOT(setupParm()));


    connect(m_ui->bn_showAlert,SIGNAL(clicked()),this,SLOT(showAlert()));

    connect(m_ui->bnBleding,SIGNAL(clicked()),this,SLOT(showBleding()));


    // відобразити мнемосхему
    m_ui->scrollArea->setWidget(new Mnemo(src,this));
    m_ui->scrollArea->horizontalScrollBar()->setStyleSheet(
"QScrollBar:horizontal {"
"    border: 1px solid grey;"
"margin: 0px 20px 0 20px;"
"    background: lightBlue;}"
"QScrollBar::handle:horizontal {"
"    background: red;}"
    "QScrollBar::add-line:horizontal {"
        "subcontrol-position: right;"
        "subcontrol-origin: margin;"
        "border: 2px solid grey;"
        "background: brown;"
        "width: 20px;}"
    "QScrollBar::sub-line:horizontal {"
        "border: 2px solid grey;"
        "background: brown;"
        "width: 20px;"
        "subcontrol-position: left;"
        "subcontrol-origin: margin;}"

                "QScrollBar:left-arrow:horizontal, QScrollBar::right-arrow:horizontal {"
                    "border: 2px solid grey;"
                    "width: 3px;"
                    "height: 3px;"
                    "background: white;           }"
                "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
                    "background: none;                }"
);

    ApMnemo *aPm = new ApMnemo(*(src[3]),this);
    m_ui->stackedWidget->addWidget(aPm);

    cFm= new CfMnemo(*(src[4]),this);
    m_ui->stackedWidget->addWidget(cFm);


    GpMnemo *gpm =new GpMnemo(src,this);
    m_ui->stackedWidget->addWidget(gpm);


    rmn=new RouMnemo(src,this);

    m_ui->stackedWidget->addWidget(rmn);


    m_ui->stackedWidget->setCurrentIndex(0);


    // відображення свіжих алертів
    connect(src[0],SIGNAL(Alert(QString)),this,SLOT(slotAlert(QString)));
    connect(src[1],SIGNAL(Alert(QString)),this,SLOT(slotAlert(QString)));
    connect(src[2],SIGNAL(Alert(QString)),this,SLOT(slotAlert(QString)));
    connect(src[3],SIGNAL(Alert(QString)),this,SLOT(slotAlert(QString)));

    
    t->start();

    // кнопки навігації
    hPos[m_ui->bnDif]=0;
    hPos[m_ui->bnMdef]=1120;
    hPos[m_ui->bn2sat]=2540;
    hPos[m_ui->bnVip]=3400;

    connect(m_ui->bnDif,SIGNAL(clicked()),this,SLOT(slotSetPos()));
    connect(m_ui->bnMdef,SIGNAL(clicked()),this,SLOT(slotSetPos()));
    connect(m_ui->bn2sat,SIGNAL(clicked()),this,SLOT(slotSetPos()));
    connect(m_ui->bnVip,SIGNAL(clicked()),this,SLOT(slotSetPos()));

    connect(m_ui->bnAparat,SIGNAL(clicked()),this,SLOT(slotShowAparat()));
    connect(m_ui->bnCentrif,SIGNAL(clicked()),this,SLOT(slotShowCentrif()));
    connect(m_ui->bnGaspich,SIGNAL(clicked()),this,SLOT(slotShowGaspich()));

    connect(m_ui->bnReport,SIGNAL(clicked()),this,SLOT(slotShowCfReport()));
    connect(m_ui->bnReportFab,SIGNAL(clicked()),this,SLOT(slotShowFabReport()));
    connect(m_ui->bnRou,SIGNAL(clicked()),this,SLOT(slotShowRou()));


    X=new xUdpClient(this);
    mBpm=new BigPumpsMon(X->loadList("192.168.15.91",":/text/list.txt"),this);

    connect(m_ui->bnBigPumps,SIGNAL(clicked()),mBpm,SLOT(exec()));
    connect(X,SIGNAL(updateData(quint16)),mBpm,SLOT(slotUpdate(quint16)));



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

    int nHeight=4000;

//    qDebug() << sender()->objectName();
    UHistorySelect *trd = new UHistorySelect(src,&tp,this);

    if(trd->exec()==QDialog::Accepted)
    {
        qDebug() << tp.host  << tp.db << tp.table;
	pTrw = new TrendWindow(NULL,&tp,nHeight);
        //pTrw->setAttribute( Qt::WA_DeleteOnClose);

        connect(pTrw,SIGNAL(finished()),this,SLOT(showMe()));

	((QStackedWidget*)parentWidget())->addWidget(pTrw);       // показати вікно графіків
        ((QStackedWidget*)parentWidget())->setCurrentWidget(pTrw);
    }
    delete trd;
    //qDebug() << "End Init";
}

void mMainForm::showMe()
{
    ((QStackedWidget*)parentWidget())->setCurrentWidget(this) ;
    ((QStackedWidget*)parentWidget())->removeWidget(pTrw);
    delete pTrw;
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

    m_ui->stackedWidget->setCurrentIndex(0);
    if(hPos.contains(sender()))
    {
        m_ui->scrollArea->horizontalScrollBar()->setValue(hPos[sender()]);
    }

    // вигрузити мнемосхему одиночки, якщо завантажена
    if(oM1)
    {
        m_ui->stackedWidget->removeWidget(oM1);
        delete oM1;
        oM1=NULL;
    }

    if(mRep)
    {
        m_ui->stackedWidget->removeWidget(mRep);
        delete mRep;
        mRep=NULL;
    }

    if(mFRep)
    {
        m_ui->stackedWidget->removeWidget(mFRep);
        delete mFRep;
        mFRep=NULL;

    }

}


void mMainForm::slotShowAparat()
{
    m_ui->stackedWidget->setCurrentIndex(1);

    // вигрузити мнемосхему одиночки, якщо завантажена
    if(oM1)
    {
        m_ui->stackedWidget->removeWidget(oM1);
        delete oM1;
        oM1=NULL;
    }

    if(mRep)
    {
        m_ui->stackedWidget->removeWidget(mRep);
        delete mRep;
        mRep=NULL;
    }

    if(mFRep)
    {
        m_ui->stackedWidget->removeWidget(mFRep);
        delete mFRep;
        mFRep=NULL;

    }

}

void mMainForm::slotShowCentrif()
{
    m_ui->stackedWidget->setCurrentIndex(2);

    // вигрузити мнемосхему одиночки, якщо завантажена
    if(oM1)
    {
        m_ui->stackedWidget->removeWidget(oM1);
        delete oM1;
        oM1=NULL;
    }

    if(mRep)
    {
        m_ui->stackedWidget->removeWidget(mRep);
        delete mRep;
        mRep=NULL;
    }

    if(mFRep)
    {
        m_ui->stackedWidget->removeWidget(mFRep);
        delete mFRep;
        mFRep=NULL;

    }

}



void mMainForm::showOneMnemo1() // ця функція слугє для виводу мнемосхеми одиночної центрифуги
{
    qDebug() << sender()->objectName();
    int i=sender()->objectName().split("_").at(2).toInt()-3; // !!!!!!!!!!!!!!!!!!!!!!!!!! корекція на 4 5 центрифуги

    if(i<6) {
    oM1 =new OneMnemo1(CENTRIF,cFm->getTrChart(),i,this);
    m_ui->stackedWidget->addWidget(oM1);
    m_ui->stackedWidget->setCurrentWidget(oM1);
    }
}


void mMainForm::slotShowCfReport()
{

     if(m_ui->stackedWidget->currentIndex()==3)
     {
         QCoreApplication::setApplicationName("gaspich");
         if(mFRep)
         {
             m_ui->stackedWidget->removeWidget(mFRep);
             delete mFRep;
             mFRep=NULL;

         }

         if(mRep==NULL)
        {
            mRep = new GpReport(this);
            m_ui->stackedWidget->addWidget(mRep);
            m_ui->stackedWidget->setCurrentWidget(mRep);
        }
     }

     else
     {
         QCoreApplication::setApplicationName("centrif");
         if(mFRep)
         {
             m_ui->stackedWidget->removeWidget(mFRep);
             delete mFRep;
             mFRep=NULL;

         }

         if(mRep==NULL)
        {
            mRep = new Report(this);
            m_ui->stackedWidget->addWidget(mRep);
            m_ui->stackedWidget->setCurrentWidget(mRep);
        }
    }
}


void mMainForm::slotShowFabReport()
{
     QCoreApplication::setApplicationName("centrif");

     if(mRep)
     {
         m_ui->stackedWidget->removeWidget(mRep);
         delete mRep;
         mRep=NULL;
     }


     if(mFRep==NULL)
    {
        mFRep = new FabReport(src,this);
        m_ui->stackedWidget->addWidget(mFRep);
        m_ui->stackedWidget->setCurrentWidget(mFRep);
    }
}

void mMainForm::slotShowGaspich()
{
    m_ui->stackedWidget->setCurrentIndex(3);

    if(mRep)
    {
        m_ui->stackedWidget->removeWidget(mRep);
        delete mRep;
        mRep=NULL;
    }


}

void mMainForm::slotShowRou()
{
    m_ui->stackedWidget->setCurrentIndex(4);

    // вигрузити мнемосхему одиночки, якщо завантажена
    if(oM1)
    {
        m_ui->stackedWidget->removeWidget(oM1);
        delete oM1;
        oM1=NULL;
    }

    if(mRep)
    {
        m_ui->stackedWidget->removeWidget(mRep);
        delete mRep;
        mRep=NULL;
    }

    if(mFRep)
    {
        m_ui->stackedWidget->removeWidget(mFRep);
        delete mFRep;
        mFRep=NULL;

    }

}
