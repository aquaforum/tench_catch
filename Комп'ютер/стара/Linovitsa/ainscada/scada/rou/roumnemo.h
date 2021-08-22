#ifndef ROUMNEMO_H
#define ROUMNEMO_H

#include <QWidget>
#include <QVector>

namespace Ui {
class RouMnemo;
}

class IoNetClient;
class QLineEdit;
class QCheckBox;
class TrendChart;
class QProgressBar;
class QUdpSocket;

class RouMnemo : public QWidget
{
    Q_OBJECT

public:
    explicit RouMnemo(QVector<IoNetClient*> &source,QWidget *parent = 0);
    ~RouMnemo();
public slots:
    void updateDataRaw(); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrendChart(); // поновлення графіків
    void updateToolTips(int);

    void updateViparka();

    void slotCallReg();

    void slotOpen();
    void slotClose();

    void setupLimit();
    void slotFqDrvUpdate();

    void slotRead();
    void slotPreshQuery();


private:
    QVector<IoNetClient*> &src;
    Ui::RouMnemo *ui;
    QUdpSocket *sock;


    QVector<QLineEdit*> le,le_v1;
    QVector<QProgressBar*> pb_v1;


    QVector<QCheckBox*> cb;

    QVector<TrendChart*> trc;

    QVector<QLineEdit*> ap_le;
    QVector<QProgressBar*> ap_pb;


};

#endif // ROUMNEMO_H
