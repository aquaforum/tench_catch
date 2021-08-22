#ifndef __ONE_MNEMO1_H__
#define __ONE_MNEMO1_H__

#include <QWidget>
#include <QVector>

class TrendChart;
class QCheckBox;
class QLineEdit;
class AuBase;
class AuPlc;

namespace Ui {
    class one_mnemo1;
}

class OneMnemo1: public QWidget

{
    Q_OBJECT
public:
    OneMnemo1(AuBase &source,   int cfNom=1, QWidget *p=NULL);
    ~OneMnemo1();

    inline int currentCfNom(){ return nCf;}

public slots:
    void updateData(); // слот обновляє дані на мнемосхемі
    void updateTrChart(); // поновлюємо дані на графічках
    void changeCf();
    void sendValue(int);
    void sendValue(double);
    void sendValue(bool);
    void sendRejSgr(bool);

private:
    Ui::one_mnemo1 *m_ui;
    int nCf;
    AuBase &src;
    // масиви вказівників на елементи управління
    TrendChart* trcf;
    // QVector<TrendChart*> &mTrChart;

    QVector<QCheckBox*>  cb; // тут будуть дискретні індикатори
    QList<QCheckBox*> cb_state; // це лампочки на панельці

    QStringList cb_name; // імена тегів для вивантаження

    QVector<QVector<QLineEdit*> > le; // тут будуть рядки

    QStringList State; // список станів

    int lastStatus;
    quint16 cfIx[3];

};

#endif
