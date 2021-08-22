#ifndef BIGPUMPSMON_H
#define BIGPUMPSMON_H

#include <QDialog>
#include <QUdpSocket>

class AuBase;

namespace Ui {
class BigPumpsMon;
}

class BigPumpsMon : public QDialog
{
    Q_OBJECT

public:
    explicit BigPumpsMon(AuBase &source, QWidget *parent = 0);
    ~BigPumpsMon();

public slots:
    void slotUpdate(); // будемо слухати раптом хто пришле дані

private:
    Ui::BigPumpsMon *ui;
    AuBase &src;

    quint32 count;

    quint16 bpIx;

};

#endif // BIGPUMPSMON_H
