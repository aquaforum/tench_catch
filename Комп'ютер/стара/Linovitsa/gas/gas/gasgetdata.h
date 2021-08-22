#ifndef GASGETDATA_H
#define GASGETDATA_H

#include <QObject>
class QFile;
class QHttpResponseHeader;

class GasGetData : public QObject
{
    Q_OBJECT
public:
    explicit GasGetData(QObject *parent = 0);

signals:

public slots:
    void slotGetData(bool);

private:
    QFile *file;

};

#endif // GASGETDATA_H
