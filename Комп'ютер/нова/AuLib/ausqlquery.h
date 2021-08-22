/* Цей клас виконує запити до бази
 *
*/
#ifndef AUSQLQUERY_H
#define AUSQLQUERY_H

#include <QObject>

class QTcpSocket;

class AuSqlQuery : public QObject
{
    Q_OBJECT
public:
    explicit AuSqlQuery(QString databaseName,QObject *parent = nullptr);

public slots:
    void exec(QString);
    void exec(QStringList); // цей не висилає finished() планується для виконання пачки INSERT

signals:
    void finished(const QList<QStringList>&);
    void finishedId(qint32, const QList<QStringList>&); // по правильному його б треба було робити перегруженим, але мені ліьнки його виправляти у вже написаному коді

    void error(const QString &);
    void end();

private slots:
    void getError(QString);
    void getRow(QStringList);
    void quit();

private:
    QString dbName;

    QList<QStringList> result;
    QString errText;

// це все не обо'язкові речі, цей кла використовується як сховище атрибутів
    // збережемо тут трохи даних  щоб потім ними скористатися
    QString myPlcName;
    QString myTable;
    QString myQuery; // це заповнюється в методі exec
    qint32 myId;

    QTcpSocket *mySock;
public:
    // запис даних
    void setPlcName(QString);
    void setTable(QString);
    void setId(qint32);
    void setSocket(QTcpSocket*);

    // читання даних
    QString plcName();
    QString tables();
    QString query();
    qint32 id();
    QTcpSocket* socket();



};

#endif // AUSQLQUERY_H
