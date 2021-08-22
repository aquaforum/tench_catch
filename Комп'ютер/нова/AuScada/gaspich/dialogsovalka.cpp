#include "dialogsovalka.h"
#include "ui_dialogsovalka.h"

#include "aulib.h"

DialogSovalka::DialogSovalka(AuPlc &source, QWidget *parent) :
    QDialog(parent),src(source),
    ui(new Ui::DialogSovalka)
{
    ui->setupUi(this);

    //ui->AM0003->setCurrentIndex(src.getValue16("AM0003")==0?0:1);
    //ui->zdTupload->setValue(src.getValue32("zdTupload")/30000.0);

    connect(ui->bnClose,SIGNAL(clicked()),this,SLOT(myAccept()));
}

DialogSovalka::~DialogSovalka()
{
    delete ui;
}


void DialogSovalka::myAccept()
{
    //src.sendValue("AM0003",qint16(-ui->AM0003->currentIndex()));
    //src.sendValue("zdTupload",qint32(ui->zdTupload->value()*30000.0));

    accept();
}
