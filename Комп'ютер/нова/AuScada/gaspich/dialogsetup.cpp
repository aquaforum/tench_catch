#include "dialogsetup.h"
#include "ui_dialogsetup.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMessageBox>

//#include "aulib.h"

DialogSetup::DialogSetup(AuPlc &s, QWidget *parent) :
    QDialog(parent),bAccess(true),
    ui(new Ui::DialogSetup),
    src(s)
{
    ui->setupUi(this);

    connect(ui->bnOk,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(ui->bnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    ui->maxTaGcl->setValue(src[QStringLiteral("maxTaGcl")]);
    ui->maxTaGst->setValue(src[QStringLiteral("maxTaGst")]);

    ui->subGcl->setValue(src[QStringLiteral("subGcl")]);
    ui->subGst->setValue(src[QStringLiteral("subGst")]);

    ui->zdTloadCl->setValue(src[QStringLiteral("zdTloadCl")]);
    ui->zdLoadSt->setValue(src[QStringLiteral("zdLoadSt")]);
    ui->zdDelayCl->setValue(src[QStringLiteral("zdDelayCl")]);
    ui->zdTPostCl->setValue(src[QStringLiteral("zdTPostCl")]);

    ui->zdTalarmStart->setValue(src[QStringLiteral("zdTalarmStart")]);
    ui->zdTwaitStart->setValue(src[QStringLiteral("zdTwaitStart")]);
    ui->zdTalarmLift->setValue(src[QStringLiteral("zdTalarmLift")]);
    ui->zdTwaitLift->setValue(src[QStringLiteral("zdTwaitLift")]);


    ui->zdTn01->setValue(src[QStringLiteral("zdTn01")]);
    ui->zdTn02->setValue(src[QStringLiteral("zdTn02")]);
    ui->zdTn03->setValue(src[QStringLiteral("zdTn03")]);

    ui->zdTv01->setValue(src[QStringLiteral("zdTv01")]);
    ui->zdTv02->setValue(src[QStringLiteral("zdTv02")]);
    ui->zdTv03->setValue(src[QStringLiteral("zdTv03")]);


    ui->zdTliftTR->setValue(src[QStringLiteral("zdTliftTR")]);
    ui->zdTUpWait->setValue(src[QStringLiteral("zdTUpWait")]);
    ui->zdTark->setValue(src[QStringLiteral("zdTark")]);
    ui->zdTTorm->setValue(src[QStringLiteral("zdTTorm")]);

    ui->zdTaccelUp->setValue(src[QStringLiteral("zdTaccelUp")]);
    ui->zdTaccelDown->setValue(src[QStringLiteral("zdTaccelDown")]);

    ui->ZdTShovel->setValue(src[QStringLiteral("ZdTShovel")]);

    ui->iKGcl->setValue(src[QStringLiteral("iKGcl")]);
    ui->iKGst->setValue(src[QStringLiteral("IKGst")]);


}

DialogSetup::~DialogSetup()
{
    delete ui;
}

void DialogSetup::slotAccept()
{


    src[QStringLiteral("maxTaGcl")]=ui->maxTaGcl->value();
    src[QStringLiteral("maxTaGst")]=ui->maxTaGst->value();

    src[QStringLiteral("subGcl")]=ui->subGcl->value();
    src[QStringLiteral("subGst")]=ui->subGst->value();




    src[QStringLiteral("zdTloadCl")]=qint32(ui->zdTloadCl->value());
    src[QStringLiteral("zdLoadSt")]=qint32(ui->zdLoadSt->value());
    src[QStringLiteral("zdDelayCl")]=qint32(ui->zdDelayCl->value());
    src[QStringLiteral("zdTPostCl")]=qint32(ui->zdTPostCl->value());


    src[QStringLiteral("zdTalarmStart")]=qint32(ui->zdTalarmStart->value());
    src[QStringLiteral("zdTwaitStart")]=qint32(ui->zdTwaitStart->value());
    src[QStringLiteral("zdTalarmLift")]=qint32(ui->zdTalarmLift->value());
    src[QStringLiteral("zdTwaitLift")]=qint32(ui->zdTwaitLift->value());

    src[QStringLiteral("zdTark")]=qint32(ui->zdTark->value());

    src[QStringLiteral("zdTn01")]=qint32(ui->zdTn01->value());
    src[QStringLiteral("zdTn02")]=qint32(ui->zdTn02->value());
    src[QStringLiteral("zdTn03")]=qint32(ui->zdTn03->value());

    src[QStringLiteral("zdTv01")]=qint32(ui->zdTv01->value());
    src[QStringLiteral("zdTv02")]=qint32(ui->zdTv02->value());
    src[QStringLiteral("zdTv03")]=qint32(ui->zdTv03->value());

    src[QStringLiteral("zdTUpWait")]=qint32(ui->zdTUpWait->value());
    src[QStringLiteral("zdTTorm")]=qint32(ui->zdTTorm->value());

    src[QStringLiteral("zdTliftTR")]=qint32(ui->zdTliftTR->value());

    src[QStringLiteral("zdTaccelUp")]=qint32(ui->zdTaccelUp->value());
    src[QStringLiteral("zdTaccelDown")]=qint32(ui->zdTaccelDown->value());
    src[QStringLiteral("ZdTShovel")]=qint32(ui->ZdTShovel->value());

    src[QStringLiteral("iKGcl")]=ui->iKGcl->value();
    src[QStringLiteral("IKGst")]=ui->iKGst->value();

    accept();
}



