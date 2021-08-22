#ifndef __GPMNEMO_H__
#define __GPMNEMO_H__

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
class QLabel;



class GpMnemo: public QWidget
{
    Q_OBJECT
public:
    GpMnemo(QVector<IoNetClient*> &source, QWidget *p=NULL);
    ~GpMnemo();

public slots:
    void updateDataRaw(); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrendChart(); // поновлення графіків
    void updateToolTips(int);

    void slotCallReg();
    void slotSetAM(int);
    void slotSetup();
    void slotControl();
    void slotCallSocalka();

    void slotShowErrorLog();


private:
    QWidget *m_gp;

    QVector<IoNetClient*> &src;




    // графіки.
    QVector<TrendChart*> trc;
    QVector<QStringList> trChTags;


    // індикатори
    QVector<QLineEdit*> s1_VC,v1_VC;
    QVector<QProgressBar*> g1_lV,s1_lV,v1_lV;

    QVector<QCheckBox*> s1_Cb,v1_Cb;  // чого Cb фіг його знає.
    QVector<QPushButton*> g1_bC,s1_bC,v1_bC;

    QVector<QLineEdit*> g1_VC,gb,gt;
    //QVector<QProgressBar*> g1_lV;

    QVector<QCheckBox*> g1_Cb;  // чого Cb фіг його знає.
    //QVector<QPushButton*> s1_bC;
    QVector<QLabel*> g1_ll;
    QLineEdit *State;

    QStringList lsState;
    QVector<QSlider*> s1_SP;

    QStringList gpErrMsg;
    int lastErr;


};

#endif
