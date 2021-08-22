#include "fabreport.h"
#include "ui_fabreport.h"
#include <QVBoxLayout>

#include <datetimedialog.h>

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

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <IoNetClient.h>

#define DIFUZ (*(src[0]))


FabReport::FabReport(QVector<IoNetClient *> &source, QWidget *parent):  QWidget(parent),
    src(source),
    m_ui(new Ui::FabReport)
{
    m_ui->setupUi(this);


    QSettings set;
    QSqlDatabase dbs=QSqlDatabase::addDatabase("QMYSQL","fabreport");

    dbs.setHostName(set.value("/db/hostname","localhost").toString());
    dbs.setDatabaseName("report");
    dbs.setUserName(set.value("/db/username","scada").toString());
    dbs.setPassword(set.value("/db/passwd","").toString());
    if(!dbs.open()) // спробувати відкрити
    {
        qDebug() << dbs.lastError().databaseText();
    }


    mRepModel = new QSqlQueryModel(this);

    mRepModel->setQuery(QString("SELECT startTime,stopTime,FORMAT(SGstr,1),FORMAT(SVvoda,1),FORMAT(SVds,1),FORMAT(SVvm,1),"
                                "FORMAT(SVsvp,1),FORMAT(SVsfv,1), FORMAT(GRou,2),FORMAT(GT1,1),FORMAT(GT2,1), FORMAT(SVvfd,1),"
                                "FORMAT(SVvtv,1),FORMAT(SVvva,1), FORMAT(SGutf,1), FORMAT(SGcl,1), FORMAT(SGst,1) FROM report") ,dbs);


    if(mRepModel->lastError().isValid())
    {
        qDebug() << mRepModel->lastError();
    }

    mRepModel->setHeaderData(0,Qt::Horizontal,tr("Початок"));
    mRepModel->setHeaderData(1,Qt::Horizontal,tr("Закінчення"));

    mRepModel->setHeaderData(2,Qt::Horizontal,tr("Буряк"));
    mRepModel->setHeaderData(3,Qt::Horizontal,tr("Вода на диф."));
    mRepModel->setHeaderData(4,Qt::Horizontal,tr("Дифуз. сік"));

    mRepModel->setHeaderData(5,Qt::Horizontal,tr("Вапн. мол."));

    mRepModel->setHeaderData(6,Qt::Horizontal,tr("Сік на В.С."));
    mRepModel->setHeaderData(7,Qt::Horizontal,tr("Сироп з В.С."));
    mRepModel->setHeaderData(8,Qt::Horizontal,tr("Пара РОУ"));
    mRepModel->setHeaderData(9,Qt::Horizontal,tr("Пара Т1"));
    mRepModel->setHeaderData(10,Qt::Horizontal,tr("Пара Т2"));

    mRepModel->setHeaderData(11,Qt::Horizontal,tr("Вода на Д.Ф."));
    mRepModel->setHeaderData(12,Qt::Horizontal,tr("Вода на В.С."));
    mRepModel->setHeaderData(13,Qt::Horizontal,tr("Вода на В-А"));


    mRepModel->setHeaderData(14,Qt::Horizontal,tr("Утфель"));
    mRepModel->setHeaderData(15,Qt::Horizontal,tr("Вугілля"));
    mRepModel->setHeaderData(16,Qt::Horizontal,tr("Вапняк"));



    m_ui->twReport->setModel(mRepModel);

    m_ui->twReport->setColumnWidth(0, 180 );
    m_ui->twReport->setColumnWidth(1 ,180 );
    m_ui->twReport->setColumnWidth(2 , 100 );
    m_ui->twReport->setColumnWidth(3 , 100 );
    m_ui->twReport->setColumnWidth(4 , 100 );
    m_ui->twReport->setColumnWidth(5 , 100 );
    m_ui->twReport->setColumnWidth(6 , 100 );
    m_ui->twReport->setColumnWidth(7 , 100 );
    m_ui->twReport->setColumnWidth(8 , 100 );
    m_ui->twReport->setColumnWidth(9 , 100 );
    m_ui->twReport->setColumnWidth(10 , 100  );
    m_ui->twReport->setColumnWidth(11 , 100 );
    m_ui->twReport->setColumnWidth(12 , 100 );
    m_ui->twReport->setColumnWidth(13 , 100 );
    m_ui->twReport->setColumnWidth(14 , 100 );
    m_ui->twReport->setColumnWidth(15 , 100 );
    m_ui->twReport->setColumnWidth(16 , 100 );



    connect(m_ui->bnPrint,SIGNAL(clicked()),this,SLOT(printReport()));
    connect(m_ui->bnSave,SIGNAL(clicked()),this,SLOT(saveReport()));




}


