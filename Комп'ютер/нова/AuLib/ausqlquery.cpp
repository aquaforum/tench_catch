#include "ausqlquery.h"
#include "ausqlexecuter.h"
#include <QThread>
#include <QDebug>

AuSqlQuery::AuSqlQuery(QString databaseName, QObject *parent) : QObject(parent)
  , dbName(databaseName)
  , errText(QStringLiteral(""))
{

}



void AuSqlQuery::exec(QString sqlQuery)
{
    AuSqlExecuter *sqlExec=new AuSqlExecuter(dbName,sqlQuery);
    QThread *thread = new QThread;

    sqlExec->moveToThread(thread);
    myQuery=sqlQuery;

    // запустити по сигналу
    connect(thread,&QThread::started,sqlExec,&AuSqlExecuter::exec);

    // послідовно зупинити
    connect(sqlExec,&AuSqlExecuter::finished,thread,&QThread::quit);
    // коли потік виконання буде зупинено, просигналізувати сюди щоб завершити роботу
    connect(thread,&QThread::finished,this,&AuSqlQuery::quit);
    // забрати дані
    connect(sqlExec,&AuSqlExecuter::dataRow,this,&AuSqlQuery::getRow);
    // повідомлення про помилки
    connect(sqlExec,&AuSqlExecuter::error,this,&AuSqlQuery::getError);

    // видалии, можливо видаляти прийдеться іншим сигналом, в
    connect(this,&AuSqlQuery::end,sqlExec,&AuSqlExecuter::deleteLater);
    connect(this,&AuSqlQuery::end,thread,&QThread::deleteLater);
    connect(this,&AuSqlQuery::end,this,&AuSqlQuery::deleteLater);

    thread->start();
}

// це зарезервовано на майбутнє
void AuSqlQuery::exec(QStringList)
{


}

// цей метод збереже дані і відправить сигнали всім іншим
void AuSqlQuery::getError(QString msgError)
{
    errText=msgError;
}

void AuSqlQuery::getRow(QStringList row)
{
    result << row;
}

void AuSqlQuery::quit()
{
    // qDebug() << result;

    if(errText.size()>0)
        emit error(errText);
    else
    {
        emit finished(result);
        emit finishedId(myId,result);
    }

    emit end();

}

void  AuSqlQuery::setPlcName(QString plcName)
{
    myPlcName=plcName;
}
void  AuSqlQuery::setTable(QString tableName)
{
    myTable=tableName;
}
void  AuSqlQuery::setId(qint32 iD)
{
    myId=iD;
}
void  AuSqlQuery::setSocket(QTcpSocket* tcpSocket)
{
    mySock=tcpSocket;
}

// читання даних
QString  AuSqlQuery::plcName()
{
    return myPlcName;
}
QString  AuSqlQuery::tables()
{
    return myTable;
}
QString  AuSqlQuery::query()
{
    return  myQuery;
}
qint32  AuSqlQuery::id()
{
    return myId;
}
QTcpSocket*  AuSqlQuery::socket()
{
    return mySock;
}

