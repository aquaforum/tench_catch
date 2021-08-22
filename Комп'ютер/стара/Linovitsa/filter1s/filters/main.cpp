#include <QApplication>
#include <QTranslator>
#include <QtSql>

#include <IoNetClient.h>
#include <QStackedWidget>

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
    QCoreApplication::setApplicationName("filters");

#if QT_VERSION >QT_VERSION_CHECK(5,0,0)
    QApplication::setStyle("fusion");
#else
    QApplication::setStyle(new QPlastiqueStyle);
#endif

    QSettings s;
    
    IoNetClient net( s.value("/ioserv/hostname","localhost").toString());
    net.setCmode(
#ifdef WIN32
                false
#else
                s.value("/ioserv/cmode",true).toBool()
#endif
    );
    QStackedWidget main;
    QColor cl;
    QPalette pal;
    cl.setRgb(90,132,201);
    pal.setColor(QPalette::Background,cl);
    main.setPalette(pal);

    QTranslator translator;
    translator.load("filters_"+QLocale::system().name(),":/translate");
    app.installTranslator(&translator);

#if QT_VERSION >QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#else
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
    
    mMainForm *mainForm = new mMainForm(net);
    
    main.addWidget(mainForm);
    main.setCurrentWidget(mainForm);

/*
    if(QApplication::desktop()->size().width()>1366 && QApplication::desktop()->size().height()> 768)
    {
        main.resize(1366,768);
        main.show();
    }
    else
    {
        main.showFullScreen();
    }
*/
        main.showFullScreen();

    return app.exec();
}


