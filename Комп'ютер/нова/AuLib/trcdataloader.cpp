#include "trcdataloader.h"
#include "trendchar/trendchart.h"
#include "aubase.h"
#include <QDebug>
#include <QTimer>

TrcDataLoader::TrcDataLoader(TrendChart *trc, QString tagList, AuBase &base, QObject *parent) : QObject(parent),
    myTrc(trc)
{

    QStringList fields=tagList.split(QRegExp("\\s+"));
    QString plcName=fields[0];
    fields.removeAt(0);

    // qDebug() << fields.join(',');

    quint32 dt=QDateTime::currentDateTime().toTime_t();

    qryIx=base.queryHtrData(plcName,fields.join(','),dt-3600,dt);
    connect(&base,&AuBase::htrData,this,&TrcDataLoader::slotHtrData);

    QTimer::singleShot(15000,this,&TrcDataLoader::deleteLater); // через 15 секунд самознищитися


}

TrcDataLoader::~TrcDataLoader()
{
    // qDebug() << "TrcDataLoader::~TrcDataLoader()";
}


void TrcDataLoader::slotHtrData(qint32 htrId, QByteArray data ,int row,int col)
{

    if(qryIx==htrId)
    {
        //qDebug() << this << qryIx << htrId;
        myTrc->loadData( data,row,col);
        // deleteLater();
    }
}


