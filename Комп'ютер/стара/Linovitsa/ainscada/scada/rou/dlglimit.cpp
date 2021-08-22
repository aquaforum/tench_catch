#include "dlglimit.h"
#include "ui_dlglimit.h"

#include <iodev.h>

#include <QFile>
#include <QDateTime>

DlgLimit::DlgLimit(IoDev &source, QWidget *parent) :
    QDialog(parent),
    src(source),
    ui(new Ui::DlgLimit)
{
    ui->setupUi(this);

    foreach (QDoubleSpinBox *p, findChildren<QDoubleSpinBox*>()) {
       p->setValue(src.getValueScaled(p->objectName()));
       oldVal[p->objectName()]=p->value();
    }

    connect(ui->bnClose,SIGNAL(clicked()),this,SLOT(myAccept()));


}

DlgLimit::~DlgLimit()
{
    delete ui;
}


void DlgLimit::myAccept()
{
    QFile f("rouparam.log");
    f.open(QIODevice::Append);

    foreach (QDoubleSpinBox *p, findChildren<QDoubleSpinBox*>()) {
        if(p->value()!=oldVal[p->objectName()])
        {
            QString log("%1\t%2\t%3 -> %4\n");
            f.write(
                    log.arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
                    .arg(p->objectName())
                    .arg(oldVal[p->objectName()],0,'f',2)
                    .arg(p->value(),0,'f',2).toUtf8()) ;

            src.sendValueScaled(p->objectName(),p->value());

        }
    }

    f.close();
    accept();

}

