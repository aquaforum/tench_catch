#include "gpreport.h"
#include "ui_gpreport.h"
#include <QVBoxLayout>

#include <aulib.h>
#include <AuWidget/datetimedialog.h>

#include <QDebug>
#include <QTimer>

#define repFld QStringLiteral("Dt,Nc,Mode, Tall,Gcl ,tmGcl,Gst,tmGst,Gst-Gcl,Gcl*100.0/prevGst")
#define agrFld QStringLiteral("avg(Tall),\"\",sum(Gcl),avg(tmGcl),sum(Gst),avg(tmGst),sum(prevGst),(sum(Gcl)/sum(prevGst)*100.0)")

#define REPTABLE QStringLiteral("gpreport")

GpReport::GpReport(AuBase &source, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::GpReport),
    src(source),
    state(0)
{
    m_ui->setupUi(this);

    connect(m_ui->bnStart,&QPushButton::clicked,this,&GpReport::slotCallCalendar);
    connect(m_ui->bnStop, &QPushButton::clicked,this,&GpReport::slotCallCalendar);
    connect(m_ui->cbSelZm,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&GpReport::slotChangeZm);

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

    m_ui->twReport->setColumnCount(10);

    m_ui->twReport->setHorizontalHeaderLabels(
    QStringList()
    <<tr("Час")
    <<tr("№ завант.")
    <<tr("Режим")
    <<tr("Час\nзавант.")
    <<tr("Вага\nвугілля")
    <<tr("Час\nвугілля")
    <<tr("Вага\nшихти")
    <<tr("Час\nшихти")
    <<tr("Вага\nвап.кам.")
    <<tr("Співвідн.") );

    m_ui->twReportAgr->setColumnCount(10);


    m_ui->twReport->setColumnWidth(0, 200 );
    m_ui->twReport->setColumnWidth(1 , 100 );
    m_ui->twReport->setColumnWidth(2 , 100 );
    m_ui->twReport->setColumnWidth(3 , 100 );
    m_ui->twReport->setColumnWidth(4 , 100 );
    m_ui->twReport->setColumnWidth(5 , 100 );
    m_ui->twReport->setColumnWidth(6 , 100 );
    m_ui->twReport->setColumnWidth(7 , 100 );
    m_ui->twReport->setColumnWidth(8 , 100 );
    m_ui->twReport->setColumnWidth(9 , 100 );

    // qDebug() << m_ui->twReport->verticalHeader()->geometry().width();

    m_ui->twReportAgr->horizontalHeader()->hide();
    m_ui->twReportAgr->setColumnWidth(0, 220 );
    m_ui->twReportAgr->setColumnWidth(1 , 100 );
    m_ui->twReportAgr->setColumnWidth(2 , 100 );
    m_ui->twReportAgr->setColumnWidth(3 , 100 );
    m_ui->twReportAgr->setColumnWidth(4 , 100 );
    m_ui->twReportAgr->setColumnWidth(5 , 100 );
    m_ui->twReportAgr->setColumnWidth(6 , 100 );
    m_ui->twReportAgr->setColumnWidth(7 , 100 );
    m_ui->twReportAgr->setColumnWidth(8 , 100 );
    m_ui->twReportAgr->setColumnWidth(9 , 100 );

    connect(&src,&AuBase::sqlData,this,&GpReport::slotSqlRes);
    connect(&src,&AuBase::sqlError,this,&GpReport::slotSqlError);

    QTimer::singleShot(10,this,&GpReport::slotSendQuery);

}


GpReport::~GpReport()
{
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

    m_ui->twReport->clearContents();
    m_ui->twReportAgr->clearContents();

    QTimer::singleShot(10,this,&GpReport::slotSendQuery);

}

void GpReport::slotChangeZm(int v)
{

    // визначили поточну зміну
    m_ui->twReport->clearContents();
    m_ui->twReportAgr->clearContents();

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
            state=2;
            break;
    }

    QTimer::singleShot(10,this,&GpReport::slotSendQuery);


}

