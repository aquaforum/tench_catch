#include "fabmnemo.h"
#include "mnemo.h"
#include <QSettings>
#include <QScrollBar>

fabMnemo::fabMnemo(AuBase &base, QWidget *parent) : QScrollArea(parent)
{
    setFrameShape(QFrame::NoFrame);
    myMnemo=new Mnemo(base);
    setWidget(myMnemo);

    connect(horizontalScrollBar(),&QScrollBar::valueChanged,this,&fabMnemo::slotStorePos);

}

fabMnemo::~fabMnemo()
{
    myMnemo->deleteLater();
}

void fabMnemo::slotStorePos(int)
{
    QSettings s;
    s.setValue("posMnemo",this->horizontalScrollBar()->value());

}
