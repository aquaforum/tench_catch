/*
 * Вся ця хрінь оперує понцепцією "тег"
 * Тег одиниця інформації в системі, описується набором атрибутів
 * а також значенням, як шкальованим так і сирим.
 * має купу атрибутів
 * перш за все це тип даних див. enum tagType
 *
 * по-друге це атрибути пов'язаі із шкалами, їх п'ять.
 * tagScale - тип шкали, див. відповідний enum
 * myFull та myZero відповідно максимум та мінімум
 * myBase - база шкали, у моїх проектах за замовчанням 4000, у Юриних 10000
 * myLinkTag* якщо шкала зв'язана - тоді брати myFull і myZero звідси
 *
 * myText - опис тега, для різноманіття туди ще треба запкласти перспективу локалізації
*/

#ifndef AUTAG_H
#define AUTAG_H

#include <QObject>

#include <QString>

#include "aulibdefs.h" // без цього moc не хоче компілятися....
class AuTag;

// перегружені арифметичні оператори для роботи з тегами
double operator+ (AuTag&, AuTag&);
double operator+ (AuTag & , double );
int operator+ (AuTag &, int );

double operator- (AuTag&, AuTag&);
double operator- (AuTag &, double );
int operator- (AuTag &, int );

double operator* (AuTag &, AuTag &);
double operator* (AuTag &, double );
int operator* (AuTag &, int right);

double operator/ (AuTag &, AuTag &);
double operator/ (AuTag &, double );
int operator/ (AuTag &, int );

int operator% (AuTag &, AuTag &);
int operator% (AuTag &, int );

bool operator== (AuTag &, double );
bool operator== (AuTag &, int );

bool operator!= (AuTag &, double );
bool operator!= (AuTag &, int );


class AuTag : public QObject
{
    Q_OBJECT

public:
    AuTag(QVector<qint16> &, QStringList&, qint32 index , QString &plcName);

public:
    void loadTag(QStringList,qint32);

    // механізми доступу до даних

    // перегружені методи доступу до даних
    // писання
    // тут дані завжди шкалюються
    double operator=(double); // AuTag* ?
    bool operator=(bool); // AuTag* ?
    int operator=(int); // AuTag* ?
    qint16 operator=(qint16);

    // читання
    operator double() const;
    operator int() const;
    operator bool() const;
    operator QString() const; // на мою думку це буде самий цінний спосіб видачі даних, можна гнати текст напряму в QLabel чи QLineEdit....
    operator qint16() const;


    // це нешкальовані дані
    qint16 value();
    qint32 value32();
    double valueReal();
    QString valueText(); // це теж повертається з поправкою на тип і якщо воно Real тоді і на myDecimal

    // це використовується для обходу шкалювання
    void setValue(qint16);
    void setValue(qint32);
    void setValue(double);

    void setValues(QVector<qint16>&);

    // далі пішов інтерфейс доступу до атрибутів
    QString &name(); // повертає моє ім'я

    // це для роботи із шкалами
    // читання
    double full();
    double zero();
    // встановлення шкал
    void setFull(double);
    void setZero(double);

    // доступ до поля бази
    double baseScale();
    void setBaseScale(double v); // {myBase=v;}

    void setLinkTag(AuTag*); // це якщо шкала береться із "сусіднього" тега
    tagScale scaleType();
    tagType  dataType()  {return myType;}
    QString linkTagName();

    // це для встановлення кількості десяткових знаків
    // взагалі для цього поля треба б завести відповідний запис в кофігурації, ну або ж зробити його налаштовуваним
    qint32 deciaml();
    void setDecimal(qint32);

    QString text() {return myText;}
    const QString &plcName() { return myPlcName; }

    int index() {return myIndex; }
    qint16 address() {return myAddress;}

    bool logging() {return myLogging ; }
    qint16 mxMask() {return myMxMask ; }
/*
private:
    // маніпуляції з даними
    union pack dataUnPack(); // це дістани дані з plcRawData і покладе в юніон
    void dataPack(union pack&); // а це навпаки із юніона покладе в plcRawData
*/
    double k(); //{return  myK;}
    double b(); //{return myZero;}


signals:
    void sendData(qint32 address,QVector<qint16> &data); //цей сигнал генерується при необхідності відправити дані в контролер
    void sendData(QString tagName,qint32 address,QVector<qint16> &data);

public slots: // а це для того щоб напряму засилати дані із елементів управління в контролер без проміжного коку
    void setData(int);
    void setData(double);
    void setData(bool);

private:
    // наступні два елементи отримуються від батька і зберігаються в ньому ж. тут тільки посилання
    QVector<qint16> &plcRawData; // в цьому місця зберігатися дані, отримані як напряму із контролерів так і через UDP
    QString &myPlcName; // це ім'я контролера


    // а це все моє, рідненьке. Атрибути тега. тільки значення тут немає. див. вище, там де визначені механізми доступу до даних.
    QString myName; // тут все логічно, моє ім'я
    int myIndex; // індекс в plcRawData !!!! це не те, що індекс в AuBase, тут реальний номер в QVector
    qint32 myAddress; // адрес MODBUS // це для подальшої відправки в контролер, знову ж таки все це злегка запиляно під I-8000

    tagType myType; // тут буде тип тега
    double myZero; // шкала нуль
    double myFull; // шкала розмах
    double myBase; // база для розрахунку шкали
    QString myText; // опис тега
    tagScale myScale; // тип шкали
    qint32 myDecimal; // ця хрінь потрібна для повернення даних в текстовому вигляді, містить інформацію про кількість десяткових знаків, за замовчанням ==0

    AuTag *myLinkTag; // якщо шкали зв'язані - тут посилання на тег із шкалою.
    QString myLinkTagName; // тут ім'я зваязаного тега("сусіда" для видобування його шкали)

    double myK;//,myB; це замінено на дефайн дивись в autag.cpp десь зверху
    void lineCalc(); // функція для розрахунку тих двох товаришів що вище.

    bool myLogging;
    qint16 myMxMask;


};

#endif // AUTAG_H
