#include "aulamp.h"
#include <QPainter>
#include <QColor>

AuLamp::AuLamp(QWidget *parent) : QWidget(parent)
  ,myIndex(0)
{
    QPixmap px(1,1);
    px.fill(QColor(0,0,0,0));

    myImage << px << px; // мінімум дві картинки
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

int AuLamp::lamp()
{
    return myIndex;
}

void AuLamp::setLamp(int lamp)
{
    bool r=myIndex!=lamp;
    myIndex=lamp>=myImage.size()?myImage.size()-1:lamp;
    if(r)
        repaint();

}

void AuLamp::paintEvent(QPaintEvent *)
{
    QPainter painter;
    QRect myG=geometry();
    myG.setSize(myImage[myIndex].size());
    setGeometry(myG);

    painter.begin(this);
    painter.eraseRect(myG);
    painter.drawPixmap(0,0,myImage[myIndex]);
    painter.end();

}

void AuLamp::resizeEvent(QResizeEvent *)
{

}

bool AuLamp::checked()
{
    return myIndex!=0;

}
void AuLamp::setChecked(bool index)
{
    bool rep= myIndex!=(index?1:0);

    myIndex=index?1:0;
    if(rep) repaint();
}

QPixmap AuLamp::AuLamp::lampOff()
{
    return myImage[0];

}

void AuLamp::setLampOff(QPixmap lamp)
{
        myImage[0]=lamp;
        repaint();
}

QPixmap AuLamp::lampOn()
{
    return  myImage[1];
}

void AuLamp::setLampOn(QPixmap lamp)
{
    myImage[1]=lamp;
    repaint();
}

QSize AuLamp::sizeHint() const
{
    QSize ret;
    ret=myImage[0].size().width()*myImage[0].size().height()>myImage[1].size().width()*myImage[1].size().height()?myImage[0].size():myImage[1].size();
    return ret;
}

void AuLamp::addLamp(QPixmap lamp)
{
    myImage << lamp;
}

