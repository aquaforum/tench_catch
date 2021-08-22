#include "dlgerrlogview.h"
#include "ui_dlgerrlogview.h"
#include <QDir>
#include <QFile>



DlgErrLogView::DlgErrLogView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgErrLogView)
{
    ui->setupUi(this);

    QFile errorLog(QDir::homePath()+"/gperror.log");
    if(errorLog.open(QIODevice::ReadOnly))
    {
        ui->errLogView->append(QString::fromUtf8(errorLog.readAll()));
        errorLog.close();
    }

}

DlgErrLogView::~DlgErrLogView()
{
    delete ui;
}
