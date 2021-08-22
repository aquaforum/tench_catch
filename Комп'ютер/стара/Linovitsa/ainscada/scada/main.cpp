#include <QApplication>
#include <QTranslator>
#include <QtSql>
#include <QMessageBox>

#include <IoNetClient.h>

#include "mainform.h"
#ifdef WIN32
#if QT_VERSION <QT_VERSION_CHECK(5,0,0)

#include <QtPlugin>
Q_IMPORT_PLUGIN(qsvg)
Q_IMPORT_PLUGIN(qsqlmysql)
#endif

#endif


class myStackedWidget : public QStackedWidget
{
public:
    myStackedWidget(QWidget *p=NULL);
protected:
    void closeEvent(QCloseEvent *event);
};

myStackedWidget::myStackedWidget(QWidget *p): QStackedWidget(p)
{
}

void myStackedWidget::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::warning(this,tr("Попередження"),tr("Роботу програми буде завершено\nПродовжити?"),
                              QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

int main(int argc,char **argv)
{
    QApplication app(argc,argv);
#if QT_VERSION >QT_VERSION_CHECK(5,0,0)
    QApplication::setStyle("fusion");
#else
    QApplication::setStyle(new QPlastiqueStyle);
#endif

#if QT_VERSION >QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#else
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

    QCoreApplication::setOrganizationName("Lynovitza");
    QStringList hosts;
    bool cMode[8];

    {
        QCoreApplication::setApplicationName("difuz");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[0]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[0]=false;
#endif
    }

    {
        QCoreApplication::setApplicationName("satur");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[1]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[1]=false;
#endif
    }

    {
        QCoreApplication::setApplicationName("viparka");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[2]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[2]=false;
#endif
    }

    {
        QCoreApplication::setApplicationName("aparat");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[3]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[3]=false;
#endif
    }


    {
        QCoreApplication::setApplicationName("centrif");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[4]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[4]=false;
#endif
    }

    {
        QCoreApplication::setApplicationName("gaspich");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[5]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[5]=false;
#endif
    }

    {
        QCoreApplication::setApplicationName("rou");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[6]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[6]=false;
#endif
    }

    {
        QCoreApplication::setApplicationName("difuz");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();
#ifndef WIN32
        cMode[7]=s.value("/ioserv/cmode",true).toBool();
#else
        cMode[7]=false;
#endif
    }
    IoNetClient net_d(hosts[0]);
    net_d.setObjectName("difuz");
    net_d.setCmode(cMode[0]);


    IoNetClient net_s(hosts[1]);
    net_s.setObjectName("satur");
    net_s.setCmode(cMode[1]);

    IoNetClient net_v(hosts[2]);
    net_v.setObjectName("viparka");
    net_v.setCmode(cMode[2]);

    IoNetClient net_a(hosts[3]);
    net_a.setObjectName("aparat");
    net_a.setCmode(cMode[3]);


    IoNetClient net_c(hosts[4],8185);
    net_c.setObjectName("centrif");
    net_c.setCmode(cMode[4]);

    IoNetClient net_g(hosts[5]);
    net_g.setObjectName("gaspich");
    net_g.setCmode(cMode[5]);

    IoNetClient net_r(hosts[6]);
    net_r.setObjectName("rou");
    net_r.setCmode(cMode[6]);


    IoNetClient net_f(hosts[7],8185);
    net_f.setObjectName("fqdrvmon");
    net_f.setCmode(cMode[7]);


    QVector<IoNetClient*> srca;
    srca << &net_d << &net_s << &net_v << &net_a << &net_c << &net_g  << &net_r << &net_f;

    
    myStackedWidget main;
    QColor cl;
    QPalette pal;
    cl.setRgb(90,132,201);
    pal.setColor(QPalette::Background,cl);
    main.setPalette(pal);



    
    mMainForm *mainForm = new mMainForm(srca,&main);
    
    main.addWidget(mainForm);
    main.setCurrentWidget(mainForm);

    //QObject::connect(mainForm->Exit,SIGNAL(clicked()),&main,SLOT(close()));


/*
    if(QApplication::desktop()->size().width()>1680 && QApplication::desktop()->size().height()> 1050)
    {
        main.resize(1680,1050);
        main.show();
    }
    else
    { */
        main.showFullScreen();
//    }
//    main.resize(1280,1024);
//    main.show();


    return app.exec();
}


