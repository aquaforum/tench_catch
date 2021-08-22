#ifndef TIMESYNC_H
#define TIMESYNC_H

#include <QObject>

class IoDev;

class TimeSync : public QObject
{
    Q_OBJECT
public:
    explicit TimeSync(IoDev &source, QObject *parent = 0);

private slots:
    void slotSetTime();

private:
    IoDev &src;
};

#endif // TIMESYNC_H
