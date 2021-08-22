#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QStringList>
#include "bitmask.h"

class AuPlc;

class ErrorManager : public QObject
{
    Q_OBJECT
public:
    explicit ErrorManager(AuPlc &source, QString alertListFile, QObject *parent = 0);
    ~ErrorManager();
signals:
    void AlertON(qint32,QString);
    void AlertOFF(qint32);

private slots:
    void updateData();
    void slotWriteMessage(QString);


private:
    bool redge(QString tag);
    bool fedge(QString tag);

    AuPlc &src;
    BitMask &srcBM;

    QStringList tags; // список тегів
    QStringList messages; // список повідомлень
    QByteArray flags; // список прапорів що перевіряти

    QVector<qint32> activeIx;
    QVector<bool> lastVal;



};

#endif // ERRORMANAGER_H
