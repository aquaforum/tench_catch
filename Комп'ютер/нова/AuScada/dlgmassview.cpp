#include "dlgmassview.h"
#include "ui_dlgmassview.h"
#include <aulib.h>
#include <QTimer>
#include <QLineEdit>
#include <QRegExp>

DlgMassView::DlgMassView(AuPlc &source, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMassView),src(source)
{
    ui->setupUi(this);

    foreach (QLineEdit *p, this->findChildren<QLineEdit*>(QRegExp("^d1_"))) {
        leMass << p;
    }

    mass1=this->findChild<QLineEdit*>("sum_Mass1");
    mass2=this->findChild<QLineEdit*>("sum_Mass2");
    massSum=this->findChild<QLineEdit*>("sum_Mass");

    leSum << ui->sum_01
          << ui->sum_02
             << ui->sum_03
                << ui->sum_04
                   << ui->sum_05
                      << ui->sum_06
                         << ui->sum_07
                            << ui->sum_08
                               << ui->sum_09
                                  << ui->sum_10
                                     << ui->sum_11
                                        << ui->sum_12;
    // синхронізація годинника контролера, це би зробити автоматично......
    QVector<qint16> time;
    QDateTime ct=QDateTime::currentDateTime();
    time << ct.date().year() << ct.date().month() << ct.date().day() << ct.time().hour() << ct.time().minute() << ct.time().second() << -1;
    src[QStringLiteral("yy_s")].setValues(time);

    //updateData();
    connect(&source,&AuPlc::updateData,this,&DlgMassView::updateData);


}

DlgMassView::~DlgMassView()
{
    delete ui;
}

void DlgMassView::updateData()
{
    //AuPlc &s=*qobject_cast<AuPlc*>(sender());
    QString tag;

    double m1,m2;
    foreach (QLineEdit *p, leMass) {
        tag=p->objectName().split('_').at(1);
        p->setText(QString::number(src[tag]/100.0,'f',2));
    }
    double massW1=0,massW2=0;
    for(int i=1;i<13;++i)
    {
        tag=QString("V00%1").arg(i,2,10,QChar('0'));
        m1=src[tag];
        tag=QString("V00%1").arg(i+24,2,10,QChar('0'));
        m2=src[tag];
        massW1+=m1;
        massW2+=m2;
        leSum[i-1]->setText(QString("%1").arg((m1+m2)/100.0,0,'f',2));
    }

    mass1->setText(QString::number(massW1/100.0,'f',2));
    mass2->setText(QString::number(massW2/100.0,'f',2));
    massSum->setText(QString::number((massW1+massW2)/100.0,'f',2));

}

