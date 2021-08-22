#ifndef AULIBDEFS_H
#define AULIBDEFS_H

/*
 * цей файл містить всі числові константи, щоб не було магічних чисел
 */

// ці використовуються для генерації виключень на предсет відсутнього контролера, тега щи ще якоїсь холери....
#define THROWN_NOPLC 1
#define THROWN_NOTAG 2

// це налаштування мережі
#define ADDR QStringLiteral("225.0.0.1")
#define PORTN 41550 //hex(code(AU))
#define QTVER QDataStream::Qt_5_8

#define OFF -1 // виключено, обмін даними в буль-якому вигляді відсутній
#define LOCALONLY 0 // тільки локальний обмін, без мережі
#define LOCALMON 1 // локальний з віддачею даних в мережу
#define LOCALFULL 2 // локальний обмін з повним доступом по мережі
#define NETFULL 3 //  повний доступ по мережі, якщо на іншійстанції буде LOCALFULL
#define NETMON 4

// ft << "Integer" <<  "Bool"<<  "Real" << "Timer" <<  "Long" << "EBOOL"
// типи тегів
enum tagType{
    DEF = 0, // за замовчанням теж що й Integer
    INT = 1, // "Integer"
    BOOL = 2, // "Bool"
    REAL = 3, // "Real"
    TIMER = 4, // "Timer"
    LONG = 5, // "Long"
    EBOOL = 6, // "EBOOL"
    QX = 7, // це біти шнайдера
    IX = 8,
    MX = 9
};

// типи шкал тегів
enum tagScale {
    NONE = 0, // немає шкали
    SET = 1,  // шкала задається
    CONST= 2, // шкала конастанта
    LINK =3   // шкала зв'язується із іншим тегом
};


// коди функцій UDP протоколу
#define UDP_RAW 0 // передача масиву plcRawData
#define UDP_PUSH 1 // передача значень окремих тегів
#define UDP_PARAM 2 // передача окремих атрибутів окремих тегів
#define UDP_ALLPARAM_SEND 3 // запит на передачу всіх атрибутів всіх тегів
#define UDP_ALLPARAM_RECV 4 // передача всіх атрибутів всіх тегів

#define UDP_SQL_QUERY 5 // запита даних історії
#define UDP_SQL_RESULT 6 // відповідь даних на запит історії
#define UDP_SQL_ERROR 7 // якщо запит завершивася помилкою, це надішле тект повідомлення про помилку

#define TCP_FINDTABLE 8 // запитна пошук таблиці

#define TCP_HEADER_LEN 4 // довжина ТСР заголовку, в основномі він буде містити довжину самого пакунка

// коди атрибутів використовуються в функціях UDP_PARAM та UDP_ALLPARAM_RECV
#define TAG_ZERO 0
#define TAG_FULL 1
#define TAG_BASE 2
#define TAG_DECIMAL 3

// це самий простий захисний макрос на відсутність plcName та tagName
// просто нічого не робити, ковтнути помилку

// так ризиковано але мабуть дуже добре повпливає на швидкодію
#ifndef QT_NO_DEBUG
#define SAFE(X) try{ X; } catch(QString txt) { qDebug() << "Tag not found: " << this->objectName()+"."+txt; } catch(...) {}
#define CATCHALL catch(QString txt) { qDebug() << "Tag not found: " << this->objectName()+"."+txt; } catch (...){}
#else
#define SAFE(X) X
#define CATCHALL catch (...){}
#endif



// це база даних за замовчанням
#define AUDBNAME QStringLiteral("logging")
// час зберігання історії кожні 5 секуна з прив'язкою до годинника, тобто на 0 5 10 15 20 25 30 35 40 45 50 55
#ifndef WIN32
#define DBFILE        QDir::homePath().append("/").append(qApp->organizationName()).append(".db")
#else
#define DBFILE        qApp->organizationName().append(".db")
#endif

#define STOREINTERVAL 5000


#define SETTINGS #ifdef WIN32 \
"AuScada.ini", QSettings::IniFormat \
#endif

#endif // AULIBDEFS_H