FabReport::~FabReport()
{
    {
        QSqlDatabase dbs=QSqlDatabase::database("fabreport");
        dbs.close();
    }
    QSqlDatabase::removeDatabase("fabreport");
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

void FabReport::printReport()
{


    int nLine=0;
    nLine=m_ui->twReport->currentIndex().row();
    if(nLine<0)// не вибрано жодного рядка
    {
        nLine=m_ui->twReport->model()->rowCount()-1 ;
    }




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
    << tr("Спожито газу:") //14
    << tr("KF-1000 циклів:"); // 15


    QString out="<html><head><style>"
            ".fat {border-style: 5px solid black;}"
            "</style></head><body>";

    out.append(QString(tr("<p align=\"center\"><span style=\"font-size:16pt; font-weight:600;\">Звіт за зміну</span></p>"
                       "<p align=\"center\"><span style=\"font-size:12pt; font-weight:600;\">від %1 до %2</span></p>"))
               .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,0)).toDateTime().toString("hh:mm yyyy/MM/dd"))
               .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,1)).toDateTime().toString("hh:mm yyyy/MM/dd")));

    out.append("<span style=\"font-size:10pt; font-weight:400;\"><table border=\"0\" style=\"margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"2\" cellpadding=\"0\">");

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[0])
           .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,2)).toString())
           .arg(tr("тон")));

    // бункерні ваги


    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[1])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,3)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[2])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,4)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[3])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,5)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[4])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,6)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[5])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,7)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2+%3+%4=%5</p></td><td><p>%6</p></td></tr>")
              .arg(fieldNames[6])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,8)).toString())
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,9)).toString())
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,10)).toString())
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,8)).toDouble()+
                 m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,9)).toDouble()+
                 m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,10)).toDouble()
                 ,0,'f',1)
            .arg(tr("т")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[7])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,11)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[8])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,12)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[9])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,13)).toString())
           .arg(tr("м<span style=\" vertical-align:super;\">3</span>")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[10])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,14)).toString())
           .arg(tr("тон")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[11])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,15)).toString())
           .arg(tr("кг")));

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
              .arg(fieldNames[12])
            .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,16)).toString())
           .arg(tr("кг")));
// ** звіт з KF-10000 додано в 2019р.
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
                    .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,0)).toDateTime().toString("yyyy/MM/dd hh:mm:ss"))
                .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,1)).toDateTime().toString("yyyy/MM/dd hh:mm:ss"))))
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

    out.append(QString("<tr><td width=\"70%\"><p>%1</p></td><td width=\"30%\"><p>%2</p></td><td><p>%3</p></td></tr>")
               .arg(fieldNames[15])
             .arg(kfN)
            .arg(tr(" ")));
//-----------------------------------------

    out.append("</table></span>");
    out.append(tr("<p align=\"left\"><span style=\"font-size:12pt; font-weight:400;\">Почасовий звіт</span></p>"));
    {
        QSqlDatabase dbs=QSqlDatabase::database("fabreport",false);
        QSqlQuery qry(dbs);

        int maxId=0;
        if(qry.exec(QString("SELECT max(id) FROM hhreport where startTime between '%1' AND '%2';")
                    .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,0)).toDateTime().toString("yyyy/MM/dd hh:mm:ss"))
                .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,1)).toDateTime().toString("yyyy/MM/dd hh:mm:ss"))))
        {
            qry.next();
            QSqlRecord rec=qry.record();
            maxId=rec.value(0).toInt();
        }
        //qDebug() << qry.executedQuery();

        qry.finish();

        QString query="SELECT startTime, H01, H02, H03, H04, H05, H06, H07, H08, H09, H10, H11, H12,"
        " G01, G02, G03, G04, G05, G06, G07, G08, G09, G10, G11, G12 FROM hhreport "
        "WHERE id='%1'";

        double massSum=0.0,gassSum=0.0;

        if(qry.exec(query.arg(maxId)))
        {
            qry.next();
            QSqlRecord rec=qry.record();
            out.append(QString(tr("<p>Дата і час: %1 </p>")).arg(rec.value(0).toString()));
            out.append("<span style=\"font-size:10pt; font-weight:400;\">"
                       "<table style=\"border-style: 5px solid black;\">"); // style=\"margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"2\" cellpadding=\"0\">");
            out.append(tr("<thead><tr><th width=\"20%\">Час зміни</th>"
                               "<th width=\"40%\">Буряк</th>"
                               "<th width=\"40%\">Газ</th></tr></thead><tfoot>"));

            int countOk=0;
            double massSumAll=0.0;

            for(int i=0;i<12;++i)
            {
                out.append(QString("<tr><td width=\"20%\"><p>%1</p>"
                                   "<td width=\"40%\"><p>%2</p>"
                                   "<td width=\"40%\"><p>%3</p>")
                           .arg(i==0?12:i).arg(rec.value(i+1).toString()).arg(rec.value(i+13).toString()));

                double gas=rec.value(i+13).toDouble();
                if(gas>0.0)
                {
                    ++countOk;
                    massSum+=rec.value(i+1).toDouble();
                    gassSum+=gas;
                }
                massSumAll+=rec.value(i+1).toDouble();

            }
            out.append(QString(tr("<tr><td width=\"40%\"><p>Сумарно</p>"
                               "<td width=\"30%\"><p>%1</p>"
                               "<td width=\"30%\"><p>%2</p>"))
                       .arg(massSumAll  ,0,'f',2).arg(gassSum,0,'f',2));
            out.append("</tfoot></table>");

            if(countOk>0)
            {
                out.append(QString(tr("<p>Співвідношення газ/буряк: %1 ")).arg(gassSum/massSum,0,'f',1));
            if(countOk<12)
                out.append(QString(tr(" (за %2 год)")).arg(countOk));
            }

        }

