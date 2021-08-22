#ifndef APONE_H
#define APONE_H

#include <QWidget>

namespace Ui {
class ApOne;
}

class AuPlc;
class QLabel;


class ApOne : public QWidget
{
    Q_OBJECT

public:
    explicit ApOne(AuPlc &plc,QWidget *parent = nullptr);
    ~ApOne();
    void setText(QString);
protected:
    void paintEvent(QPaintEvent *);

private slots:
    void slotUpdate();

public slots:
    void blink(bool off);

private:
    Ui::ApOne *ui;
    AuPlc &src;
    QList<QLabel*> lcdS,le2;


    QStringList kodState;
    QStringList kodColor;
};

#endif // APONE_H
