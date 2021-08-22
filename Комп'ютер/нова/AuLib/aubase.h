#ifndef AUBASE_H
#define AUBASE_H

#include <QObject>
#include <QVector>
#include <QHash>


/* трохи термінології від 20200215
 * все вищенаписане треба розглядати в контексті поточної копії програми яка виконується на поточному комп'ютері.
 * ця поточна копія називатиметься місцевою,
 * відповідно всі дані які така місцева копія зберігає у себе на диску називаються місцевими
 * всі інші копії програми що запущені на сусідніх комп'ютерах так ні називатимуться - сусіди і їхні дані - сусідні.
 * місцева програма може вести обмін даними з контролером напряму, такий контролер називається активним в місцевому контексті.
 * всі інші контролери - по аналогії з іншим - сусідні.
 *
 * я спробую повиправляти всі інші кометарі згідно з цієї термінології.
*/



/*
 * 2017-10-19
 * Цей клас буде відповідати за обмін даними між прокладкою та клієнтскою частиною
 * ідея  в чому. Щоб зробити доступ доданих по принциму двовимірного масиву і позбавитися викликів get/send
 * отже шкальовані дані будуть діставатися таким чином
 * double value = X[crcIx][tagName];
 * value - в цю змінну будуть записані дані отримані із мережі
 * X - екземпляр класу
 * crcIx - індекс адреса контролера,
 * Важливо!!! цей оператор перегружено так, щоб він працював в обидава боки, наприклад
 * X[crcIx][tagName]=data;
 * не шкальовані значення дістаються функціями value(), Приклад
 * qint16 value= X[crcIx][tagName].value();
 * qint32 value= X[crcIx][tagName].value32();
 * double value= X[crcIx][tagName].valueFloat();
 *
 *
 * PS хочу щоб код був красивим!!!
 */

// контролер ідентифікується за допомогою ідентифікатора quint16 або по імені в QString.
// власне цей клас відрізняється від звичайного масива тим що реалізує


/* Опис інтерфейсу доступу до історії
 * Для доступу до історії використовується дві публічних функцій класу AuBase
 * Запит на дані потрібно відправити за допомогою
 * int queryHtrData(QString plcName, QString tagList, quint32 startT, qint32 stopT,qint32 iD=0);
 * plcName - ім'я контролера
 * tagList - список тегів, через кому
 * startT та stopT - інтервал часу в time_t (час Юнікс), треба б його роширити до 64 біт бо через пару десятків років буде проблема
 * iD необов'язковий параметр - ідентифікатор запиту, потрібен тому що одночасно(послідовно) AuBase може обробляти ще й запити по мережі
 * в результаті виконання повертається ідентифікатор запиту, який при не нульовому iD просто його дублює.
 * така схема потрібна для обробки запитів по мережі, AuUdpTransiver незважає на iD а просто його пресилає в обидва боки
 * дані повертаються сигналом
 * із параметрів виклику буде сформовано запит виду
 * SELECT Dt,<tagList> FROM <plcName> WHERE Dt BETWEEN <startT> AND <stopT> ORDER BY Dt;
 * void htrData(int iD,QVector<QVector<qint32> > &data );
 * iD це відповідно ідентфікатор запиту який був згенерований вище
 * data це самі дані у порядку
 * "Dt+<tagList>
 * Для різноманіття треба ще повернути помилку
 *
*/

#include "auplc.h"
#include <QQueue>

class AuPlcDrive;
class AuUdpTransiver;
class AuTcpTransmitter;
class AuTcpReceiver;

class QNetworkDatagram;
class TrendChart;

// час знаходження імені таблиці в кеші, макс 10хв
#define CACHE_TIME 600
// тут ще писати і писати
class  AuBase : public QObject
{
    Q_OBJECT
public:
    AuBase(QString, QObject *parrent=nullptr); // конструктор, тут треба вписати ініціалізацію всієї системи обміну даними
    ~AuBase();


    // це методи доступу до конкретних пристроїв
    AuPlc  &operator[](quint16); // це по індексу
    AuPlc &operator[](QString);  // це по імені

    // ну я навіть не знаю навіщо ця фігня, нехай поки що буде, можливо для мети відлагодження
    QHash<QString,quint16> &plcList();
    QHash<quint16,AuPlc*> &getTags() {return plcS;}

    quint16 index(AuPlc*);

    quint32 size();

//   AuPlcDrive *plcDrive(quint16 iX) {return myPlcDrive[iX];}
//   void setPlcDrive(AuPlcDrive* plcD) {my};



    const AuUdpTransiver* transiver() {return  myTransiver;}
    void setTransiver(AuUdpTransiver* trans) {myTransiver=trans;}

    const AuTcpReceiver* receiver() { return  myTcpRx ;}
    void setReceiver(AuTcpReceiver* rx) {myTcpRx=rx;}

    const AuTcpTransmitter* transmitter() { return  myTcpTx;}
    void setTransmitter(AuTcpTransmitter* tx) { myTcpTx=tx;}

    qint32 activePlcCount() {return activePlc.size(); }


    void scaleQuery(const QString&);
    void loadTrendChartData(TrendChart*,QString);


public slots:
    void setData(quint16);
    void slotUpdateData();

    void addActivePlc(AuPlcDrive *plc) { activePlc << plc; }
    // void inputScaleResult(const QString& plcName, const QHash<QString, QVector<double> > &);


signals:
    void updateData(quint16); // цей сигнал висилається при отриманні свіжої порції даних, параметр - індекс контролера для якого отримано свіжу порцію даних.
    void Alert(const QString&);

    // інтерфейс виконання SQL запитів, добре якщо воно локальне, а якщо ні?
    // тому так методом sqlQuery(QString, QString) відправляємо дані перший параметр - таблиця, другий - запит до цієї таблиці
    // результати повертаються сигналом sqlResult(QList<QStringList>)
    // або буде sqlError(QString) якщо виникла помилка
public slots:
    // наступна функція надсилає запит на дані
    // повертає ідентифікатор запиту, який буде продубльовано в сигналі
    qint32 queryHtrData(QString plcName, QString tagList, quint32 startT, qint32 stopT,qint32 iD=0);
    qint32 querySql(const QString &table, const QString &query); // це виконує запит до БД або відправляє в мережу, якщо в локальній БД такої таблиці немає
    void processQuery(qint32 , const QList<QStringList> &);

    void setTablePlcName(const QString&,const QString&, qint16);

    void getAlert(const QString&);

signals:
    void htrData(int iD,QByteArray data,int row,int col);  // цей сигнал надсилається при отримані даних
    void sqlData(qint32, const QList<QStringList>&);
    void sqlError(QString); // або цей повертає текст з посилкою



private:

    QHash<quint16, AuPlc* > plcS; // це буде індекс для data_raw
    QHash<QString,quint16> names; // тут зберігається співставлення між іменами та індексами
//    QHash<quint16, AuPlcDrive*> myPlcDrive;

    AuUdpTransiver *myTransiver;
    AuTcpReceiver *myTcpRx;
    AuTcpTransmitter *myTcpTx;

    QList<AuPlcDrive*> activePlc; // тут активні контролери

    QVector<qint32> htrIx;


//    QString findPlcName(QString);
    QString remoteQuerys;
    qint32 remoteIdx;
// це кеш із іменем коп'ютрера який має необхідну таблицю.
    time_t lastTime;
    QString lastTable;
    QString lastPlcName;

    QList<QStringList> sqlStore;

};

#endif // AUBASE_H
