#ifndef BIGPUMPSMON_H
#define BIGPUMPSMON_H

#include <QDialog>
#include <QUdpSocket>


namespace Ui {
class BigPumpsMon;
}

class BigPumpsMon : public QDialog
{
    Q_OBJECT

public:
    explicit BigPumpsMon(quint16 crcIx, QWidget *parent = 0);
    ~BigPumpsMon();

public slots:
    void slotUpdate(quint16); // будемо слухати раптом хто пришле дані

private:
    Ui::BigPumpsMon *ui;
    quint16 index; // тут буде збережено crcIx ідентифікатор для перевірки джерела даних

    quint32 count;

};

#endif // BIGPUMPSMON_H
