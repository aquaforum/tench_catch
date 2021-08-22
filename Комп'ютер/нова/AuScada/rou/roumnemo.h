#ifndef ROUMNEMO_H
#define ROUMNEMO_H

#include <QWidget>
#include <QVector>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RouMnemo; }
QT_END_NAMESPACE

class AuBase;
class QLineEdit;
class QCheckBox;
class TrendChart;
class QProgressBar;
class QUdpSocket;
class QFrame;
class AuPlc;

class RouMnemo : public QWidget
{
    Q_OBJECT

public:
    explicit RouMnemo(AuBase &source,QWidget *parent = 0);
    ~RouMnemo();
public slots:
    void updateData(); // слот обновляє дані на мнемосхемі

    void updateTrendChart(); // поновлення графіків
    void updateToolTips();

    void updateViparka();
    void updateAparat();

    void slotCallReg();

    void slotOpen();
    void slotClose();

    void setupLimit();
    void slotFqDrvUpdate();

    void slotRead();
    void slotPreshQuery();

protected:
    void paintEvent(QPaintEvent *);

private:
    AuBase &src;
    Ui::RouMnemo *ui;
    QUdpSocket *sock;


    QVector<QLineEdit*> le,le_v1;
    QVector<QProgressBar*> pb_v1;


    QVector<QCheckBox*> cb;

    QVector<TrendChart*> trc;

    QVector<QLineEdit*> ap_le;
    QVector<QProgressBar*> ap_pb;

    quint16 rou;

    QHash<QObject*,int> apIx;

    QFrame* drawLegend(QString tags,TrendChart *trChar,AuPlc &plc);

};

#endif // ROUMNEMO_H
