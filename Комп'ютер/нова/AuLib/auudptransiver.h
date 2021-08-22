/*
 * Тут описано взаємодію мід копіями програми, яку треба налаштувати.
 * є два варіанти
 * 1) програма сама веде обмін з контролером
 * 1а) програма не веде мережевого обміну для цього контролера з іншими програмами (із можливим блокуванням передачі даних в контролер із мережі)
 * 1б) програма веде мережний обмін з іншими програмами для цього контролера
 * 2) програма спілкується з контролером тільки через інші програми (на запис треба перевірити права доступу)
 * отже є чотири варіанти, які будуть настроєні в конфігураційному файлі
 * (запис виду ім'я=індекс)
 * 0 - варіант за замовчанням - тільки локальний обмін з контролером, без мережі.
 * 1 - локальний обмін з контролером, передача даних в мережу без можливості переслати дані із мережі в контролер
 * 2 - локальний обмін з контролером, передача даних в мережу із можливістю переслати дані із мережі в контролер
 * -1 - та всі інші - робота в мережі
 */


#ifndef AUUDPTRANSIVER_H
#define AUUDPTRANSIVER_H
#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class QNetworkDatagram;
class QTcpServer;
class QTcpSocket;

class AuBase;
class AuPlcDrive; // це інтерфейс для передачі даних в контролер по модбасу чи ін...
class AuUdpTransiver : public QObject
{
    Q_OBJECT
public:
    explicit AuUdpTransiver(AuBase &base, QObject *parent = nullptr);
    ~AuUdpTransiver();
    QHash<QString,QHostAddress>& scadaAddreses();


public slots:
    void sendData(quint16);
    void recvData();
    void printStat();

signals:
    void updateData(quint16);


    void Alert(const QString&); // це сигналізує про проблеми

private:
    QUdpSocket *sock,*sSock;
    AuBase &src;
    QHostAddress addr;
    QHash<quint16,quint64> pktCnt; // лічильник надісланих пакунків
    QHash<quint16,quint64> rcvCout; //лічильник отриманих пакунків

    QHash<QHostAddress,quint16> lastCrc;
    // TCP
    // воно проситься щоб його розділили на три окремі частини
    // клієнська частина
    QHash<QString,QHostAddress> scadaAddr; // співставлення ардес ТСР сервера і імені контролера, витягується з UDP мультікаста,

    quint64 allPktCnt,allPktSend;
    QUdpSocket *s2Sock;

};

#endif // AUUDPTRANSIVER_H
