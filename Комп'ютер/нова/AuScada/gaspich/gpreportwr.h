#ifndef REPORTWITER_H
#define REPORTWITER_H

#include <QObject>
#include <QVector>


class AuPlc;

class GpReportWiter : public QObject
{
    Q_OBJECT
public:
    GpReportWiter(AuPlc &source);

public slots:
    void checkState();

private:
    AuPlc &src;

    QVector<int> lastState;
    QVector<QVector<double> > data;

    double prevGst;


};

#endif // REPORTWITER_H
