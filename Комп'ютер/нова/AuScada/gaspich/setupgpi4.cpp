#include "setupgpi4.h"
#include "ui_setupgpi4.h"
#include <aulib.h>

#include <QDateEdit>

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMessageBox>

SetupGpi4::SetupGpi4(AuPlc &source,QWidget *parent) :
    QDialog(parent),
    src(source),bAccess(true),
    ui(new Ui::SetupGpi4)
{
    ui->setupUi(this);



    ui->zdGcl->setValue(src[QStringLiteral("zdGcl")]);
    ui->zdGst->setValue(src[QStringLiteral("zdGst")]);
    ui->pGclst->setValue(ui->zdGcl->value()/(ui->zdGst->value()-ui->zdGcl->value())*100.0);

    ui->zdTProbeWait->setValue(src[QStringLiteral("zdTProbeWait")]/60.0);

    ui->nomLoad->setValue(src[QStringLiteral("nomLoad")]);

    connect(ui->zdGcl,SIGNAL(valueChanged(double)),this,SLOT(slotChangeMass(double)));
    connect(ui->zdGst,SIGNAL(valueChanged(double)),this,SLOT(slotChangeMass(double)));
    connect(ui->pGclst,SIGNAL(valueChanged(double)),this,SLOT(slotChangeMass(double)));

    int zdMode = src[QStringLiteral("zdMode")];

    switch(zdMode)
    {
        case 1:
              ui->zdMode->setCurrentIndex(2);
              break;
        case 2:
            ui->zdMode->setCurrentIndex(3);
            break;
        default:
            ui->zdMode->setCurrentIndex(0);
    }
    // синхронізація годинника контролера, це би зробити автоматично......
    QVector<qint16> time;
    QDateTime ct=QDateTime::currentDateTime();
    time << ct.date().year() << ct.date().month() << ct.date().day() << ct.time().hour() << ct.time().minute() << ct.time().second() << -1;
    src[QStringLiteral("yy_s")].setValues(time);

    connect(ui->Close,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->bnOk,SIGNAL(clicked()),this,SLOT(slotAccept()));

/*
#ifndef WIN32
    // перевірка наявності файлу розмежування доступу
    QFile fAccess("/usr/etc/ac.list");
    if(fAccess.exists())
    {
        bAccess=false;
        QDir dlist("/dev/disk/by-uuid");
        QStringList list=dlist.entryList();
        list.removeAt(0); // видалити два перших елементи
        list.removeAt(0);
        fAccess.open(QIODevice::ReadOnly);
        for(;!fAccess.atEnd();)
        {
            QString key=fAccess.readLine().trimmed();
            if(list.contains(key)) bAccess=true;
        }
        if(bAccess)
        {
            QMessageBox::information(parent,tr("Повідомлення"),tr("Ключ доступу знайдено.\nЗміну параметрів дозволено"));
        }

    }
#endif
*/

}

SetupGpi4::~SetupGpi4()
{
    delete ui;
}

void SetupGpi4::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SetupGpi4::slotAccept()
{
/*
    qDebug() << "bAccess" << bAccess;

    if(bAccess)
    {
    */
        src[QStringLiteral("zdGcl")]=ui->zdGcl->value();
        src[QStringLiteral("zdGst")]=ui->zdGst->value();

        src[QStringLiteral("zdTProbeWait")]=qint32(ui->zdTProbeWait->value()*60.0);
        src[QStringLiteral("nomLoad")]=qint16(ui->nomLoad->value());

        double kW=(1.0-(100.0/(100.0+ui->pGclst->value())))*(
                    src[QStringLiteral("V0002")].full()-
                    src[QStringLiteral("V0002")].zero())/(
                    src[QStringLiteral("V0001")].full()-
                    src[QStringLiteral("V0001")].zero());

        src[QStringLiteral("kW")]=kW;


    switch (ui->zdMode->currentIndex()) {

    case 0:
        src[QStringLiteral("zdMode")]=qint16(0);
        break;
    case 1:
        src[QStringLiteral("zdMode")]=qint16(0);
        src[QStringLiteral("oneShot")]=qint16(-1);
        break;
    case 2:
        src[QStringLiteral("zdMode")]=qint16(1);
        break;
    case 3:
        src[QStringLiteral("zdMode")]=qint16(2);
        break;
    default:
        break;
    }

    accept();


}

void SetupGpi4::slotChangeMass(double v)
{
    QString s=sender()->objectName();
    if(s=="zdGcl")
    {
        ui->pGclst->blockSignals(true);
        ui->pGclst->setValue(v/(ui->zdGst->value()-v)*100.0);
        ui->pGclst->blockSignals(false);
    }
    else if(s=="zdGst")
    {
        ui->zdGcl->blockSignals(true);
        ui->zdGcl->setValue( v*(ui->pGclst->value()/100.0)/(1.0+(ui->pGclst->value()/100.0)) );
        ui->zdGcl->blockSignals(false);

    }
    else if (s=="pGclst")
    {

        ui->zdGcl->blockSignals(true);
        //ui->zdGcl->setValue(v*ui->zdGst->value()/100.0);
        ui->zdGcl->setValue( ui->zdGst->value()*(v/100.0)/(1.0+(v/100.0))  );
        ui->zdGcl->blockSignals(false);
    }
}


