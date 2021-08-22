#ifndef __MNEMO_H__
#define __MNEMO_H__

#include <QWidget>
#include <QVector>

class TrendChart;
class IoNetClient;
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

namespace Ui {
    class wMnemo;
}

class Mnemo: public QWidget
{
    Q_OBJECT
public:
    Mnemo(QVector<IoNetClient*> &source, QWidget *p=NULL);
    ~Mnemo();

public slots:
    void updateDataRaw(); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrendChart(); // поновлення графіків
    void updateToolTips(int);

    void slotCallReg();
    void slotCallVbCtrl();
    void slotCallPumpCtrl();
    void slotCallAmVater();
    void slotShowMassView();

    void slotFqDrvUpdate();
    void slotSendSpPresh(int);
    void slotSpPreshSend();



private:
    QWidget *m_d,*m_s,*m_v;

    QVector<IoNetClient*> &src;


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

};

#endif
