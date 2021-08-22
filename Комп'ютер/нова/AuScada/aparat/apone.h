#ifndef APONE_H
#define APONE_H

#include <QLabel>
#include <QVector>

class QCheckBox;
class QLCDNumber;
class AuPlc;


namespace Ui {
    class ApOne;
}

class ApOne : public QLabel {
    Q_OBJECT
public:
    ApOne(AuPlc &source, QWidget *parent = 0);
    ~ApOne();

public slots:
    void updateData();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ApOne *ui;
    AuPlc &src;

    QVector<QCheckBox*> cb;
    QVector<QLCDNumber*> lcd;

};

#endif // APONE_H
