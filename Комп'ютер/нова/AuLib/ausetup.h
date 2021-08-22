#ifndef AUSETUP_H
#define AUSETUP_H

#include <QObject>

// всі налаштування роблаться тут
// цей клас створює AuBase для GUI та все інше для обміну даними
// як вхідний параметр отримується файл що містить список контролерів та файлів із списком тегів.
// за замовчанням іде значення ":/plc.list"
// далі воно читає за допомогою QSettings що там має бути налаштовано та запускає його
// див. реалізацію, там трохи більше кометнарів.

class AuBase;
class AuPlcDrive;
class AuUdpTransiver;
class AuTcpTransmitter;
class AuTcpReceiver;

class AuLogging;


class AuSetup : public QObject
{
    Q_OBJECT
public:
    explicit AuSetup(QString plcList=QStringLiteral(":/plc.list"), QObject *parent = nullptr);
    ~AuSetup();

    AuBase& base() {return *src;} // за допомогою цього методу вертається AuBase для GUI
    AuUdpTransiver* udpTransiver() {return  transiver;}


signals:

private:
    AuBase *src;
    AuUdpTransiver *transiver;
    AuTcpTransmitter *tcpTx;
    AuTcpReceiver *tcpRx;

    AuLogging *myLogging;

};

#endif // AUSETUP_H
