/* Цей клас виконує запити до бази даних в
 *
 */
#ifndef AUSQLEXECUTER_H
#define AUSQLEXECUTER_H

#include <QObject>

class AuSqlExecuter : public QObject
{
    Q_OBJECT
public:
    explicit AuSqlExecuter(QString database, QString sqlQuery, QObject *parent = nullptr);

public slots:
    void exec(); //

signals:
    void finished();
    void dataRow(QStringList);
    void error(QString);

private:
    QString dbName;
    QString query;
};

#endif // AUSQLEXECUTER_H
