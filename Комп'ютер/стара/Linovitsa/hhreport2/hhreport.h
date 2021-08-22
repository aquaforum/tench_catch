#ifndef HHREPORT_H
#define HHREPORT_H

#include <QObject>
#include <QVector>

class IoNetClient;
class QFile;

class HhReport : public QObject
{
    Q_OBJECT
public:
    explicit HhReport(QObject *parent = 0);

signals:

private slots:
    void slotGasDataReady(bool);
    //void slotUpdateaData();

private:
    //QVector<IoNetClient*> &src;
    QFile *file;

    QVector<double> mass,gass;
    bool gasOk;
    qint32 updateCount;

};

#endif // HHREPORT_H
