#include "aulibdefs.h"
#include "auudp.h"

AuUdp::AuUdp(QHash<quint16 , QVector<qint16> > &plcData,QObject *parent) : QObject(parent),
    plcRawData(plcData)
{

}
