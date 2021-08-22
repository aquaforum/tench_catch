#ifndef APCOMMON_H
#define APCOMMON_H

#include <QWidget>
#include "tmaaparat/bitmask.h"

class AuBase;
class ApOne;
class QLineEdit;
class QProgressBar;
class QPushButton;
class AuLamp;

namespace Ui {
class ApCommon;
}

class ApCommon : public QWidget
{
    Q_OBJECT

public:
    explicit ApCommon(AuBase &base,QWidget *parent = nullptr);
    ~ApCommon();


protected:
    void paintEvent(QPaintEvent *);
private slots:
    void slotUpdate();
    void slotCallReg();
    void slotSgKl();

    void blink();

signals:
    void blinkOff(bool);

private:
    Ui::ApCommon *ui;
    AuBase &src;
    BitMask &srcBM;

    QVector<ApOne*> ap1;

    QList<QLineEdit*> le;
    QList<AuLamp*> lamp;
    QList<QProgressBar*> pb;

    QHash<QString,QStringList> sgKlObList,sgKlApList;

    QVector<AuLamp*> blinkLamps;
    QList<QPushButton*> bnLamp;

    bool myOff;
    quint32 count;



};

#endif // APCOMMON_H
