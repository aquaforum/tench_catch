#ifndef AUPANELREG_H
#define AUPANELREG_H

#include <QWidget>
#include <QDialog>
#include <QHash>

class AuBase;
class AuPlc;
class TrendChart;
class QVBoxLayout;

namespace Ui {
class AuPanelReg;
}

namespace Ri
{
    enum Index {Deskritp = 0,
                PV_1  = 1,
                PV_2  = 2,
                PV_3  = 3,
                SPR_1 = 4,
                X     = 5,
                SP_1  = 6,
                SP_2  = 7,
                SP_3  = 8,
                AM    = 9,
                Rej   = 10,
                Rev   = 11,
                Kpr   = 12,
                TI    = 13,
                Td    = 14,
                Xmin  = 15,
                Xmax  = 16,
                K_1   = 17,
                K_2   = 18,
                K_3   = 19,
                K_4   = 20,
                Kkor  = 21,
                Mode  = 22,
                P0    = 23,
                To    = 24
                };
}

class AuPanelReg : public QDialog
{
    Q_OBJECT

public:
    explicit AuPanelReg(AuBase &base, quint16 index, int n, QString cfName=":/text/reg.txt", QWidget *parent = nullptr);
    ~AuPanelReg();

protected slots:
    void changeReg(int Index); // зміна регулятор
    void Control(bool); // відображення-приховувавння частини вікна з настройками регулятора
    void runTrend();
    void updateTrend(int);

    void updateData(); // поновлення даних у віджетах
    // і тут буде ще купа сигналів від різних контролів
    // Ctrl
    void setCtrlValue(double v);
    void setCtrlValue(int v);
    // Parm
    void setParmValue(double v);
    void setParmValue(int v);
    void setParmAM(int v);
    void setParmRej(int v);
    void setParmRev();
    void setParmKprSig(int v);
    void setParamMode(bool v);

    void setTO(int);


    void setGraph(); // слот на 5ти-секундний інтервал для виводу гріфіків
    //void startTimer();
    void htrDataLoad(qint32, QByteArray ,int,int);

protected:
    void closeEvent( QCloseEvent * event);

private:
    Ui::AuPanelReg *ui;
    AuBase &myBase;
    quint16 plcIx;
    int RegNum;

    AuPlc &src;

    QVector<QStringList> RegDes; // масив опису параметрів регуляторів

    QVector<double> v; // дані для запису на графік
    // масив для пошуку віджетів
    QHash<QString, Ri::Index> ctrlSearch;     // хеш для пошуку тегів
    QTimer *t1;

    double kk_1 ;  // коефіцієнт корекції для K_1
    TrendChart *trChart;
    QVBoxLayout *tL;
    int htrId;
};

#endif // AUPANELREG_H
