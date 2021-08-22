#include "sparamdialog.h"
#include "ui_sparamdialog.h"

#include "aulib.h"

#include <QSettings>
#include <QMessageBox>
#include <QDebug>

#define DIFUZ (*(src[0]))
#define SATUR (*(src[1]))
#define VIPARKA (*(src[2]))
#define APARAT (*(src[3]))
#define CENTRIF (*(src[4]))

#define GASPICH (*(src[5]))

#define ROU (*(src[6]))


SParamDialog::SParamDialog(AuBase &source,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SParamDialog),
    src(source)
{
    m_ui->setupUi(this);
    //AuPlcSetup *auSetup=
    new AuPlcSetup(QStringLiteral(":/plc.list"),m_ui->tabPlc); //а просто постворювати, нікуди не записуючи результат, щоб не матюкалося

    // AuTagSetup *auTs=
    new AuTagSetup(src,m_ui->tabTag);

    QSettings s;
    m_ui->writeRep->setChecked(s.value("FabReportWriter",0).toInt());
}

SParamDialog::~SParamDialog()
{
    QSettings s;
    if((m_ui->writeRep->isChecked()?1:0)!=s.value("FabReportWriter"))
        QMessageBox::information(this,"Попередження","Увага. Необхідно перезапустити програму");
    s.setValue("FabReportWriter",m_ui->writeRep->isChecked()?1:0);

    delete m_ui;
}


