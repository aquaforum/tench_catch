#include "gasgetdata.h"
#include <QHttp>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QHttpResponseHeader>

GasGetData::GasGetData(QObject *parent) :
    QObject(parent)
{
    file=new QFile("/tmp/gasreport.html");
    file->open(QIODevice::WriteOnly);

    QHttp *client = new QHttp(this);
    client->setHost("www.izodromgas.com.ua");
    connect(client,SIGNAL(done(bool)),this,SLOT(slotGetData(bool)));
    client->setUser("lin","lin321");
    //client->get("/nodes/211/reports/html/cv_arc?thread=1&start_time=2016-9-10%200:0&end_time=2016-9-11%200:0",file);
    client->get("/nodes/211/reports/html/av_arc?thread=1&start_time=2016-9-10%200:0&end_time=2016-9-11%200:0",file);

}

void GasGetData::slotGetData(bool v)
{
    QHttp *client=qobject_cast<QHttp*>(sender());

    if(v)
        qDebug() << client->errorString();

    file->close();

    qApp->quit();
}


