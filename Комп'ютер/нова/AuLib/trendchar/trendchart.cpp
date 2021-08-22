#include <QPainter>
#include <QRect>
#include <QPen>
#include <QColor>
#include <QString>
#include <QtDebug>
#include <QSize>
#include <QResizeEvent>

#include "trendchart.h"
#include <math.h>

#include <QVBoxLayout>

#include <iostream>

using namespace std;

TrendChart::TrendChart(QWidget *parent,int timeLen,int Interval,double base):QWidget(parent),nLen(timeLen),nInterval(Interval),cP(0),slave(false),vbl(NULL),myBase(base)
{
//    int i,n,ofs;
//    int h2;
    int i;
    
    resize(parent->width(),parent->height());

    xS=((double)size().width())/(double)nLen; // 3600.0;
    yS=((double)size().height())/myBase;


    cP=nLen/nInterval;
    //qDebug() << 8*cP;
    pY=new int[8*cP]; // виділити пам’ять для точок
    for(i=0;i<8*cP;++i)
    pY[i]=myBase;
    clr << QColor(255,24,237) << QColor(66,168,255)<< QColor(0,186,0)<< QColor(100,255,0)<< QColor(Qt::yellow)<< QColor(90,0,113)<< QColor(0,9,137)<< QColor(0,89,0);
}

TrendChart::TrendChart(QWidget *parent,TrendChart *p):QWidget(parent),slave(true),master(p),vbl(new QVBoxLayout(parent)) // конструктор класу-слейва
{

    resize(parent->width(),parent->height());

    xS=((double)size().width())/(double)nLen;
    yS=((double)size().height())/myBase;

    nLen=master->nLen;
    nInterval=master->nInterval;
    cP=master->cP;
    pY=master->pY; // дані мастера
    clr=master->clr;

    vbl->addWidget(this);
    parent->setLayout(vbl);
}

TrendChart::~TrendChart()
{    
//    qDebug() << "Mem clean";
//  delete pGraph;
    if(vbl) delete vbl;
    if(!slave) delete [] pY; // якщо свої дані - тоді видалити
}



void TrendChart::paintEvent(QPaintEvent*)
{
    int j,i,x,t;

    QPainter p(this);
    QPen pen;

    p.setRenderHint(QPainter::Antialiasing,false);
#if QT_VERSION_MAJOR>4
    p.setRenderHint(QPainter::Qt4CompatiblePainting	,true);
#endif

    p.fillRect(rect(),QBrush(Qt::black));
    p.scale(xS,yS);
//    qDebug() << rect();
    t=nLen/nInterval;

    for(j=0;j<8;++j)
    {
	// намалювати одну криву
	pen.setColor(clr[j]);
	p.setPen(pen);

	x=nLen-1; // індекс попередньої точки
        for(i=cP;i>1;--i)
	{
	    p.drawLine(x-nInterval,pY[i-1+t*j],x,pY[i+t*j]);
	    x-=nInterval;
	}
	for(i=t-1;i>cP+1;--i)
	{
	    p.drawLine(x-nInterval,pY[i-1+t*j],x,pY[i+t*j]);
	    x-=nInterval;
	}
    }

    p.end();

}


void TrendChart::resizeEvent(QResizeEvent *e)
{
    xS=((double)e->size().width())/(double)nLen ; // 3600.0; //  розразувати нові коефіцієнти масштабування
    yS=((double)e->size().height())/myBase;
    update();
}

void TrendChart::addPoint(QVector<double>& Val)
{
    loadPoint(Val);
    update();
}

void TrendChart::loadData(QByteArray &baData,int row,int col)
{
    if(baData.size()>0)
    {
        // тут перша колонка буде ігноруватися оскільки там Dt
        int i,t,j;
        qint32 max;
        // вирівнюємо по правому краю
        t=nLen/nInterval; //720
        qint32 *htrVal=(qint32*)baData.data();
        if(row>1) // якщо там хоч щочь є
        {
            max=htrVal[(row-1)*col+0];
            if(max<htrVal[(row-2)*col+0]) // фігня якась в даних!!!!
            {
                qDebug() << "htrVal FAIL max=" << max << " (row-2)=" << htrVal[(row-2)*col+0];
                return;
            }


            for(i=0;i<row;++i)
                htrVal[i*col+0]=(nLen-max+htrVal[i*col+0])/nInterval;

            for(i=0;i<8*t;++i) // очистити
                pY[i]=myBase;

            cP=t-1; // стартувти з нуля


            for(j=0;j<row;++j)
            {
                if(htrVal[j*col+0]<t)
                {
                    for(i=0;i<col-1;++i)
                    {
                    pY[t*i+htrVal[j*col+0]]=myBase-htrVal[j*col+i+1]; // записати нові
                    }

                    for(;i<8;++i) // !!!! доповнюємо графік до 8 кривих
                        pY[htrVal[j*col+0]+t*i]=myBase+1; // записати пусту криву
                }
            }

            update();
        }
        else
            fill(0.0);

    }

}

// тільки завантаження даних у буфер без відображення на графіку
void TrendChart::loadPoint(QVector<double>& Val)
{
  if(!slave)
  {
    int i,t;

    t=nLen/nInterval;

    if(!(t>++cP)) cP=0; // заворот

    for(i=0;i<Val.size();++i)
    pY[cP+t*i]=myBase-Val[i]; // записати нові

    for(;i<8;++i) // !!!! доповнюємо графік до 8 кривих
        pY[cP+t*i]=myBase+1; // записати пусту криву
  }
  else
  {
        cP=master->cP; // прочитати значення сР з мастера
  }

}

void TrendChart::fill(double v)
{
    if(!slave)
    {
        int i,cP;
        cP=nLen/nInterval;
        //qDebug() << 8*cP;
        for(i=0;i<8*cP;++i)
                pY[i]=4000-(int)v;
        update();
    }
}

