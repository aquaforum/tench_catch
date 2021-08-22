#ifndef AuPlc_H
#define AuPlc_H

#include <QObject>
#include <QVector>
#include <QHash>

class AuTag;
class QFile;

// цей клас обслуговує абстрактний контролер
// він містить сирі дані

// УВАГА!!!! по факуту всі дані містяться тут

class AuPlc :public QObject
{
    Q_OBJECT
public:
    AuPlc(QString name, QString,quint16);
    ~AuPlc();

    AuTag &operator[](QString);

    qint32 loadList(QString txt);

    QStringList tagsList();

    QVector<qint16>& data() {return plcRawData; }

    QString &name(); // ця функція поверне назву контролера, яка використовується для встановлення з'єднання з ним в якості імені хоста
    QString &fileList(); // тут зберігається ім'я файла із тегами.
    quint16 index(); // повертає індекс

    QList<AuTag*> tags() {return tagList.values();}

    void setNet( bool v) {myNet=v;}
    bool net() {return myNet;}

    void setActive( bool v) {myActive=v;}
    bool active() {return myActive;}

    void setText(QString text) {myText=text;}
    const QString text() { return myText; }

    // це буде використовуватися для ретрансляції сигналів про поновлення даних
    bool isOk();// це буде повертати наявність чи відсутність актуальних даних

    // інтервал запису історії.
    void setInterval(int interval) { myInterval=interval;}
    int interval() {return myInterval;}

    void setDbReal(bool val) { myDbReal=val;}
    bool dbReal() {return  myDbReal; }

    void setOffset(qint16 v) { myOffset=v;}
    qint16 offset() {return myOffset;}



signals:
    void updateData();
public slots:
    void setData();

private:
    QString myName; //це ім'я контролера
    QString myFileList;
    quint16 iX;


    QHash<QString,AuTag*> tagList;

    QVector<qint16> plcRawData; // цей елемент містить сирі дані з контролера
    bool myNet,myActive;
    QString myText; // текстова назва
    int myInterval; // інтервал для збереження історії в секундах.

    bool myDbReal;
    qint16 myOffset;

    qint32 loadMx(QFile&, qint32 , qint32 );


};

#endif // AuPlc_H
