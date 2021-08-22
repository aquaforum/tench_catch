#ifndef AUTAGSETUP_H
#define AUTAGSETUP_H

#include <QWidget>

class AuBase;
class QNetworkDatagram;

namespace Ui {
class AuTagSetup;
}

class AuTagSetup : public QWidget
{
    Q_OBJECT

public:
    explicit AuTagSetup(AuBase &base, QWidget *parent = nullptr);
    ~AuTagSetup();
private slots:
    //void slotPlcChange(const QString &);
    //void slotTagChange(const QString &);

    void slotPlcChange(int);
    void slotTagChange(int);

    void setZero(double);
    void setFull(double);
    void setBase(int);
    void setDecimal(int);

    void slotSendQuery();
    void slotDatagram(const QString&, const QHash<QString, QVector<double> > &);


private:
    Ui::AuTagSetup *ui;
    AuBase &src;
    quint16 lCrc;

    QHash<QString, QHash<QString, QHash<quint8,double> > > netPush;
};

#endif // AUTAGSETUP_H