void GpReport::slotSqlRes(qint32 ,const QList<QStringList>& res)
{
    // qDebug() << state << res;
    int i=0;
    QTableWidgetItem *item;
    switch (state) {
    case 0:
    case 3:
    {
        m_ui->twReport->clearContents();
        m_ui->twReport->setRowCount(res.size());
        foreach (auto row, res) {

            // cft
            QDateTime cDt=QDateTime::fromTime_t(row[0].toUInt());
            item=new QTableWidgetItem;
            item->setText(cDt.toString("yyyy/MM/dd hh:mm:ss"));
            m_ui->twReport->setItem(i,0,item);
            // Nc,
            item=new QTableWidgetItem;
            item->setText(row[1]);
            m_ui->twReport->setItem(i,1,item);
            // Mode,
            item=new QTableWidgetItem;
            item->setText(row[2]);
            m_ui->twReport->setItem(i,2,item);
            // Tall,
            item=new QTableWidgetItem;
            item->setText(row[3]);
            m_ui->twReport->setItem(i,3,item);
            // Gcl ,
            item=new QTableWidgetItem;
            item->setText(row[4]);
            m_ui->twReport->setItem(i,4,item);
            // tmGcl,
            item=new QTableWidgetItem;
            item->setText(row[5]);
            m_ui->twReport->setItem(i,5,item);
            // Gst,
            item=new QTableWidgetItem;
            item->setText(row[6]);
            m_ui->twReport->setItem(i,6,item);
            // tmGst,
            item=new QTableWidgetItem;
            item->setText(row[7]);
            m_ui->twReport->setItem(i,7,item);
            // Gst-Gcl,
            item=new QTableWidgetItem;
            item->setText(row[8]);
            m_ui->twReport->setItem(i,8,item);
            // Gcl*100.0/prevGst
            item=new QTableWidgetItem;
            item->setText(QString::number(row[9].toDouble(),'f',2));
            m_ui->twReport->setItem(i,9,item);
            ++i;
        }
    }
        state = state==0?1:4;
        QTimer::singleShot(10,this,&GpReport::slotSendQuery);

        break;
    case 1:
    case 4:
    {
        //qDebug() << "proc result " << state;
        QStringList row=res[0];
        m_ui->twReportAgr->setRowCount(1);
        //qDebug() << row;

        //
        item=new QTableWidgetItem;
        item->setText(row[0]);
        m_ui->twReportAgr->setItem(i,0,item);
        //
        item=new QTableWidgetItem;
        item->setText(row[1]);
        m_ui->twReportAgr->setItem(i,1,item);
        //avg(Tall),
        item=new QTableWidgetItem;
        item->setText(row[2]);
        m_ui->twReportAgr->setItem(i,3,item);
        // \"\",
        item=new QTableWidgetItem;
        item->setText(row[3]);
        m_ui->twReportAgr->setItem(i,2,item);
        //sum(Gcl),
        item=new QTableWidgetItem;
        item->setText(QString::number(row[4].toDouble(),'f',2));
        m_ui->twReportAgr->setItem(i,4,item);
        //avg(tmGcl),
        item=new QTableWidgetItem;
        item->setText(row[5]);
        m_ui->twReportAgr->setItem(i,5,item);
        //sum(Gst),
        item=new QTableWidgetItem;
        item->setText(QString::number(row[6].toDouble(),'f',2));
        m_ui->twReportAgr->setItem(i,6,item);
        //avg(tmGst),
        item=new QTableWidgetItem;
        item->setText(row[7]);
        m_ui->twReportAgr->setItem(i,7,item);
        //sum(prevGst),
        item=new QTableWidgetItem;
        item->setText(QString::number(row[8].toDouble(),'f',2));
        m_ui->twReportAgr->setItem(i,8,item);
        //(sum(Gcl)/sum(prevGst)*100.0
        item=new QTableWidgetItem;
        item->setText(QString::number(row[9].toDouble(),'f',2));
        m_ui->twReportAgr->setItem(i,9,item);

    }

        m_ui->twReportAgr->setColumnWidth(0, 185+m_ui->twReport->verticalHeader()->geometry().width() ); // змінити ширину
        // qDebug() <<m_ui->twReport->columnWidth(0) ;

        state=0;
        break;
    case 2: // обробка запиту на кількість рядків і максимальний індекс
        count = res[0][0].toInt();
        maxId = res[0][1].toInt();

        if(count>10) first=maxId-9;
        else first=0;
        state=3;
        QTimer::singleShot(10,this,&GpReport::slotSendQuery);

        break;
    default:
        break;


    }
}

void GpReport::slotSqlError(QString msg)
{
    qDebug() << msg;
}

void GpReport::slotSendQuery()
{
    QStringList qry;
    qry << QStringLiteral("SELECT %0 FROM gpreport WHERE Dt BETWEEN \'%1\' AND \'%2\' AND prevGst>0  ORDER BY Dt").arg(repFld)
            .arg(m_ui->sbStart->dateTime().toTime_t())
            .arg(m_ui->sbStop->dateTime().toTime_t())

    <<QStringLiteral("SELECT \"%0\",count(*),%1 FROM gpreport WHERE Dt BETWEEN \'%2\' AND \'%3\'")
                           .arg(tr("-"))
                           .arg(agrFld)
                        .arg(m_ui->sbStart->dateTime().toTime_t())
                        .arg(m_ui->sbStop->dateTime().toTime_t())
    << QStringLiteral("SELECT count(*),max(rowid) from gpreport")
    << QStringLiteral("SELECT %0 FROM gpreport where rowid BETWEEN %1 and %2 ORDER BY Dt").arg(repFld)
                                       .arg(first).arg(maxId)
    << QStringLiteral("SELECT \"%0\",count(*),%1 FROM gpreport where rowid BETWEEN  %2 and %3")
                                          .arg(tr("-"))
                                          .arg(agrFld)
                                          .arg(first)
                                       .arg(maxId)
       ;

    // qDebug() << "sql:" << qry[state];
    src.querySql(REPTABLE,qry[state]);

}


