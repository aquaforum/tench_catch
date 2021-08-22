#ifndef AUTRENDPLOT_H
#define AUTRENDPLOT_H

#include <QWidget>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QObject>

// Клас малювання самого графіка
class TrendPlot: public QWidget
{
    Q_OBJECT
public:
    TrendPlot(QWidget *p=nullptr);

    ~TrendPlot();

    inline QSize sizeHint() const {return QSize(400,300);}
    inline QSizePolicy sizePolicy() const {return QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);}

    void setColors(QVector<QColor> &pColor);
    bool grid() {return m_grid;}

public slots:
    void start(int nLen,int nHeight=4000); // тут створ.ється новий oб'єкт m_px
    void setData(int start, int *, int row, int col, int colorIx, QVector<double> wK); // цей слот приймає дані для малювання.
    void draw(); // власне по цій команді об'єкт m_px відображається на екрані
    void setGrid(bool v) {m_grid=v;}

    void clear();

signals:
    void _redraw();
    void cursorMoved(int pos);

protected:
    QPen m_pen; // карандаш для ма
    QPainter m_paint; // об'єкт за допомогю якого буду малювати

    QPixmap *m_px; // картинка, на якій малюються графіки
    int m_nPlot;
    // bool m_bStart;
    bool m_grid;

    int myHeight;

    QVector<QColor> m_pColor;


    void paintEvent(QPaintEvent *e); // малюю вищенаведену картину на екрані.
    void resizeEvent(QResizeEvent *e); // подія зміни розміру
    // обробники курсоку
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    int m_nCursorPos;
    int ixC ; // індекс кольору

};
#endif // AUTRENDPLOT_H
