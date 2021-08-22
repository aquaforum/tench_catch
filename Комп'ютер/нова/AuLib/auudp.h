#ifndef AUUDP_H
#define AUUDP_H

#include <QObject>

class AuUdp : public QObject
{
    Q_OBJECT
public:
    explicit AuUdp(QHash<quint16 , QVector<qint16> > &plcRawData, QObject *parent = nullptr);

signals:
    void updateData(quint16); // отримано дані

public slots:
    void sendData();

private:
    QHash<quint16 , QVector<qint16> > &plcRawData; // тут будуть зберігатися дані з контролерів, отримані як напряму із контролерів так і через UDP

};

#endif // AUUDP_H
