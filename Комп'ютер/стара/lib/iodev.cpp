#include <QSettings>

#include "iodev.h"


IoDev::~IoDev()
{

}


void IoDev::sendValueScaled(QString tag,double v)
{
    if(tags.contains(tag))
    {
        //qDebug() << "sendValueScaled: tag -" <<  tag << "type:" << tags[tag][2] << " ; value :" << v ;
        QVector<qint16> t(2);
        data_scale[tag][0]=v;
        v=((v-data_scale[tag][1])/(data_scale[tag][2] - data_scale[tag][1]) *4000.0 );

        switch(tags[tag][2]) // тип даних
            {
                default:
                case 0: // Integer
                    sendValue(tag,qint16(v));
                    break;
                case 1: // Bool
                    break;
                case 2: // Real
                    sendValue(tag,v);
                    break;
                case 3: // Timer
                case 4: // Long
                    sendValue(tag,qint32(v));
                    break;
            }


    }
    else
        qDebug() << "Tag not found " << tag;

}

void IoDev::setScaleZero(QString tag,double v)
{
    //qDebug() << "setScaleZero("<< tag << "," << v << ")";
    if(data_scale.contains(tag))
    {
        QSettings s;
        //qDebug() << "IoDev::setScaleZero to " << QString("/ioserv/scale/%1/Zero").arg(objectName());

        s.beginGroup(QString("/ioserv/scale/%1/Zero").arg(objectName()));
        s.setValue(tag,v);
        data_scale[tag][1]=v;
    }
    else
    {
        qDebug() << "Tag not found " << tag;
    }

}

void IoDev::setScaleFull(QString tag,double v)
{
    //qDebug() << "setScaleZero("<< tag << "," << v << ")";
    if(data_scale.contains(tag))
    {
        QSettings s;
        //qDebug() << "IoDev::setScaleFull to" << QString("/ioserv/scale/%1/Full").arg(objectName());

        s.beginGroup(QString("/ioserv/scale/%1/Full").arg(objectName()));
        s.setValue(tag,v);
        data_scale[tag][2]=v;
    }
    else
    {
        qDebug() << "Tag not found " << tag;
    }

}


double IoDev::getValueFloat(QString tag)
    {
        double v=0.0;
	union fPack
	{
	    float v;
	    struct
	    {
		qint16 hi;
		qint16 lo;
	    };

	}pack;

        if (tags.contains(tag) && tags[tag][0]<data_raw.size() )
        {
            switch(tags[tag][2]) // перевірити тип змінної
            {   case 2:
		    pack.hi=data_raw[tags[tag][0]];
		    pack.lo=data_raw[tags[tag][0]+1];
                    v=pack.v;//!!!!!!!!!!!!! *(float*)(data_raw.data()+tags[tag][0]);
                    break;
                case 0:
                    v=data_raw[tags[tag][0]];
                    break;
                case 3:
                case 4:
                    v=*(qint32*)(data_raw.data()+tags[tag][0]);
                    break;
                default:
                    break;
            }
        }
        return v;
    }
