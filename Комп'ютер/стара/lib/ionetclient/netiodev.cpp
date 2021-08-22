#include "netiodev.h"
#include "../iodev.h"

#include "IoNetClient.h"
#include <QDebug>

NetIoDev::NetIoDev(IoNetClient *parent) : p(parent)
{


}

NetIoDev::~NetIoDev()
{

}


void NetIoDev::sendValue(QString tag,qint16 v)
{
#ifndef WIN32

    QVector<qint16> t;
	union fPack
	{
	    float v;
	    struct
	    {
		qint16 hi;
		qint16 lo;
	    };

	}pack;

    if(tags.contains(tag)){
        switch(tags[tag][2])
        {
        case 2: // Real
	    pack.v=v;
            t << pack.hi << pack.lo;
            // !!!!!!!!!!!!!!!!!!!!  *((float*)t.data())=(float)v;
            sendValue(tag,t);
            break;
        case 0: // Integer
        case 1: // Bool
        case 5: // EBOOL
            t << v;
            sendValue(tag,t);
            break;
        case 3: // Long
        case 4: // Timer
            t << 0 << 0;
            *(qint32*)t.data()  = v;
            sendValue(tag,t);
            break;

        default:
            break;
   }
    }
    else
    {
        qDebug() << "sendValue: tag" << tag << "not found";
    }
#else
    qDebug()  << "WIN32 Value: tag" << tag << "not send";
#endif

}

void NetIoDev::sendValue(QString tag,qint32 v)
{
#ifndef WIN32
    QVector<qint16> t(2);
	union fPack
	{
	    float v;
	    struct
	    {
		qint16 hi;
		qint16 lo;
	    };

	}pack;

    if(tags.contains(tag)){

    switch(tags[tag][2])
    {
        case 2:
	    pack.v=v;
            t << pack.hi << pack.lo;
            //!!!!!!!!!!!!!!  *((float*)t.data())=(float)v;
            sendValue(tag,t);
            break;
        case 0:
        case 1: // Bool
        case 5: // EBOOL
            sendValue(tag,qint16(v));
            break;
        case 3:
        case 4:
            *(qint32*)t.data()  = v;
            sendValue(tag,t);
            break;
        default:
            break;
   }
    }
    else
    {
        qDebug() << "sendValue: tag" << tag << "not found";
    }

#else
    qDebug()  << "WIN32 Value: tag" << tag << "not send";
#endif

}

void NetIoDev::sendValue(QString tag,double v)
{
#ifndef WIN32
    QVector<qint16> t;
//    *(float*)t.data()  = (float)v;
//    sendValue(tag,t);

	union fPack
	{
	    float v;
	    struct
	    {
		qint16 hi;
		qint16 lo;
	    };

	}pack;

    if(tags.contains(tag)){

     qDebug() << "netiodev.cpp|sendValue(doulbe " << tag << "," << v << ") type" << tags[tag][2];
        switch(tags[tag][2])
        {
            case 2:
		pack.v=v;
		t << pack.hi << pack.lo;
                // !!!!!!!!!!!!!!!!!!!!!!!!!! *((float*)t.data())=(float)v;
                sendValue(tag,t);
                break;
            case 1: // Bool
            case 5: // EBOOL
            case 0:
                sendValue(tag,qint16(v));
                break;
            case 3:
            case 4:
                sendValue(tag,qint32(v));
                break;
            default:
                break;
        }
    }
    else
    {
        qDebug() << "sendValue: tag" << tag << "not found";
    }
#else
    qDebug()  << "WIN32 Value: tag" << tag << "not send";
#endif


}

void NetIoDev::sendValue(QString tag,QVector<qint16> &v)
{
#ifndef WIN32
    if(p->cMode) {
    QByteArray query;
    QDataStream qry(&query,QIODevice::WriteOnly);
    qry.setVersion(QDataStream::Qt_4_2);

    qry << qint8('W') << qint8('D') << iD << qint16(0) << qint16(0) << tag << v;
    qry.device()->seek(5);
    qry << qint16(query.size()-7);

    p->pTcpSock->write(query);
    }
    qDebug() << tag << v;
#else
    qDebug()  << "WIN32 Value: tag" << tag << "not send";
#endif

}

void NetIoDev::sendValueScaled(QString tag,double v)
{
#ifndef WIN32
    if(p->cMode) {
    if(data_scale.contains(tag))
    {
        QByteArray query;
        QDataStream qry(&query,QIODevice::WriteOnly);
        qry.setVersion(QDataStream::Qt_4_2);
        qry << qint8('W') << qint8('S') << iD << qint16(0) << qint16(0) << tag << v;
        qry.device()->seek(5);
        qry << qint16(query.size()-7);

        p->pTcpSock->write(query);

    } }
#else
    qDebug()  << "WIN32 Value: tag" << tag << "not send";
#endif

}

void NetIoDev::setScaleZero(QString tag,double v)
{
#ifndef WIN32
    if(p->cMode) {
    if(data_scale.contains(tag))
    {
        //qDebug() << "NetIoDev::setScaleZero("<< tag << "," << v << ")";
        QByteArray query;
        QDataStream qry(&query,QIODevice::WriteOnly);
        qry.setVersion(QDataStream::Qt_4_2);
        qry << qint8('W') << qint8('Z') << iD << qint16(0) << qint16(0) << tag << v;
        qry.device()->seek(5);
        qry << qint16(query.size()-7);

        p->pTcpSock->write(query);

    } }
#else
    qDebug()  << "WIN32 set Scale Zero: tag" << tag << "not send";
#endif

}


void NetIoDev::setScaleFull(QString tag,double v)
{
#ifndef WIN32
    if(p->cMode) {
    if(data_scale.contains(tag))
    {
        //qDebug() << "NetIoDev::setScaleZero("<< tag << "," << v << ")";
        QByteArray query;
        QDataStream qry(&query,QIODevice::WriteOnly);
        qry.setVersion(QDataStream::Qt_4_2);
        qry << qint8('W') << qint8('F') << iD << qint16(0) << qint16(0) << tag << v;
        qry.device()->seek(5);
        qry << qint16(query.size()-7);

        p->pTcpSock->write(query);

    } }
#else
    qDebug()  << "WIN32 set Scale Full: tag" << tag << "not send";
#endif


}


