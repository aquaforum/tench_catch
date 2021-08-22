#ifndef __APMNEMO_H__
#define __APMNEMO_H__

#include <QLabel>
#include <QVector>

class TrendChart;
class IoNetClient;
class QCheckBox;
class QLineEdit;
class QStringList;
class ApOne;
class QProgressBar;

namespace Ui {
    class aPmnemo;
}

class ApMnemo: public QLabel
{
    Q_OBJECT
public:
    ApMnemo(IoNetClient &src, QWidget *p=NULL);
    ~ApMnemo();

public slots:
    void updateDataRaw(); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrend();
    void slotCallDlgApOne();
    void slotCallTermCtrl();
    void slotAlarm();

signals:
    void signalAlarm();

private:
    Ui::aPmnemo *m_ui;
    IoNetClient &s;

    QVector<ApOne*> ap;
    QVector<TrendChart*> trc;


    QStringList state;
    bool alm;

    QVector<QLineEdit*> leOB;
    QVector<QProgressBar*> pbOB;
};

#endif
