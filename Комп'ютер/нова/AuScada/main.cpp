#include <QApplication>
#include <QTranslator>

#include <aulib.h>
#include <QDebug>

#include <QIcon>
#include <QSettings>
#include <QWidget>
#include <QProcess>

#include <mainwidget.h>
#include <QSqlDatabase>
#include <QDir>

#include <QDebug>

#include "gaspich/gpreportwr.h"
#include "reporwrmgr.h"
#include <QFile>

#define GPI4    QStringLiteral("plcgpi4")


int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    QCoreApplication::setOrganizationName(QStringLiteral("Линовиця"));
    QCoreApplication::setApplicationName(QStringLiteral("AuScada"));

    QApplication::setStyle("fusion");

    QIcon appIcon(":/butons/pict/iph_logo_32x32.png");
    app.setWindowIcon(appIcon);

    // ця фігня скопіює налаштування в реєстра з файла.
#ifdef WIN32
    if(QFile::exists("AuScada.ini"))
    {
        {
            QSettings s;
            s.clear();
            QSettings o("AuScada.ini",QSettings::IniFormat);
            foreach(QString k,o.allKeys())
            {

                s.setValue(k,o.value(k));
            }
            s.sync();
        }
        QFile::remove("AuScada.ini");
    }

#endif

    AuSetup set;
    //qDebug() << set.base().plcList();
    // настройка апаратів
    {
        QSettings s;
        QStringList apPlc;
        apPlc << "tmaob"
              << "tmaap1"
              << "tmaap2"
              << "tmaap3";
        foreach (QString plcName, apPlc) {
            foreach(QString tagName,set.base()[plcName].tagsList())
            {
                if((set.base()[plcName][tagName].dataType()==tagType::INT || set.base()[plcName][tagName].dataType()==tagType::REAL)
                    && !s.contains(QStringLiteral("%1/%2/%3").arg(plcName).arg("Base").arg(tagName)))
                {
                    s.setValue(QStringLiteral("%1/%2/%3").arg(plcName).arg("Base").arg(tagName),100.0);
                }

/*
                if((set.base()[plcName][tagName].dataType()==tagType::INT || set.base()[plcName][tagName].dataType()==tagType::REAL)
                    && !s.contains(QStringLiteral("%1/%2/%3").arg(plcName).arg("Decimal").arg(tagName)))
                {
                    s.setValue(QStringLiteral("%1/%2/%3").arg(plcName).arg("Decimal").arg(tagName),1);
                }
*/
            }
        }
    }

    MainWidget main(set.base());
    //FormTest main(set.base());

    QColor cl;
    QPalette pal;
    cl.setRgb(90,132,201);
    pal.setColor(QPalette::Background,cl);
    main.setPalette(pal);

    // створення писалки звітів газової
    ReporWrMgr *rep=nullptr;
    {
        QSettings s;
        qint32 commType=s.value("plcgpi4",-1).toInt();
        if(commType==LOCALONLY
        || commType==LOCALMON
        || commType==LOCALFULL )
        {
            GpReportWiter *gpRep= new GpReportWiter(set.base()[GPI4]);
        }
        // зпустити писалку звітів, якщо включено.
        if(s.value("FabReportWriter",0).toInt()!=0)
        {
            QSqlDatabase dbs=QSqlDatabase::database(AUDBNAME,true);
            if(!dbs.isValid()) // якщо бази даних нема бо цей комп без історії - створити її
            {
                dbs=QSqlDatabase::addDatabase(
                    #ifndef WIN32
        //                    "QSQLITE3"
                            "QSQLITE"
                    #else
                            "QSQLITE"
                    #endif
                            ,AUDBNAME);
                dbs.setDatabaseName(DBFILE);
                dbs.open();
            }

            rep = new ReporWrMgr(set.base()); //
        }
    }

// відключити гасіння екрану
/*
#ifdef __linux__
    QProcess xset;
    xset.start("xset s off && xset -dpms");
#endif
*/


    main.showFullScreen();
    //main.showMaximized();
    //main.show();

    //if(rep!=nullptr)
    //    delete rep;
    qDebug() << "Starter at: " << QDateTime::currentDateTime();


    return app.exec();;
}


