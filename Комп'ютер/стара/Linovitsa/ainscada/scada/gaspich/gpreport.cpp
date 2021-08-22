#include "gpreport.h"
#include "ui_gpreport.h"
#include <QVBoxLayout>

#include "datetimedialog.h"

#include <QSqlQueryModel>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>

#include <QSqlRecord>


GpReport::GpReport(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::GpReport),
    repFld("cft,Nc,Mode, Tall,FORMAT(Gcl,2) ,tmGcl,FORMAT(Gst,2),tmGst,FORMAT(Gst-Gcl,2),FORMAT(Gcl*100.0/prevGst,2)"),
    agrFld("FORMAT(avg(Tall),2),\"\",FORMAT(sum(Gcl),2),FORMAT(avg(tmGcl),2),FORMAT(sum(Gst),2),FORMAT(avg(tmGst),2),FORMAT(sum(prevGst),2),FORMAT((sum(Gcl)/sum(prevGst)*100.0),2)")

{
    m_ui->setupUi(this);

    connect(m_ui->bnStart,SIGNAL(clicked()),this,SLOT(slotCallCalendar()));
    connect(m_ui->bnStop,SIGNAL(clicked()),this,SLOT(slotCallCalendar()));

//    connect(m_ui->cbSelZm,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeZm(int)));
    connect(m_ui->cbSelZm,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeZm(int)));


    QDateTime cdt=QDateTime::currentDateTime(); // визначити поточний час
    if(cdt.time().hour()>=8 && cdt.time().hour()<20) // якщо зараз денна зміна
    {
        m_ui->sbStart->setDate(cdt.date());
        m_ui->sbStart->setTime(QTime(8,0,0,0));
    }
    else
    {
        m_ui->sbStart->setDate(QDate(cdt.date().year(),cdt.date().month(),cdt.date().day()).addDays(cdt.time().hour()>19?0:-1));
        m_ui->sbStart->setTime(QTime(20,0,0,0));
    }
    m_ui->sbStop->setDateTime(m_ui->sbStart->dateTime().addSecs(12*3600));

    QSqlDatabase dbs=QSqlDatabase::addDatabase("QMYSQL","GpReport");
    // заточка під drizzle, який створює сокети в /tmp
    if(QFile::exists("/tmp/mysql.socket")) // якщо такий файл існує
    {
        dbs.setConnectOptions("UNIX_SOCKET=/tmp/mysql.socket");
    }

    QSettings set;

    qDebug() << "/db/host" << set.value("/db/hostname","localhost").toString();

    dbs.setHostName(set.value("/db/hostname","localhost").toString());
    dbs.setDatabaseName(set.value("/db/dbname","centrif").toString());
    dbs.setUserName(set.value("/db/username","scada").toString());
    dbs.setPassword(set.value("/db/passwd","").toString());
    if(!dbs.open()) // спробувати відкрити
    {
        qDebug() << dbs.lastError().databaseText();
    }


    mRepModel = new QSqlQueryModel(this);
    mRepAgrModel = new QSqlQueryModel(this);

    mRepModel->setQuery(QString("SELECT %0 FROM report WHERE cft BETWEEN \'%1\' AND \'%2\' AND prevGst>0").arg(repFld)
                        .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                        .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);

    mRepAgrModel->setQuery(QString("SELECT \"%0\",count(*),%1 FROM report WHERE cft BETWEEN \'%2\' AND \'%3\'")
                           .arg(tr("-"))
                           .arg(agrFld)
                        .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                        .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);


    qDebug() << mRepModel->query().lastQuery();

    if(mRepAgrModel->lastError().isValid())
    {
        qDebug() << mRepAgrModel->query().lastQuery();
        qDebug() << mRepAgrModel->lastError();
    }


    if(mRepModel->lastError().isValid())
    {
        qDebug() << mRepModel->lastError();
    }

    mRepModel->setHeaderData(0,Qt::Horizontal,tr("Час"));
    mRepModel->setHeaderData(1,Qt::Horizontal,tr("№ завант."));
    mRepModel->setHeaderData(2,Qt::Horizontal,tr("Режим"));
    mRepModel->setHeaderData(3,Qt::Horizontal,tr("Час завант."));
    mRepModel->setHeaderData(4,Qt::Horizontal,tr("Вага вугілля"));
    mRepModel->setHeaderData(5,Qt::Horizontal,tr("Час вугілля"));
    mRepModel->setHeaderData(6,Qt::Horizontal,tr("Вага шихти"));
    mRepModel->setHeaderData(7,Qt::Horizontal,tr("Час шихти"));
    mRepModel->setHeaderData(8,Qt::Horizontal,tr("Вага вап.кам."));
    mRepModel->setHeaderData(9,Qt::Horizontal,tr("Співвідн."));





    m_ui->twReport->setModel(mRepModel);

    m_ui->twReport->setColumnWidth(0, 180 );
    m_ui->twReport->setColumnWidth(1 , 100 );
    m_ui->twReport->setColumnWidth(2 , 100 );
    m_ui->twReport->setColumnWidth(3 , 100 );
    m_ui->twReport->setColumnWidth(4 , 100 );
    m_ui->twReport->setColumnWidth(5 , 100 );
    m_ui->twReport->setColumnWidth(6 , 100 );
    m_ui->twReport->setColumnWidth(7 , 100 );
    m_ui->twReport->setColumnWidth(8 , 100 );
    m_ui->twReport->setColumnWidth(9 , 100 );

    m_ui->twReportAgr->setModel(mRepAgrModel);


    m_ui->twReportAgr->setColumnWidth(0, 190 );
    m_ui->twReportAgr->setColumnWidth(1 , 100 );
    m_ui->twReportAgr->setColumnWidth(2 , 100 );
    m_ui->twReportAgr->setColumnWidth(3 , 100 );
    m_ui->twReportAgr->setColumnWidth(4 , 100 );
    m_ui->twReportAgr->setColumnWidth(5 , 100 );
    m_ui->twReportAgr->setColumnWidth(6 , 100 );
    m_ui->twReportAgr->setColumnWidth(7 , 100 );
    m_ui->twReportAgr->setColumnWidth(8 , 100 );
    m_ui->twReportAgr->setColumnWidth(9 , 100 );
}

