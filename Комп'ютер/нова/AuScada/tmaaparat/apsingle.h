#ifndef APSINGLE_H
#define APSINGLE_H

#include <QWidget>
#include "bitmask.h"

class AuLamp;
class QLineEdit;
class QProgressBar;
class QPushButton;
//class QLCDNumber;
class TrendChart;
class QLabel;

namespace Ui {
class ApSingle;
}

class AuPlc;
class AuBase;

class ApSingle : public QWidget
{
    Q_OBJECT

public:
    explicit ApSingle(AuPlc &plc, AuBase &base, QWidget *parent = nullptr);
    ~ApSingle();
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void slotUpdate();
    void slotUpdateOb();

    void slotSgKl();
    void slotCallCurkyl();
    void slotCallReg();

    void slotControl();
    void blink();

    void updateTrend();
    void slotHtrData(qint32 iD, QByteArray baData, int row, int col);
    void sqlError(QString);

private:
    Ui::ApSingle *ui;
    AuPlc &src;
    AuBase &myBase;
    AuPlc &srcOB;

    BitMask &srcBM;
    QList<AuLamp*> lamp;

    QList<AuLamp*> blinkLamp;

    // QList<QLineEdit*>
    QHash<QString,QStringList> sgKlObList,sgKlApList;

    QList<QPushButton*> bnLamp;
    QList<QLineEdit*> obLe,apLe;
    // QList<QLCDNumber*> apLcd;
    QList<QLabel*> lcdS,le2;

    QList<QPushButton*> bnReg;

    QStringList kodState;
    QStringList kodColor;

    TrendChart *trc;


};

#endif // APSINGLE_H
