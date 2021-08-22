/*цей клас служить для прозорої розпаковки бітових полів із змінних St_i, St_i1, St_i2
 *  якщо запрошегі теги містяться в запакованих полят тоді вернути їх значення
 *  інакше прозоро вернути значення тега.
*/

#ifndef BITMASK_H
#define BITMASK_H

#include <QObject>
#include <QHash>

class AuPlc;

class BitMask : public QObject
{
    Q_OBJECT
public:
    explicit BitMask(AuPlc &plc,QObject *parent = nullptr);

    bool operator [] (QString);
    bool containts(QString);

signals:

private:
    AuPlc &src;
    QHash<QString,qint16> St_i,St_i1,St_i2;


};

#endif // BITMASK_H
