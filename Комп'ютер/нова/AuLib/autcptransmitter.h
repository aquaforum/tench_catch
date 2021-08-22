/*
 * цей клас є клієнською частиною
 *
* 1 - передача даних по мережі в іншу програму
* далі
* quint16:addrress адреса куди писати дані
* qint16:data - самі дані
*
* 2 - передача списку параметрів
* QString;
* 3 - запит на вітправку параметрів, відповідають тільки ті в кого є відповідний plcName
* параметрів немає
*
*
* для UDP_RAW та UDP_PUSH додається контрольна сума qChecksum
* для перевірки задвоєння пукетів
* всі інші ходять дуже рідко тому поки-що перевіряти не буду.
* для великих пакунків
* 4 Відповідь із шкалами
* 5 Запит на дані історії
* 6 Відповідь із даними історії
*
* треба їх якось ідентифікувати
* 7 SQL запит
* 8 SQL відповідь
*
*/

#ifndef AUTCPTRANSMITTER_H
#define AUTCPTRANSMITTER_H

#include <QObject>
#include <QHostAddress>

class QTcpSocket;

class AuTcpTransmitter : public QObject
{
    Q_OBJECT
public:
    explicit AuTcpTransmitter(QHash<QString,QHostAddress> &,QObject *parent = nullptr);

    void setData(QString tagName, quint16 address,QVector<qint16> &data); // це відправить дані в мережу

    void sendScaleQuery(const QString &);

    void datagramParam();
    void datagramSql();

    void sendSqlQuery(const QString&, const QString&, const QString&, int index=0);
    // ця штука наділше
    void findTable(const QString&);

signals: // це віддає дані назад в AuBase
    // void rawSqlResult();
    // void rawSqlError();
    void Alert(const QString&); // це сигналізує про проблеми

    void scaleResult(const QString& plcName, const QHash<QString, QVector<double> > &);

    void sqlResult(QList<QStringList>&);

    void resultRemoteQuery(qint32,QList<QStringList>& );
    void reslutRemoteError(qint32,QString&);

    void tableFinded(const QString&,const QString&,qint16);

private slots:
    // це все тсp
    void slotNewConnection();
    void slotRecvTcp();
    void slotDisconnect();
    void slotSendTcp();

    void sendPkt(const QString &, QByteArray &);

private:
                                           // можливо буде проблему якщо два хоста мультикастить один plcName
    QHash<QTcpSocket*,QByteArray> sockData;// дані для відправики після втановлення з'єднання
    QHash<QString,QHostAddress> &scadaAddr; // співставлення ардес ТСР сервера і імені контролера, витягується з UDP мультікаста,

    QHash<QTcpSocket*,int> pktLen; // кінцевий автомат розбору з'єднань

};

#endif // AUTCPTRANSMITTER_H
