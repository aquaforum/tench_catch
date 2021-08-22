

#include "AuTrendPlot.h"
#include <QMessageBox>
#include <QResizeEvent>
#include <QDebug>
#include <QPainterPath>
#include <QObject>

// Функції-члени класу TrendPlot
TrendPlot::TrendPlot(QWidget *p) : QWidget(p),
    m_grid(true),
    myHeight(4000)

{
    setObjectName("TrendPlot_1");
    m_px=new QPixmap(2000,2000);
    m_px->fill(QColor(Qt::black));


    m_paint.begin(m_px);


    repaint();
    m_nPlot=0; // на всяк пожарний
    m_pColor.fill(QColor(Qt::black),8);

    ixC=0; // теж на всякий пожарний

}

TrendPlot::~TrendPlot()
{
    m_paint.end();
    delete m_px;
}

void TrendPlot::clear()
{

}

void TrendPlot::start(int nLen, int nHeight /*=4000*/) // тут створ.ється новий oб'єкт m_px
{

    if(m_paint.isActive())
        m_paint.end();


    if(m_px)
        delete m_px; // знищити старий малюнок
    ixC=0;

    m_px = new QPixmap(size());  // створити новий малюнок
    if(m_px)
    {

    m_paint.begin(m_px);
    m_paint.setRenderHint(QPainter::Qt4CompatiblePainting	,true);
    m_paint.scale((double)m_px->width()/(double)nLen,(double)m_px->height()/(double)nHeight);
    //m_pen.setWidthF((double)nLen/(double)m_px->width()+(double)nHeight/(double)m_px->height());


        m_px->fill(Qt::black);     // замалювати фон

        // намалювати сітку якщо увімкнута
        if(m_grid)
        {
            QPen m_tPen;
            m_tPen.setColor(Qt::darkGray);
            m_tPen.setWidth(0);

            QVector<qreal> dashes;
            dashes << 1.0 << 2.0
                   << 1.0 << 2.0
                   << 1.0 << 2.0
                   << 1.0 << 5.0 ;

            m_tPen.setDashPattern(dashes);

            m_paint.setPen(m_tPen);
            int nStep=nHeight/4;
            for(int i=1;i<4;++i)
            {
                m_paint.drawLine(0,nStep*i,nLen,nStep*i);
                // qDebug() << i <<nHeight/i;
            }

            nStep=nLen/4;
            for(int i=1;i<4;++i)
            {
                m_paint.drawLine(nStep*i,0,nStep*i,nHeight);
                // qDebug() << i <<nHeight/i;
            }


        }
        // m_bStart=true;

        myHeight=nHeight; // зберегти висоту графіку
    }
    else
    {
    QMessageBox::critical(this,tr("Помилка"),tr("Не вдалося виділити пам\'ять під об\'єкт QPixmap"));
    close();
    }
}

void TrendPlot::setData(int start, int *data, int row,int col,int colorIx,QVector<double> wK) // цей слот приймає дані для малювання.
{

    if(row==0)
        return;

    for(int i=1;i<col;++i) // пройтися по кривих
    {
        // qDebug() << m_pColor.size() << ixC << data[0].size();
        m_pen.setColor(m_pColor[colorIx++]);
        m_paint.setPen(m_pen);
        QPainterPath path;

        path.moveTo(data[0*col+0]-start,myHeight-(data[0*col+i]*wK[i-1]));
        for(int j=1;j<row;++j)
            path.lineTo(data[j*col+0]-start,myHeight-(data[j*col+i]*wK[i-1]));
            //m_paint.drawLine(data[j-1][0]-start,myHeight-data[j-1][i], data[j][0]-start  ,myHeight-data[j][i]); // намалювати поточний відрізок
        m_paint.drawPath(path);

    }
    update();

}

void TrendPlot::draw() // власне по цій команді об'єкт m_px відображається на екрані
{
    repaint(); // а можливо це непотрібно ?
}

void TrendPlot::setColors(QVector<QColor> &pColor)
{
    m_pColor=pColor;
}

void TrendPlot::paintEvent(QPaintEvent *) // малюю вищенаведену картину на екрані.
{
    QPainter pnt(this);
    QPen p((QColor(Qt::yellow)));

    pnt.drawPixmap(0,0,*m_px);

    pnt.setPen(p);
    pnt.drawLine(m_nCursorPos,0,m_nCursorPos,height()); // намалювати курсор

}

/* схоче що це поки-що непортрібно, управлінням розміром об'єкту керує менеджер розміщення */
void TrendPlot::resizeEvent(QResizeEvent *e)
{
//    resize(e->size());
    if(e->size()!=e->oldSize()) // якщо розміри змінились, сказати що треба перемалюватися
        emit _redraw();
}

void TrendPlot::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons()&Qt::LeftButton)
    {
        m_nCursorPos=e->x();
    emit cursorMoved(m_nCursorPos);
        repaint();
    }
}

void TrendPlot::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        m_nCursorPos=e->x();
    emit cursorMoved(m_nCursorPos);
        repaint();
    }
}

