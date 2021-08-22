#include <QApplication>
#include <QTranslator>
#include <QtSql>

#include <IoNetClient.h>
#include <QStackedWidget>
#include <QMessageBox>
#include <QDesktopWidget>

#include "mainform.h"
#ifdef WIN32

#include <QtPlugin>
Q_IMPORT_PLUGIN(qsvg)
Q_IMPORT_PLUGIN(qsqlmysql)
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

    QCoreApplication::setOrganizationName("Lynovitza");
    QCoreApplication::setApplicationName("Video");
    //QApplication::setStyle(new QPlastiqueStyle);

    QSettings s;
    

    myStackedWidget main;
    QColor cl;
    QPalette pal;
    cl.setRgb(90,132,201);
    pal.setColor(QPalette::Background,cl);
    main.setPalette(pal);

    //QTranslator translator;
    //translator.load("scada_"+QLocale::system().name(),":/translate");
    //app.installTranslator(&translator);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    
    mMainForm *mainForm = new mMainForm(&main);
    
    main.addWidget(mainForm);
    main.setCurrentWidget(mainForm);

    //QObject::connect(mainForm->Exit,SIGNAL(clicked()),&main,SLOT(close()));
/*
    if(QApplication::desktop()->screenGeometry(-1).width()>1440 && QApplication::desktop()->screenGeometry(-1).height()> 900)
    {
        main.resize(1440,900);
        main.show();
    }
    else */
       main.showFullScreen();

    return app.exec();
}


