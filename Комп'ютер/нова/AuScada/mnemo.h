#ifndef __MNEMO_H__
#define __MNEMO_H__

#include <QWidget>
#include <QVector>

class AuBase;
class AuPlc;

class QCheckBox;
class QLineEdit;
class QLCDNumber;
class QCheckBox;
class QProgressBar;
class QPushButton;
class QWidget;
class TrendChart;
class QSlider;
class QUdpSocket;
class QSpinBox;
class QLabel;
class QFrame;

namespace Ui {
    class wMnemo;
}

class Mnemo: public QWidget
{
    Q_OBJECT
public:
    Mnemo(AuBase &source, QWidget *p=NULL);
    ~Mnemo();

private:
    void updatePlc(AuPlc&,QVector<QLineEdit*>&,QVector<QProgressBar*>&,QVector<QCheckBox*>&); // це поновлює загальну частину щоб не дублювати цей код

    QFrame* drawLegend(QString tags,TrendChart *trChar,AuPlc &plc);

public slots:
    void updateDifuz(); // слот обновляє дані на мнемосхемі
    void updateSatur(); // слот обновляє дані на мнемосхемі
    void updateVipar(); // слот обновляє дані на мнемосхемі
    void updateFqdrv(); // слот обновляє дані на мнемосхемі
    void updateFqMb(); // частотники підключені по Еthernetu
    void updateApOb();

    void updateTrendChart(); // поновлення графіків
    void updateToolTips();

    void slotCallReg();
    void slotCallVbCtrl();
    void slotCallPumpCtrl();
    void slotCallAmVater();
    void slotShowMassView();

    void slotSendSpPresh(int);
    void slotSpPreshSend();



private:
    QWidget *m_d,*m_s,*m_v;
    AuBase &src;

    // контролери, для доступу по індексах

    quint16 difuz; // дифузія і так далі по тексту
    quint16 satur;
    quint16 vipar;
    quint16 fqdrv; // монітор частотників


    // індикатори
    QVector<QLineEdit*> d1_VC,s1_VC,s2_VC,v1_VC;
    QVector<QProgressBar*> d1_lV,s1_lV,s2_lV,v1_lV;

    QVector<QCheckBox*> d1_Cb,s1_Cb,s2_Cb,v1_Cb;  // чого Cb фіг його знає.
    QVector<QPushButton*> d1_bC,s1_bC,s2_bC,v1_bC;


    // графіки.
    QVector<TrendChart*> trc;

    QVector<QStringList> trChTags;
    QVector<int> nIo;

    QVector<QPushButton*> s1_bleding;

    QPushButton *d1_oven;
    QVector<QSlider*> v1_SP;

    QVector<QPushButton*> fd1_BN; // тут буде саме цікаве

    QPushButton *xC05,*xC20;
    qint16 last_xC;
    QSpinBox *spPresh;
    QUdpSocket *sock;

    QLabel *ss_REC;
    QVector<QLineEdit*> ss_SUM;

    QLineEdit *V10;
    QPushButton *d1_Y_01;

    QHash<QObject*,QPushButton*> bnFqMbEth;

    AuPlc *frDrv;
    QStringList lastIconNames;

    QList<QLineEdit*> apLe;
    QList<QProgressBar*> apPb;

    QLineEdit *vodaSum;

};

#endif
