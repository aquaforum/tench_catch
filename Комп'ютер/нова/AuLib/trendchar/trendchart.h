#ifndef TREND_H
#define TREND_H

#include <QtCore>
#include <QWidget>
#include <QTime>
#include <QSize>
#include <QSizePolicy>
#include <QPoint>


class QVBoxLayout;

class TrendChart : public QWidget
{
public:
//    TrendChart(QWidget *parent); // конструктор
    TrendChart(QWidget *parent,int timeLen=3600,int Interval=5,double base=4000.0); // конструктор
    TrendChart(QWidget *parent,TrendChart *p); // якщо клас створюється на основі іншого класу то від буде працювати як слейв

    virtual ~TrendChart(); // деструктор

    inline QSize sizeHint() const {return QSize(400,300);}
    inline QSizePolicy sizePolicy() const {return QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);}
    
    void addPoint(QVector<double>& Val);

    void loadPoint(QVector<double>& Val);

    void loadData(QByteArray &data, int row, int col);

    void setColor(QVector<QColor>& v)
    {
	clr=v;
    }

    QColor color(int i) {return clr[i];}

    void fill(double v);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *e);
private:
    int nLen,nInterval;
    QVector<QColor> clr;
    //QPixmap *pGraph; // це буде картинка, на якій будуть намальовані графіки

    int *pY; // тут зберігаються дані
    int cP; // кількість точок
    double xS,yS; // коефіцієнти шкалювання

    bool slave; // прапор, виставляється конструкторами класу
    TrendChart *master;
    QVBoxLayout *vbl;

    double myBase;

};


#endif
