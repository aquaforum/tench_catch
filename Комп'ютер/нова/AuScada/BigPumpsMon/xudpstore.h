#ifndef XUDPSTORE_H
#define XUDPSTORE_H

#include <QString>
#include <QVector>
#include <QHash>
#include <QStringList>


// ця структура описує всі атрибути тера
struct _tag {
    size_t index; // індекс в data_raw
    quint16 address; // адрес MODBUS
    quint8 type; // тут буде тип тега
    double zero; // шкала нуль
    double full; // шкала розмах
    QString text; // опис тега
};


class xUdpStore
{
public:
    xUdpStore();
    xUdpStore(QString);

    double operator[](QString) const; // це буде повертати шкальоване значення.

    // діставання сирих узгоджено з стилем бібліотеки
    qint16 &value16(QString); // базовий
    qint32 &value32(QString);
    float valueFl(QString); // нешкальоване значення

    QStringList tagList() {return tags.keys();} // видача списка тегів
    QString &text(QString tagName) {return tags[tagName].text ;}
    QVector<qint16>&dataRaw() {return data_raw; } // повертається посилання на data_raw для можливості прямого доступу


private:
    QHash<QString,_tag> tags; // це буде індекс для data_raw

    QVector<qint16> data_raw;

    float fZero;
    qint32 iZero;
    qint16 sZero;

    double dZero;


};

#endif // XUDPSTORE_H
