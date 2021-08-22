#ifndef __CFMNEMO_H__
#define __CFMNEMO_H__

#include <QWidget>
#include <QVector>
#include <QList>

class TrendChart;
class IoNetClient;
class QCheckBox;
class QLineEdit;
class QProgressBar;


class CfMnemo: public QWidget
{
    Q_OBJECT
public:
    CfMnemo(IoNetClient &src, QWidget *prn=NULL);
    ~CfMnemo();
    QVector<TrendChart*> &getTrChart() {return trcf;}

public slots:
    void updateDataRaw(int); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrChart(); // поновлюємо дані на графічках
    void slotSetnQ(double);
    void slotSetnTnt(int);
    void slotSetnTsgr(int);
    void updatenCor();


private:
    QWidget *ui;
    IoNetClient &s;
    // масиви вказівників на елементи управління
    QVector<TrendChart*> trcf;

    QList<QCheckBox*> cb; // тут будуть дискретні індикатори

    QList<QLineEdit*> le,leState; // тут будуть рядки

    QStringList State; // список станів

    // це відноситься до загальних параметрів
    QList<QLineEdit*> le_cm;
    QList<QProgressBar*> pb_cm;
    QVector<QCheckBox*> cb_cm;

    // далі пішли одиночні центрифуги
    QVector<QList<QLineEdit*> > le_co;
    QVector<QList<QCheckBox*> > cb_co;
    QVector<QLineEdit*> State_co;


};

#endif
