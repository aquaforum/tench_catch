#ifndef ONEFILTER_H
#define ONEFILTER_H

#include <QtGui/QDialog>
#include <QTime>


namespace Ui {
    class OneFilter;
}

class AuPlc;

class OneFilter : public QDialog {
    Q_OBJECT
public:
    OneFilter(AuPlc &src,QWidget *parent = 0);
    ~OneFilter();

    void setFn(int i);

public slots:
    void updateData();
    void setAm(int v);
    void setValve();
    void setTosad(QTime);
    void setRskyd(int);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OneFilter *m_ui;
    int n;

    AuPlc &s;

};

#endif // ONEFILTER_H
