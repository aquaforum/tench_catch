#ifndef __ONE_MNEMO1_H__
#define __ONE_MNEMO1_H__

#include <QWidget>
#include <QVector>

class TrendChart;
class QCheckBox;
class QLineEdit;
class IoNetClient;
class IoDev;

namespace Ui {
    class one_mnemo1;
}

class OneMnemo1: public QWidget

{
    Q_OBJECT
public:
    OneMnemo1(IoNetClient &src, QVector<TrendChart*> &masterTrChart,  int cfNom=1, QWidget *p=NULL);
    ~OneMnemo1();

    inline int currentCfNom(){ return nCf;}

public slots:
    void updateDataRaw(int); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrChart(); // поновлюємо дані на графічках
    void changeCf();
    void sendValue(int);
    void sendValue(double);
    void sendValue(bool);
    void sendRejSgr(bool);

private:
    Ui::one_mnemo1 *m_ui;
    int nCf;
    IoNetClient &s;
    // масиви вказівників на елементи управління
    TrendChart* trcf;
    QVector<TrendChart*> &mTrChart;

    QVector<QCheckBox*>  cb; // тут будуть дискретні індикатори
    QList<QCheckBox*> cb_state; // це лампочки на панельці

    QStringList cb_name; // імена тегів для вивантаження

    QVector<QVector<QLineEdit*> > le; // тут будуть рядки

    QStringList State; // список станів

    int lastStatus;

};

#endif
