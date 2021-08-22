#ifndef __APMNEMO_H__
#define __APMNEMO_H__

#include <QLabel>
#include <QVector>

class TrendChart;
class AuBase;
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
    ApMnemo(AuBase &src, QWidget *p=NULL);
    ~ApMnemo();

public slots:
    void updateData(); // слот обновляє дані на мнемосхемі
    void updateState();

    void updateTrend();
    void slotCallDlgApOne();
    void slotCallTermCtrl();
    void slotAlarm();

signals:
    void signalAlarm();

private:
    Ui::aPmnemo *m_ui;
    AuBase &s;

    QVector<ApOne*> ap;
    QVector<TrendChart*> trc;


    QStringList state;
    bool alm;

    QVector<QLineEdit*> leOB;
    QVector<QProgressBar*> pbOB;

    quint16 apIx[4];
    QHash<QObject*,QLineEdit*> leList;

};

#endif
