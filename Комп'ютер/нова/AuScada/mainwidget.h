#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QGLWidget>

class QVBoxLayout;
class AuBase;
class AuTrendView;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(AuBase &base,QWidget *parent = nullptr);

public slots:
    void slotShowMain();
    void slotShowAparat();

    void slotShowTrend(AuTrendView*);
    void slotRetTrend();

protected:
    void closeEvent(QCloseEvent *event);

signals:

private:
    AuBase &src;
    QVBoxLayout *vbl;

    QWidget *currentWidget;
};

#endif // MAINWIDGET_H
