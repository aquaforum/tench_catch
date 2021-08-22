#ifndef AUPLCDRIVE_H
#define AUPLCDRIVE_H

#include <QObject>

#include <aubase.h>

// цей клас - це шаблон для побудови інтерфейсів зв'язку з контролером
// він містить базові речі, які мають бути спільними для всіх контролерів, хоча вони - контролеронезалежні

class AuPlcDrive: public QObject
{
Q_OBJECT
public:
    AuPlcDrive(AuBase &base, quint16 index, QObject *parrent=nullptr): QObject(parrent),
        src(base),
        iX(index),
        plcData(base[index].data()),
        myName(base[index].name())
    {}// конструктор, просто заповнє поля даних коаса

    ~AuPlcDrive() ;
    virtual int start() =0;

signals: // сигнали які будуть вислані реальним драйвером
    void updateData(QVector<qint16>&); // це при надходженні даних
    void updateData();
    void Alert(QString); // а це всілякі повідомлення

public slots:
    virtual void setData(qint32 ,QVector<qint16>&)=0; // ця річ реалізує інтерфейс надсилання даних в контролер,
    //virtual void setData(QString, quint16 ,QVector<qint16>&)=0; // ця річ реалізує інтерфейс надсилання даних в контролер,
    // вона - віртуальна і мусить бути переписана в кожному нащадку
    // хоча варіант не дуже гарний, це тільки для контролера І-8000 підходить, із однотипним адресним простором
    // у Шнайдера, наприкла є EBOOL
    // а у Міцюбісі так взагалі X... Y... D... L... і ще хірн зна скільки всякої всячини

protected:
    AuBase &src; // тут базовий клас
    quint16 iX; // індекс контролера
    QVector<qint16> &plcData; // сюди записуємо дані
    QString &myName; // тут мережеве ім'я контролера

};

#endif // AUPLCDRIVE_H
