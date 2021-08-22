#include "mnemo_gp.h"
#include "ui_gpmnemo.h"

#include <QStyleOption>
#include <QPainter>

mnemo_gp::mnemo_gp(QWidget *parent) : QWidget(parent)
  ,ui(new Ui::mnemo_gp)
{
    ui->setupUi(this);


}

void mnemo_gp::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
