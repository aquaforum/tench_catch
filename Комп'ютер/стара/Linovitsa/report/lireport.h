#ifndef LIREPORT_H
#define LIREPORT_H

#include <QObject>
#include <QVector>

class IoNetClient;


class LiReport : public QObject
{
    Q_OBJECT
public:
    explicit LiReport(QVector<IoNetClient*> &source,  QObject *parent = 0);

//signals:

public slots:
    void updateDataRaw();
    void updateDataScaled();

private:
    QVector<IoNetClient*> &src; // вказівник на джерело даних
    QVector<double> scale,value;

    int updateCount,vCount;
};

#endif // LIREPORT_H
