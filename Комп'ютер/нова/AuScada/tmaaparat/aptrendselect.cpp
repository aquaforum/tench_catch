#include "aptrendselect.h"
#include "ui_aptrendselect.h"

#include <aulib.h>

#include "tmaaparat/apmainform.h"


#include <QDebug>

ApTrendSelect::ApTrendSelect(AuBase &base, ApMainForm *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApTrendSelect),
    src(base)
{
    ui->setupUi(this);
    resize(parent->size());

    connect(ui->bnAp1,&QPushButton::clicked,p,&ApMainForm::slotChangeMnemo);
    connect(ui->bnAp2,&QPushButton::clicked,p,&ApMainForm::slotChangeMnemo);
    connect(ui->bnAp3,&QPushButton::clicked,p,&ApMainForm::slotChangeMnemo);
    connect(ui->bnAp4,&QPushButton::clicked,p,&ApMainForm::slotChangeMnemo);

}

ApTrendSelect::~ApTrendSelect()
{
    delete ui;
}

void ApTrendSelect::slotChangeMnemo()
{



}
