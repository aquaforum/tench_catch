/*
 * цей клас є серверною частиною
 *
*/

#ifndef AUTCPRECEIVER_H
#define AUTCPRECEIVER_H

#include <QObject>
#include <QHash>


class QTcpSocket;
class QTcpServer;
class AuPlcDrive;
class AuBase;
class AuSqlQuery;

class AuTcpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit AuTcpReceiver(AuBase& source, QObject *parent = nullptr);

signals:
    void updateData();
    void updateData(QVector<qint16>&); // це при надходженні даних
    void Alert(const QString&); // це сигналізує про проблеми

public slots:
    // void slotRawSqlQuery(); // видобуває дані з бази, смикається таймером
    void addPlcDrive(QString plcName, AuPlcDrive* plcDrive);

private slots:
    // це все тсp
    void slotNewConnection();
    void slotRecvTcp();
    void slotDisconnect();
    void slotSendTcp();

    // для SQL
    void sqlResultSend(const QList<QStringList> &);
    void sqlErrorSend(const QString&);

    // void slotHtrDataUdpSend(int iD, QByteArray data, int row, int col); // це відправить дані історії в мережу3

private:
    // серверна частина
    QTcpServer *myTcpServer;
    AuBase &src;

    QHash<QTcpSocket*,int> pktLen; // кінцевий автомат розбору з'єднань

    // для SQL запитів
    QHash<qint32,QString> iDs; // список активих мережених запитів до історії
    QString table;
    QString query;
    quint16 rawSqlIx;

    QHash<QString,AuPlcDrive*> myPlcDrive;


};

#endif // AUTCPRECEIVER_H
