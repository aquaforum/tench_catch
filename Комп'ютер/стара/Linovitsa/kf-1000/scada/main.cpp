#include <QApplication>
#include <QTranslator>
#include <QtSql>

#include <IoNetClient.h>

#include "mainform.h"
#ifdef WIN32

#include <QtPlugin>
Q_IMPORT_PLUGIN(qsvg)
Q_IMPORT_PLUGIN(qsqlmysql)
#endif
int main(int argc,char **argv)
{
    QApplication app(argc,argv);

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("kf1000");
    QApplication::setStyle(new QPlastiqueStyle);

    QSettings s;
    
    IoNetClient net(s.value("/ioserv/hostname","localhost").toString());
    net.setCmode(
#ifdef WIN32
                false
#else
                s.value("/ioserv/cmode",true).toBool()
#endif
                         );

//    QSqlDatabase dbs=QSqlDatabase::addDatabase("QMYSQL");

//    dbs.setHostName(s.value("/db/host","localhost").toString());
//    dbs.setDatabaseName(s.value("/db/db","vipgr").toString());
//    dbs.setUserName(s.value("/db/username","scada").toString());
//    dbs.setPassword(s.value("/db/passwd","").toString());

//    if( ! dbs.open())
//	qDebug() << "No connect to database";
    
    
    QStackedWidget main;
    QColor cl;
    QPalette pal;
    cl.setRgb(90,132,201);
    pal.setColor(QPalette::Background,cl);
    main.setPalette(pal);

    QTranslator translator;
    translator.load("filters_"+QLocale::system().name(),":/translate");
    app.installTranslator(&translator);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    
    mMainForm *mainForm = new mMainForm(net);
    
    main.addWidget(mainForm);
    main.setCurrentWidget(mainForm);

    //QObject::connect(mainForm->Exit,SIGNAL(clicked()),&main,SLOT(close()));

    if( QApplication::desktop()->size().height()>768 )
    {
        main.resize(1366,768);
        main.show();
    }
    else
    {
        main.showFullScreen();
    }

    return app.exec();
}


