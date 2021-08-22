#ifndef AUMODBUS_H
#define AUMODBUS_H


#include <auplcdrive.h>
#include <QVector>
#include <QQueue>
#include <QTcpSocket>

class QTcpSocket;
class QTimer;
class AuBase;

//  цей клас не містить даних свох даних, plcData - вказівник на зовнішнє сховище, заповнюється в коснрукторі

// Для того щоб зв'язатись по модбус тцп треба знати ім'я або ір адресу, порт (за замовчанням 502)


class AuModbus : public AuPlcDrive
{
Q_OBJECT
public:
    explicit AuModbus(AuBase &base,quint16 index, qint16 addOffset=1, QObject *parrent=nullptr); // перший параметр - посилання на клас в якому зберігаються дані
                                                // другий параметр - індекс контролера


    ~AuModbus();

    QVector<qint16> &data();// {return  plcData};


public slots:
    void setData(qint32 , QVector<qint16> & );


public:
    int port() {return nPort;} // це для можливості подивитися та змінини номер порту за замовчанням
    void setPort(int Port) {nPort=Port; }

    void setHostName(QString);

    int loadList(QString); // ця функція на основі файла із списком тегів формує чергу запитів для контролера, яку потим крутить по колу
    int start(); // це запускає крутілку вищезгаданої черги, без виклику цієї функції обмін з контролером не розпочнеться

private slots: // локальні функція для обслуговування ТСР-з'єдрання
    void slotConnected (); // приєдналися
    void slotNewConnect(); // тут відбувається реконект у випадку втрати зв'зку з контролером
    void slotTimeout(); // таймаут отримання даних від сервера
    void slotDisconnect(); // відєднання зі сторони сервера

    void slotError(QAbstractSocket::SocketError);

    void slotRead();

private:
    int nPort; // номер порту ТСР в контрері, для I-8000 (і Шнайдера) == 502
    QTcpSocket *pS; //сокет для зв’язку


    qint16 nI; // це буде індекс пакунка
    int nLen; // довжина наступноно бока даних

    //QTimer *connSend; // таймер для відправки чергового запиту
    QTimer *connWait; // тайсер очікування перед спробою встановити нове з’єднання
    QTimer *connTimeout; // таймер таймауту з’єднання, можливо в нових версіях QT цей алгоритм буде непотрібен


    // Список запитів
    QVector<QByteArray> query_list; // список Modbus запитів, які регулярно відправляються до контролера
    QVector<int> dataLen; // довжина відповіді в словах

    QVector<qint16> query_read; // періодичність відправки запитів прочитана з конфігураційного файлу
    QVector<qint16> local_read; // періодичність відправки запитів часу виконання, обробляється в slotRead

    QQueue<QByteArray> query_queue; // черга на відправку даних в контролер

    int nC;
    quint8 unitId;
    qint16 Index;

    bool linkOk; // це прапор який сигналізує наявність зв'язку з контролером

    quint64 cklCount; // кічильник кількості циклів обміну з контролером
    qint16 offset;

    void loadMx(QFile&,qint16);


};

#endif // AUMODBUS_H
