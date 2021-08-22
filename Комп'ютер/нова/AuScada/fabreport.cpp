#include "fabreport.h"
#include "ui_fabreport.h"
#include <QVBoxLayout>

#include <AuWidget/datetimedialog.h>

#include <QSqlQueryModel>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>

#include <QSqlRecord>

#include <QPrintDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTableWidgetItem>
#include <QDateTime>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <aulib.h>

#include <QPdfWriter>
#include <QPainter>


// #include "fabreportwriter.h"
// #include "formreporttest.h"

#define DIFUZ   QStringLiteral("plcdifuz")
#define SATUR   QStringLiteral("plcsatur")
#define VIPARKA QStringLiteral("plcvipar")
#define ROU     QStringLiteral("plcrou")
#define GPI4    QStringLiteral("plcgpi4")

#define mRepModel m_ui->twReport
#define COLWIDTH 92

FabReport::FabReport(AuBase &source, QWidget *parent):  QWidget(parent),
    src(source),
    m_ui(new Ui::FabReport)
{
    m_ui->setupUi(this);


    m_ui->twReport->setColumnCount(18);
    m_ui->twReport->setHorizontalHeaderLabels(
    QStringList() <<tr("Початок")
    <<tr("Закінчення")
    <<tr("Буряк\nстр. ваги")
    <<tr("Вода\nна диф.")
    <<tr("Дифуз.\n сік")
    <<tr("Вапн.\nмол.")
    <<tr("Сік\nна В.С.")
    <<tr("Сироп\nз В.С.")
    <<tr("Пара\nРОУ")
    <<tr("Пара\nТ1")
    <<tr("Пара\nТ2")
    <<tr("Вода\nна Д.Ф.")
    <<tr("Вода\nна В.С.")
    <<tr("Вода\nна В-А")
    <<tr("Вугілля")
    <<tr("Вапняк")
    <<tr("Цикли\nКА-1000")
    <<tr("-")
            );

    m_ui->twReport->setColumnWidth(0, 200 );
    m_ui->twReport->setColumnWidth(1 ,200 );
    m_ui->twReport->setColumnWidth(2 , COLWIDTH );
    m_ui->twReport->setColumnWidth(3 , COLWIDTH );
    m_ui->twReport->setColumnWidth(4 , COLWIDTH );
    m_ui->twReport->setColumnWidth(5 , COLWIDTH );
    m_ui->twReport->setColumnWidth(6 , COLWIDTH );
    m_ui->twReport->setColumnWidth(7 , COLWIDTH );
    m_ui->twReport->setColumnWidth(8 , COLWIDTH );
    m_ui->twReport->setColumnWidth(9 , COLWIDTH );
    m_ui->twReport->setColumnWidth(10 , COLWIDTH  );
    m_ui->twReport->setColumnWidth(11 , COLWIDTH );
    m_ui->twReport->setColumnWidth(12 , COLWIDTH );
    m_ui->twReport->setColumnWidth(13 , COLWIDTH );
    m_ui->twReport->setColumnWidth(14 , COLWIDTH );
    m_ui->twReport->setColumnWidth(15 , COLWIDTH );
    m_ui->twReport->setColumnWidth(16 , COLWIDTH );
    m_ui->twReport->setColumnWidth(17 , COLWIDTH );

    m_ui->twMassReport->setColumnCount(15);
    m_ui->twMassReport->setHorizontalHeaderLabels(
    QStringList() <<tr("Зміна")
    <<tr("")
                <<tr("12")
                <<tr("1")
                <<tr("2")
                <<tr("3")
                <<tr("4")
                <<tr("5")
                <<tr("6")
                <<tr("7")
                <<tr("8")
                <<tr("9")
                <<tr("10")
                <<tr("11")
                <<tr("Cума"));
    m_ui->twMassReport->setColumnWidth(0, 200 );
    m_ui->twMassReport->setColumnWidth(1 ,200 );
    m_ui->twMassReport->setColumnWidth(2 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(3 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(4 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(5 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(6 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(7 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(8 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(9 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(10 , COLWIDTH  );
    m_ui->twMassReport->setColumnWidth(11 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(12 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(13 , COLWIDTH );
    m_ui->twMassReport->setColumnWidth(15 , COLWIDTH );


    connect(m_ui->bnPrint,SIGNAL(clicked()),this,SLOT(printReport()));
    connect(m_ui->bnSave,SIGNAL(clicked()),this,SLOT(saveReport()));
    connect(&src,&AuBase::sqlData,this,&FabReport::slotSqlRes);




    qIx=src.querySql(QStringLiteral("fabreport"),QStringLiteral("SELECT %1 FROM fabreport")
                     .arg("tStart,tStop,"
                          "SGstr, "
                          "SVvoda, "
                          "SVds, "
                          "SVvm, "
                          "SVsvp, "
                          "SVsfv, "
                          "GRou, "
                          "GT1, "
                          "GT2, "
                          "SVvfd, "
                          "SVvtv, "
                          "SVvva, "
                          "SGcl, "
                          "SGst, "
                          "KFn, "
                          "S7Vds, "
                          "H11, H00, H01, H02, H03, H04, H05, H06, H07, H08, H09, H10, "
                          "W11, W00, W01, W02, W03, W04, W05, W06, W07, W08, W09, W10"));

    // це все тимчасовий код, який після тестування треба буде видалити
    //connect(m_ui->bnRepGen,&QPushButton::clicked,this,&FabReport::slotRunReport);
    //connect(m_ui->bnEmit,&QPushButton::clicked,this,&FabReport::slotRunEmit);
}

/*
void FabReport::slotRunEmit()
{
    FormReportTest dlg(src,this);
    dlg.exec();

}

void FabReport::slotRunReport()
{
    // new FabReportWriter(src,this);  // далі він живе своїм життям
}
*/
// кінець тимчасового коду

FabReport::~FabReport()
{
    delete m_ui;
}


void FabReport::changeEvent(QEvent *e)
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

#define REP m_ui->twReport

void FabReport::slotSqlRes(qint32 id, const QList<QStringList> &res)
{
    // qDebug() << "FabReport slotSqlRes start";

    if(id==qIx)
    {
        m_ui->twReport->clearContents();
        m_ui->twReport->setRowCount(res.size());

        m_ui->twMassReport->clearContents();
        m_ui->twMassReport->setRowCount(res.size()*2);

        int i=0;
        foreach(QStringList row,res)
        {
            //qDebug() << res;
            QTableWidgetItem *item;
            item=new QTableWidgetItem(QDateTime::fromTime_t(row[0].toUInt()).toString(QStringLiteral("yyyy/MM/dd hh:mm:ss")));
            m_ui->twReport->setItem(i,0,item);

            item=new QTableWidgetItem(QDateTime::fromTime_t(row[0].toUInt()).toString(QStringLiteral("yyyy/MM/dd hh:mm:ss")));
            m_ui->twMassReport->setItem(i*2,0,item);

            item=new QTableWidgetItem(QDateTime::fromTime_t(row[1].toUInt()).toString(QStringLiteral("yyyy/MM/dd hh:mm:ss")));
            m_ui->twReport->setItem(i,1,item);

            item=new QTableWidgetItem(QDateTime::fromTime_t(row[1].toUInt()).toString(QStringLiteral("yyyy/MM/dd hh:mm:ss")));
            m_ui->twMassReport->setItem(i*2+1,0,item);

            item=new QTableWidgetItem(QStringLiteral("Газ"));
            m_ui->twMassReport->setItem(i*2,1,item);
            item=new QTableWidgetItem(QStringLiteral("Буряк"));
            m_ui->twMassReport->setItem(i*2+1,1,item);

            for(int j=2;j<18;++j)
            {
                item=new QTableWidgetItem(row[j]);
                m_ui->twReport->setItem(i,j,item);
            }

            double gas=0,mass=0;

            for(int j=0;j<12;++j)
            {
                item=new QTableWidgetItem(row[18+j].toDouble()>0.0?row[18+j]:QStringLiteral("-")); // - якщо немає даних по газу
                m_ui->twMassReport->setItem(i*2,j+2,item);
                gas+=row[18+j].toDouble();

                item=new QTableWidgetItem(row[18+12+j]);
                m_ui->twMassReport->setItem(i*2+1,j+2,item);
                mass+=row[18+12+j].toDouble();
            }
            item=new QTableWidgetItem(gas>0?QString::number(gas,'f',0):QStringLiteral("-"));
            m_ui->twMassReport->setItem(i*2,14,item);

            item=new QTableWidgetItem(QString::number(mass,'f',0));
            m_ui->twMassReport->setItem(i*2+1,14,item);

            ++i;
        }
    }
}

// ** звіт з KF-10000 додано в 2019р.
/*
    int kfN=-1;
    {
        QSqlDatabase kfDbs=QSqlDatabase::addDatabase("QMYSQL","kf1000");

        kfDbs.setHostName("kffilter");
        kfDbs.setDatabaseName("kf1000");
        kfDbs.setUserName("scada");
        kfDbs.setPassword("");
        if(!kfDbs.open()) // спробувати відкрити
        {
            qDebug() << kfDbs.lastError().databaseText();
        }
        QSqlQuery qry(kfDbs);

        if(qry.exec(QString("SELECT count(*) FROM report where Dt between '%1' AND '%2';")
                    .arg(m_ui->twReport->item(nLine,0)->text())
                .arg(m_ui->twReport->item(nLine,1)->text())))
        {
            qry.next();
            QSqlRecord rec=qry.record();
            kfN=rec.value(0).toInt();
        }
        //qDebug() << qry.executedQuery();
        //qDebug() << "kfN" << kfN;

        qry.finish();
        kfDbs.close();

    }
    QSqlDatabase::removeDatabase("kf1000");
*/

#define FC 0.351
void FabReport::printReport()
{

    // друкуємо звіт
    QPrinter printer(QPrinter::PrinterResolution);
    QPrintDialog pDialg(&printer,this);

    if(pDialg.exec()==QDialog::Accepted)
    {
        int nLine=0;

        nLine=m_ui->twReport->currentRow();
        if(nLine<0)// не вибрано жодного рядка
        {
            nLine=m_ui->twReport->rowCount()-1;
        }

        QStringList fieldNames;
        fieldNames
                << tr("Стрічкові ваги:")                    //0
                << tr("Витрачено води на дифузію:")         //1
                << tr("Отримано дифузійного  соку:")        //2
                << tr( "Витрачено вапнякового молока:")     // 3
                << tr("Подано соку на випарку:")            // 4
                << tr("Отримано сиропу:")                   // 5
                << tr("Пара (РОУ+T1+T2):")                  // 6 7 8
                << tr("Вода на дискові фільтри:") // 9
                << tr("Вода на В.С.:")            // 10
                << tr("Вода на вакуум-апарати")             // 11
        << tr("Спожито вугілля:")                           // 12
        << tr("Спожито вапняку:")                           // 13
        << tr("KF-1000 циклів:");                           // 14

        QStringList unit;
        unit << tr("тон")
             << tr("м³")
             << tr("м³")
             << tr("м³")
             << tr("м³")
             << tr("м³")
             << tr("тон")
             << tr("м³")
             << tr("м³")
             << tr("м³")
             << tr("кг")
             << tr("кг")
             << QStringLiteral("");


    // генерація звіту в PDF форматі.
    /*------------------------------------------------------------------------------------------------------------------------------------------------*/
        printer.setPageSize(QPagedPaintDevice::A4);
        printer.setPageMargins(QMargins(15, 15, 15, 15));

        QPainter p;
        p.begin(&printer);

        // задати товщину лінії для рамок таблиці
        QPen pen;
        pen.setWidth(1);
        p.setPen(pen);

        QRect r = p.viewport(); // отримати розмір аркуша
        // розрахувати коефіцієнти перерахунку в міліметри
        double kx,ky;
        kx=r.width()/(210.0-30.0); // розмір сторінки мінус поля по 20 мм див. вище
        ky=r.height()/(297.0-30.0);

        int yPos=0; // це позиція по осі Y а "пікселях"
        double fontPx=32; // ромір шрифту, для початку для заголовку, потім по ходу вінміняється і приймає участь в розрахунку позиції по осі Y

        // звіт за зміну
        p.setFont(QFont("DejaVu Sans",fontPx));
        p.drawText(QRect(0,yPos,r.width(),fontPx*FC*ky),Qt::AlignCenter| Qt::AlignVCenter,QStringLiteral("Звіт за зміну"));
        yPos+=(fontPx*FC+1.0)*ky;

        // наступний після заголвка рядок
        // від - до -
        fontPx=18;
        p.setFont(QFont("DejaVu Sans",fontPx,QFont::Bold));
        p.drawText(QRect(0,yPos,r.width(),fontPx*FC*ky),Qt::AlignCenter| Qt::AlignVCenter,
                   QString("від %1 до %2")
                   .arg(m_ui->twReport->item(nLine,0)->text())
                   .arg(m_ui->twReport->item(nLine,1)->text()));

        yPos+=(fontPx*FC+1.0)*ky;

        fontPx=14;
        p.setFont(QFont("DejaVu Sans",fontPx));

        // yPos*=ky; // тут перйшов на пікселі бо інакше таблиця розпадається
        int hPx=(fontPx*FC+1.0)*ky; // розрахувати зміщення по осі Y для решти сторінки,

        int yStart=yPos;
        // заголовок таблиці параметрів
        p.setFont(QFont("DejaVu Sans",fontPx,-1,true));
        p.drawText(QRect(1.0*kx     ,yPos,90*kx,hPx),Qt::AlignLeft | Qt::AlignVCenter ,QStringLiteral("Параметр"));
        p.drawText(QRect(91*kx,yPos,53*kx ,hPx),Qt::AlignLeft | Qt::AlignVCenter ,QStringLiteral("Значення"));
        p.drawText(QRect(146.0*kx     ,yPos,24*kx,hPx),Qt::AlignLeft | Qt::AlignVCenter ,QStringLiteral("Одиниця"));
        yPos+=hPx;
        p.drawLine(0,yPos,180.0*kx,yPos);

        p.setFont(QFont("DejaVu Sans",fontPx));
        int i=0;
        foreach (QString s, fieldNames) {
            p.drawText(QRect(1.0*kx     ,yPos,90*kx,hPx),Qt::AlignLeft | Qt::AlignVCenter ,s);

            if(i<6)
                p.drawText(QRect(91*kx,yPos,53*kx ,hPx),Qt::AlignLeft | Qt::AlignVCenter ,m_ui->twReport->item(nLine,i+2)->text()); // тут треба вставити лямбду
            else if(i==6)
            {
                double v=m_ui->twReport->item(nLine,i+2)->text().toDouble()
                        +m_ui->twReport->item(nLine,i+3)->text().toDouble()
                        +m_ui->twReport->item(nLine,i+4)->text().toDouble();
                p.drawText(QRect(91*kx,yPos,53*kx ,hPx),Qt::AlignLeft | Qt::AlignVCenter ,QString::number(v,'f',1)); // тут треба вставити лямбду

            }
            else
                p.drawText(QRect(91*kx,yPos,53*kx ,hPx),Qt::AlignLeft | Qt::AlignVCenter ,m_ui->twReport->item(nLine,i+4)->text()); // тут треба вставити лямбду


            p.drawText(QRect(146*kx     ,yPos,24*kx,hPx),Qt::AlignLeft | Qt::AlignVCenter ,unit[i]);

            ++i;
            yPos+=hPx;
            p.drawLine(0,yPos,180.0*kx,yPos);
        }

        // домалювати решту таблиці
        p.drawLine(0,yPos,0,yStart);
        p.drawLine(0,yStart,180.0*kx,yStart);
        p.drawLine(180.0*kx,yPos,180.0*kx,yStart);
        p.drawLine(90.0*kx,yPos,90.0*kx,yStart);
        p.drawLine(145.0*kx,yPos,145.0*kx,yStart);

        yPos+=1.5*ky; // відступити
        p.setFont(QFont("DejaVu Sans",fontPx,QFont::Bold));
        p.drawText(QRect(1.0*kx,yPos,r.width()-1*kx,hPx),QStringLiteral("Почасовий звіт"));
        yPos+=hPx;


        // таблиця почасового звіту
        yStart=yPos;
        p.drawLine(0,yPos,180.0*kx,yPos);

        // заголовок таблиці почасового звіту
        p.setFont(QFont("DejaVu Sans",fontPx,-1,true));
        p.drawText(QRect(1.0*kx,yPos,28*kx,hPx),Qt::AlignLeft,QStringLiteral("Година"));
        p.drawText(QRect(31.0*kx,yPos,68*kx,hPx),Qt::AlignLeft,QStringLiteral("Буряк, т"));
        p.drawText(QRect(101.0*kx,yPos,68*kx,hPx),Qt::AlignLeft,QStringLiteral("Газ, м³"));

        yPos+=hPx;
        p.drawLine(0,yPos,180.0*kx,yPos);

        p.setFont(QFont("DejaVu Sans",fontPx));
        // заносимо значення в таблицю
        double gasSum=0.0,massSum=0.0;
        for(int i=0;i<12;++i)
        {
            p.drawText(QRect(1.0*kx,yPos,28*kx,hPx),Qt::AlignLeft,QString::number(i==0?12:i));
            p.drawText(QRect(31.0*kx,yPos,68*kx,hPx),Qt::AlignLeft,m_ui->twMassReport->item(nLine*2+1,i+2)->text());
            p.drawText(QRect(101.0*kx,yPos,68*kx,hPx),Qt::AlignLeft,m_ui->twMassReport->item(nLine*2,i+2)->text());

//#warning тут треба треба робити поправку на відсутні дані по газу
            gasSum+=m_ui->twMassReport->item(nLine*2,i+2)->text()!="-"?m_ui->twMassReport->item(nLine*2,i+2)->text().toDouble():-1; // -1 якщо немає даних по газу
            massSum+=m_ui->twMassReport->item(nLine*2+1,i+2)->text().toDouble();
            //qDebug() << massSum << m_ui->twMassReport->item(nLine*2+1,i+2)->text().toDouble();

            yPos+=hPx;
            p.drawLine(0,yPos,180.0*kx,yPos);
        }
        // рядок СУМА
        p.drawText(QRect(1.0*kx,yPos,28*kx,hPx),Qt::AlignLeft,QStringLiteral("Cума"));
        p.drawText(QRect(31.0*kx,yPos,68*kx,hPx),Qt::AlignLeft,QString::number(massSum,'f',1));
        p.drawText(QRect(101.0*kx,yPos,68*kx,hPx),Qt::AlignLeft,gasSum>0.0?QString::number(gasSum,'f',1):QStringLiteral("-"));

        yPos+=hPx;
        p.drawLine(0,yPos,180.0*kx,yPos);

        p.drawLine(0,yPos,0,yStart);
        p.drawLine(30.0*kx,yPos,30.0*kx,yStart);
        p.drawLine(100.0*kx,yPos,100.0*kx,yStart);
        p.drawLine(180.0*kx,yPos,180.0*kx,yStart);

        yPos+=1.5*ky;

        p.drawText(QRect(1.0*kx,yPos,168*kx,hPx),Qt::AlignLeft,QStringLiteral("Співвідношення газ/буряк: %1 м³/т").arg(massSum>0? gasSum/massSum:0.0,10,'f',1));
        yPos+=hPx;

        p.drawText(QRect(1.0*kx,yPos,168*kx,hPx),Qt::AlignLeft,QStringLiteral("Співвідношення стружка/відкачка: %1 %")
                   .arg(massSum>0? QString::number( m_ui->twReport->item(nLine,17)->text().toDouble()/massSum*100.0,'f',1):QStringLiteral("-")));
        // qDebug() << m_ui->twReport->item(nLine,17)->text().toDouble();

        yPos+=hPx;

        p.end();
/*------------------------------------------------------------------------------------------------------------------------------------------------*/

    }


}


#define SEPARATOR ";"

void FabReport::saveReport()
{
    /*
    QVector<int> rowIdx;

    QStringList fieldNames;
    fieldNames
            << tr("Стрічкові ваги:") //0
            << tr("Витрачено води на дифузію:") //1
            << tr("Отримано дифузійного  соку:") //2
            << tr( "Витрачено вапнякового молока:") // 3
            << tr("Подано соку на випарку:") // 4
            << tr("Отримано сиропу:") // 5
            << tr("Пара (РОУ+T1+T2):") // 6
            << tr("Витрачено води на дискові фільтри:") //7
            << tr("Витрачено води на В.С.:") //8
            << tr("Води на вакуум-апарати") //9
            << tr("Перероблено утфелю:") //10
    << tr("Спожито вугілля:") //11
    << tr("Спожито вапняку:") //12
    << tr("Бункерні ваги:") //13
    << tr("Спожито газу:") ; //14

    QVector<double> data;
    data.fill(.0,18);
    QLocale loc(QLocale::C);

    QString fileName = QFileDialog::getSaveFileName(this,
                                QString::fromUtf8("Зберегти файл"),
                                QDir::currentPath(),
                                "Csv (*.csv);;All files (*.*)");
    if(m_ui->twReport->selectionModel()->selectedIndexes().size()>0)
    {
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
         {
            {
            QTextStream out(&file);
            out.setLocale(QLocale::Ukrainian);
            out.setRealNumberNotation(QTextStream::FixedNotation);
            out.setRealNumberPrecision(3);

            out << "sep =" << SEPARATOR << "\n";

            foreach (QModelIndex p, m_ui->twReport->selectionModel()->selectedIndexes()) {
                int index= p.row();
                if(!rowIdx.contains(index))
                {
                    out << tr("Зміна:") << SEPARATOR <<
                                           m_ui->twReport->item(index,0)->text()
                                           << SEPARATOR << m_ui->twReport->item(index,0)->text()
                                           << "\n";

                    qDebug()  << index;
                    for(int i=2;i<17;++i){
                            data[i-2]+=loc.toDouble(m_ui->twReport->item(index,i)->text());
                        }
                    {
                        {
                            QSqlDatabase dbs=QSqlDatabase::database("fabreport",true);
                            if(!dbs.isOpen())
                            {
                                if(!dbs.open())
                                    qDebug() << dbs.lastError();
                            }

                            QSqlQuery qry(dbs);

                            int maxId=0;
                            if(qry.exec(QString("SELECT max(id) FROM hhreport where startTime between '%1' AND '%2';")
                                        .arg(m_ui->twReport->item(index,0)->text())
                                    .arg(m_ui->twReport->item(index,1)->text())))
                            {
                                qry.next();
                                QSqlRecord rec=qry.record();
                                maxId=rec.value(0).toInt();
                            }
                            else
                            {
                                qDebug() << qry.lastError();
                            }
                            //qDebug() << qry.executedQuery();

                            qry.finish();

                            QString query="SELECT startTime, H01, H02, H03, H04, H05, H06, H07, H08, H09, H10, H11, H12,"
                            " G01, G02, G03, G04, G05, G06, G07, G08, G09, G10, G11, G12 FROM hhreport "
                            "WHERE id='%1'";

                            if(qry.exec(query.arg(maxId)))
                            {
                                qry.next();
                                QSqlRecord rec=qry.record();

                                for(int i=0;i<12;++i)
                                {
                                    data[15]+=rec.value(i+1).toDouble();
                                    data[16]+=rec.value(i+13).toDouble();

                                }

                            }
                            else
                            {
                                qDebug() << qry.lastError();
                            }
                            qry.finish();


                         }


                    }

                }
               }

            out << "\n";
               for(int i=0;i<6;++i)
               {
                   out << fieldNames[i]<< SEPARATOR << data[i]<<"\n";
               }

               out << fieldNames[6]<< SEPARATOR<< data[6]+data[7]+data[8]<< SEPARATOR << data[6] << SEPARATOR <<data[7]<< SEPARATOR << data[8] << "\n";
               for(int i=7;i<15;++i)
               {
                   out << fieldNames[i]<< SEPARATOR << data[i+2]<<"\n";
               }
            }
               file.close();
            }
        else
        {
            QMessageBox::warning(this,tr("Попередження"),tr("Не вдалося відкрити файл"),QMessageBox::Ok);
        }

    }
    else
    {
        QMessageBox::warning(this,tr("Попередження"),tr("Не вибрано жодного запису"),QMessageBox::Ok);
    }
    qDebug() << data;
*/

}

