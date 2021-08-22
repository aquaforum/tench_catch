#include "apzagtrendselect.h"
#include "ui_apzagtrendselect.h"
#include "mainwidget.h"
#include <aulib.h>

ApZagTrendSelect::ApZagTrendSelect(AuBase &base, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApZagTrendSelect),
    src(base)
{
    ui->setupUi(this);
    resize(parent->size());

    connect(ui->bnTrend1,&QPushButton::clicked,this,&ApZagTrendSelect::slotShowTrend);
    connect(ui->bnTrend2,&QPushButton::clicked,this,&ApZagTrendSelect::slotShowTrend);
    connect(ui->bnTrend3,&QPushButton::clicked,this,&ApZagTrendSelect::slotShowTrend);
}

ApZagTrendSelect::~ApZagTrendSelect()
{
    delete ui;
}

void ApZagTrendSelect::slotShowTrend()
{
    int v=sender()->objectName().right(1).toInt();
    QStringList tags;
    switch (v) {
    case 1:
        tags << "tmaob,k,Ai7_i,Ai8_i,Ai16_i";
        break;
    case 2:
        tags << "tmaob,Ai2_i,Ai7_i,Ai4_i,Ai5_i,Ai1_i,Ai14_i,Ai15_i";

        break;
    case 3:
        tags << "tmaob,Ai10_i,Ai9_i";

        break;
    case 4:
        break;
    default:
        break;
    }

    MainWidget *mW=qobject_cast<MainWidget*>( parentWidget()->parentWidget()->parentWidget()->parentWidget()) ;

    AuTrendView *pTrw  = new AuTrendView(src,tags,mW);
    pTrw->setText(qobject_cast<QPushButton*>(sender())->text());

    mW->slotShowTrend(pTrw);

}
