#ifndef TRCDATALOADER_H
#define TRCDATALOADER_H

#include <QObject>
class TrendChart;
class AuBase;

class TrcDataLoader : public QObject
{
    Q_OBJECT
public:
    explicit TrcDataLoader(TrendChart *trc,QString tagList ,AuBase &base, QObject *parent = nullptr);
    ~TrcDataLoader();
private slots:
    void slotHtrData(qint32, QByteArray ,int,int);

signals:

private:
    TrendChart *myTrc;
    qint32 qryIx;

};

#endif // TRCDATALOADER_H