// співвідношення стружка/відкачка 2019р------
        {
            double dfS=0.0;
            QSqlDatabase kfDbs=QSqlDatabase::addDatabase("QMYSQL","difuz");

            kfDbs.setHostName("difuz");
            kfDbs.setDatabaseName("difuz");
            kfDbs.setUserName("scada");
            kfDbs.setPassword("");
            if(!kfDbs.open()) // спробувати відкрити
            {
                qDebug() << kfDbs.lastError().databaseText();
            }
            QSqlQuery qry(kfDbs);
            QString strQry=QString("SELECT avg(V_15)*12.0 FROM trend where Dt between '%1' AND '%2';")
                    .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,0)).toDateTime().toTime_t() - 3600)
                .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(nLine,1)).toDateTime().toTime_t()-3600);

            if(qry.exec(strQry))
            {
                qry.next();
                QSqlRecord rec=qry.record();
                dfS=DIFUZ[0]->scaleZero("V_15")+ rec.value(0).toDouble()/4000.0*(DIFUZ[0]->scaleFull("V_15")-DIFUZ[0]->scaleZero("V_15"));

            }
            else
            {
                qDebug() << qry.lastError().text();
            }


//            qDebug() << qry.executedQuery();
//            qDebug() << "dfS" << dfS;

            qry.finish();
            kfDbs.close();
            out.append(QString(tr("<p>Відкачка з дифузії: %1 %")).arg(dfS/massSum*100.0,0,'f',1));

        }
        QSqlDatabase::removeDatabase("difuz");




    }

    out.append("</body></html>");

    //qDebug() << out;

        // друкуємо звіт
        QPrinter printer(QPrinter::PrinterResolution);

        //printer.setOutputFormat(QPrinter::PdfFormat);
        //printer.setOutputFileName( "/tmp/test.pdf");  //  по другому printer.setOutputFileName("Test.pdf");

        //вибрати прінтер
        QPrintDialog pDialg(&printer,this);

        if(pDialg.exec()==QDialog::Accepted)
        {
            QTextDocument tDoc;

            QSizeF paperSize;
            paperSize.setWidth(printer.width());
            paperSize.setHeight(printer.height());

            tDoc.setPageSize(paperSize); // the document needs a valid PageSize
            tDoc.setHtml(out);

            tDoc.print(&printer);
        }


}


#define SEPARATOR ";"

void FabReport::saveReport()
{
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
                                           m_ui->twReport->model()->data(m_ui->twReport->model()->index(index,0)).toString()
                                           << SEPARATOR << m_ui->twReport->model()->data(m_ui->twReport->model()->index(index,0)).toString()
                                           << "\n";

                    qDebug()  << index;
                    for(int i=2;i<17;++i){
                            data[i-2]+=loc.toDouble(m_ui->twReport->model()->data(m_ui->twReport->model()->index(index,i)).toString());
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
                                        .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(index,0)).toDateTime().toString("yyyy/MM/dd hh:mm:ss"))
                                    .arg(m_ui->twReport->model()->data(m_ui->twReport->model()->index(index,1)).toDateTime().toString("yyyy/MM/dd hh:mm:ss"))))
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

}