GpReport::~GpReport()
{
    {
        QSqlDatabase dbs=QSqlDatabase::database("GpReport");
        dbs.close();
    }
    QSqlDatabase::removeDatabase("GpReport");
    delete m_ui;
}

void GpReport::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void GpReport::slotCallCalendar()
{
    QDateTime v= (sender()->objectName()=="bnStart"? m_ui->sbStart: m_ui->sbStop)->dateTime();


    myDateTimeDialog dt(this,v);
    if(dt.exec()==QDialog::Accepted)
    {
        v.setDate(dt.date->selectedDate());
        v.setTime(QTime(dt.hour->value(),dt.minute->value(),dt.second->value()));
    }
    (sender()->objectName()=="bnStart"? m_ui->sbStart: m_ui->sbStop)->setDateTime(v);


    m_ui->cbSelZm->blockSignals(true);
    m_ui->cbSelZm->setCurrentIndex(3);
    m_ui->cbSelZm->blockSignals(false);

    QSqlDatabase dbs=QSqlDatabase::database("GpReport",true); //знайти з’єдання logging
    if(dbs.isOpen())
    {
        mRepModel->setQuery(QString("SELECT %0 FROM report WHERE cft BETWEEN \'%1\' AND \'%2\'  AND prevGst>0").arg(repFld)
                            .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                            .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);


        mRepAgrModel->setQuery(QString("SELECT \"%0\",count(*),%1 FROM report WHERE cft BETWEEN \'%2\' AND \'%3\'")
                               .arg(tr("-"))
                               .arg(agrFld)
                            .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                            .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);

    }

}

void GpReport::slotChangeZm(int v)
{

    // визначили поточну зміну

    qDebug() << "------------------------------------------------------------------v" <<v ;

    QDateTime cdt=QDateTime::currentDateTime(); // визначити поточний час
    if(cdt.time().hour()>=8 && cdt.time().hour()<20) // якщо зараз денна зміна
    {
        m_ui->sbStart->setDate(cdt.date());
        m_ui->sbStart->setTime(QTime(8,0,0,0));
    }
    else
    {
        m_ui->sbStart->setDate(QDate(cdt.date().year(),cdt.date().month(),cdt.date().day()).addDays(cdt.time().hour()>19?0:-1));
        m_ui->sbStart->setTime(QTime(20,0,0,0));
    }
    m_ui->sbStop->setDateTime(m_ui->sbStart->dateTime().addSecs(12*3600));

    QSqlDatabase dbs=QSqlDatabase::database("GpReport",true); //знайти з’єдання logging

    int first=0,count=0,maxId=0;

    switch(v)
    {
        default:
        case 0:
            break;
        case 1:
            m_ui->sbStart->setDateTime(m_ui->sbStart->dateTime().addSecs(-12*3600));
            m_ui->sbStop->setDateTime(m_ui->sbStop->dateTime().addSecs(-12*3600));
            break;
        case 2:
            m_ui->sbStart->setDateTime(m_ui->sbStart->dateTime().addSecs(-12*3600));
            break;
        case 4:
                QSqlQuery qry("SELECT count(*),max(id) from Report",dbs);
                if(qry.exec())
                {
                    qry.next();
                    QSqlRecord rec=qry.record();

                     count = rec.value(0).toInt();
                     maxId = rec.value(1).toInt();

                     if(count>10) first=maxId-9;


                }
                else
                    qDebug() << qry.lastError().databaseText();


                qDebug() << "count" << count;

            break;
    }

    if(dbs.isOpen())
    {
        if(v<4)
        {
        mRepModel->setQuery(QString("SELECT %0 FROM report WHERE cft BETWEEN \'%1\' AND \'%2\'  AND prevGst>0").arg(repFld)
                            .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                            .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);

        mRepAgrModel->setQuery(QString("SELECT \"%0\",count(*),%1 FROM report WHERE cft BETWEEN \'%2\' AND \'%3\'")
                               .arg(tr("-")).arg(agrFld)
                            .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                            .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);
        }
        else
        {
            mRepModel->setQuery(QString("SELECT %0 FROM report where id BETWEEN %1 and %2 ORDER BY cft").arg(repFld)
                                .arg(first).arg(maxId),dbs);

            mRepAgrModel->setQuery(QString("SELECT \"%0\",count(*),%1 FROM report where id BETWEEN  %2 and %3")
                                   .arg(tr("-"))
                                   .arg(agrFld)
                                   .arg(first)
                                .arg(maxId),dbs);

            qDebug() << mRepModel->query().lastQuery();

        }

    }
    else
    {
        qDebug()<< dbs.lastError().text();

    }

}
