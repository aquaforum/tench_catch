#include "dlgflokctl.h"
#include "ui_dlgflokctl.h"
#include <aulib.h>


DlgFlokCtl::DlgFlokCtl(AuPlc &plc, QStringList tagList,QWidget *parent) :
    QDialog(parent),
    src(plc),
    tags(tagList),
    ui(new Ui::DlgFlokCtl)
{
    ui->setupUi(this);

    connect(ui->bnClose,&QPushButton::clicked,this,&QDialog::accept);

    ui->pump->setCurrentIndex(src[tags[5]]==0?0:1);


    int mode=src[tags[2]].value()!=0?1:0;
    ui->mode->setCurrentIndex(mode);
    connect(ui->mode,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&DlgFlokCtl::setAM);

    ui->xC->setValue(src[tags[1]]);
    connect(ui->xC,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&DlgFlokCtl::setCtrl);
    ui->xC->blockSignals(mode);

    ui->slC->setValue(src[tags[1]].valueReal());
    connect(ui->slC,&QSlider::valueChanged,this,&DlgFlokCtl::setSlC);
    ui->slC->blockSignals(mode);
    ui->pump->blockSignals(mode);

    ui->k->setValue(src[tags[3]]);
    connect(ui->k,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&DlgFlokCtl::setKC);

    connect(ui->pump,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&DlgFlokCtl::setPump);


    connect(&plc,&AuPlc::updateData,this,&DlgFlokCtl::updateData);

    setWindowTitle(tags[4]);


}

DlgFlokCtl::~DlgFlokCtl()
{
    delete ui;
}
void DlgFlokCtl::updateData()
{
    ui->V->setText(src[tags[0]]);
    ui->pbV->setValue(src[tags[0]].valueReal());
    if(ui->mode->currentIndex()!=0)
    {
        ui->xC->setValue(src[tags[1]]);
        ui->slC->setValue(src[tags[1]].valueReal());
        ui->pump->setCurrentIndex(src[tags[5]]==0?0:1);
    }
    ui->pvPump->setValue(ui->slC->value());
    ui->Ffl->setText(QString::number(src[tags[1]].valueReal()*0.184375,'f',0));

}

void DlgFlokCtl::setAM(int v)
{
    src[tags[2]]=qint16(-v);
    ui->xC->blockSignals(v);
    ui->slC->blockSignals(v);
    ui->pump->blockSignals(v);

}

void DlgFlokCtl::setCtrl(double v)
{
    src[tags[1]]=v;
    ui->slC->blockSignals(true);
    ui->slC->setValue(src[tags[1]].valueReal());
    ui->slC->blockSignals(false);


}

void DlgFlokCtl::setSlC(int v)
{
    src[tags[1]].setValue(v);
    ui->xC->blockSignals(true);
    ui->xC->setValue(src[tags[1]]);
    ui->xC->blockSignals(true);

}

void DlgFlokCtl::setKC(double v)
{
    src[tags[3]].setValue(v);
}

void DlgFlokCtl::setPump(int v)
{
    src[tags[5]]=qint16(-v);
    //qDebug() << tags[5] << v;

}
