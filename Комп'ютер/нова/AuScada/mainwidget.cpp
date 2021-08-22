#include "mainwidget.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSettings>

#include "mainform.h"
#include "tmaaparat/apmainform.h"

#include <AuWidget/autrendview.h>
#include <aubase.h>


#define FABMNEMO 0
#define APMNEMO 1

MainWidget::MainWidget(AuBase &base, QWidget *parent) : QWidget(parent),
    src(base)
{
    vbl=new QVBoxLayout(this);
    vbl->setMargin(0);
    setLayout(vbl);

    QSettings set;
    switch (set.value("topMnemo",FABMNEMO).toInt()) {
    case APMNEMO:
        currentWidget=new ApMainForm(src,this);
        break;
    default:
        currentWidget=new mMainForm(src,this);;
        break;
    }


    vbl->addWidget(currentWidget);


}

void MainWidget::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::warning(this,tr("Попередження"),tr("Роботу програми буде завершено\nЗакрити програму?"),
                              QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWidget::slotShowTrend(AuTrendView* trw)
{
    vbl->removeWidget(currentWidget);
    currentWidget->deleteLater();

    currentWidget=trw;

    vbl->addWidget(currentWidget);
    connect(trw,&AuTrendView::finished,this,&MainWidget::slotRetTrend);
}

void MainWidget::slotRetTrend()
{
    vbl->removeWidget(currentWidget);
    currentWidget->deleteLater();

    // тепер треба вернутися на попередній кадр

    QSettings set;
    switch (set.value("topMnemo",FABMNEMO).toInt()) {
    case APMNEMO:
        currentWidget=new ApMainForm(src,this);
        break;
    default:
        currentWidget=new mMainForm(src,this);;
        break;
    }

    vbl->addWidget(currentWidget);
}

void MainWidget::slotShowMain()
{
    vbl->removeWidget(currentWidget);
    currentWidget->deleteLater();

    mMainForm *mainForm = new mMainForm(src,this);
    vbl->addWidget(mainForm);

    currentWidget=mainForm;
    QSettings set;
    set.setValue("topMnemo",FABMNEMO);
}


void MainWidget::slotShowAparat()
{
    vbl->removeWidget(currentWidget);
    currentWidget->deleteLater();

    ApMainForm *ap=new ApMainForm(src,this);
    vbl->addWidget(ap);

    currentWidget=ap;

    QSettings set;
    set.setValue("topMnemo",APMNEMO);
}
