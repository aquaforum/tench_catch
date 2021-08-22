#ifndef AUCOLLECTDATA_H
#define AUCOLLECTDATA_H

#include <QObject>

class AuPlc;

class AuCollectData : public QObject
{
    Q_OBJECT
public:
    explicit AuCollectData(AuPlc &plc,int interval, QObject *parent = nullptr);

signals:
    void pushData(QString);
private slots:
    void slotCollectData();
private:
    AuPlc &myPlc;
    int myInterval;

    QStringList fields;



};

#endif // AUCOLLECTDATA_H
