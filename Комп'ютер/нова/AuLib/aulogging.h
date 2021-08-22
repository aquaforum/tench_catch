#ifndef AULOGGING_H
#define AULOGGING_H

#include <QObject>
#include <QVector>
#include <QQueue>

class AuBase;

class AuLogging : public QObject
{
    Q_OBJECT
public:
    explicit AuLogging(AuBase &base, QObject *parent = nullptr);
    ~AuLogging(); // тут треба закрити базу даних

    void setStoreInterval(int val) {myStoreInterval=val;}
    int storeInterval() {return myStoreInterval;}

signals:
private slots:
    void slotStoreQuery();
    void slotCollectData(QString);

private:
    AuBase &src;

    QVector<quint16> activeList;

    QQueue<QString> queryPool;
    int myStoreInterval;


};

#endif // AULOGGING_H
