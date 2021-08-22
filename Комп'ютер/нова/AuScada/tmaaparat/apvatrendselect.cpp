#include "apvatrendselect.h"
#include "ui_apvatrendselect.h"
#include "mainwidget.h"
#include <aulib.h>

#include <QDebug>

ApVaTrendSelect::ApVaTrendSelect(AuBase &base, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApVaTrendSelect),
    src(base)
{
    ui->setupUi(this);

    resize(parent->size());

    connect(ui->bnTrend1,&QPushButton::clicked,this,&ApVaTrendSelect::slotShowTrend);
    connect(ui->bnTrend2,&QPushButton::clicked,this,&ApVaTrendSelect::slotShowTrend);
    connect(ui->bnTrend3,&QPushButton::clicked,this,&ApVaTrendSelect::slotShowTrend);

}

ApVaTrendSelect::~ApVaTrendSelect()
{
    delete ui;
}


void ApVaTrendSelect::slotShowTrend()
{
    int v=sender()->objectName().right(1).toInt();
    QStringList tags;
    QStringList wK;

    switch (v) {
    case 1:
        tags << QStringLiteral("tmaap%1,%2").arg(objectName().right(1)).arg(
              "Tt4a_ind,"
              "Termo_ind,"
              "Lt1a_ind,"
              "Uroven_ind,"
              "Dt2a_ind,"
              "Cv_ind,"
              "Delta,"
              "Delta_ind,"
              "Sy20a_ind,"
              "Ob_ind,"
              "Ey20a_ind,"
              "Moment_ind,"
              "Pt6a_ind,"
              "Vakuum_ind,"
              "Kod_blok");
            wK<< "1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,-1.0,1.0,1.0";
        break;
    case 2:
        tags << "tmaob,Ai10_i,Ai9_i";
        tags << QStringLiteral("tmaap%1,%2").arg(objectName().right(1)).arg(
                    "Pt6a_ind,"
                    "Pt5a_ind"
                    );
        wK<< "1.0,1.0";
        wK<< "1.0,1.0";
        break;
    case 3:
        tags << QStringLiteral("tmaap%1,%2").arg(objectName().right(1)).arg(
              "Lt1a_ind,"
              "Dt2a_ind,"
              "Cv_ind,"
              "Delta,"
              "Sy20a_ind,"
              "Ey20a_ind,"
              "K7g_c,"
              "K8g_c,"
              "K10g_c"      );
        wK<< "1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0";
        break;
    case 4:
        break;
    default:
        break;
    }

    MainWidget *mW=qobject_cast<MainWidget*>( parentWidget()->parentWidget()->parentWidget()->parentWidget()) ;

    AuTrendView *pTrw  = new AuTrendView(src,tags,wK,mW);
    pTrw->setText(qobject_cast<QPushButton*>(sender())->text());

    mW->slotShowTrend(pTrw);

}
