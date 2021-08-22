#include "errormanager.h"
#include <aulib.h>

#include <QFile>
#include <QDir>

#include <QRandomGenerator>
#include <QDebug>


ErrorManager::ErrorManager(AuPlc &source,QString alertListFile, QObject *parent) :
    QObject(parent),src(source),
    srcBM((new BitMask(source,this))[0])

{
    QFile fList(alertListFile);
    fList.open(QIODevice::ReadOnly);
    while(!fList.atEnd())
    {
        QString line=fList.readLine().trimmed();
        QStringList fields= line.split(',');

        SAFE(
        lastVal << srcBM[fields[0]]; // це повинно бути саме тут на випадокщо тега немає той в списки для перевірки воно не попаде
        tags << fields[0];
        flags.append( fields[1][0].toLatin1());
        messages << fields[2];
        )
    }
    fList.close();
    activeIx.fill(0,tags.size());

    connect(&src,&AuPlc::updateData,this,&ErrorManager::updateData);
}

ErrorManager::~ErrorManager()
{

}

void ErrorManager::updateData()
{
    // в цьому місці крок за кроком треба перевіряти всі помилки
    for(int i=0;i<tags.size();++i)
    {
        bool val=srcBM[tags[i]];
        // qDebug() << tags[i] << val;
        if(val && (!lastVal[i]) ) // передній фронт
        {
            activeIx[i]=QRandomGenerator::global()->generate();
            emit AlertON(activeIx[i],messages[i]); // вислати сигнал про включення аварії
            //qDebug() << tags[i] << val << activeIx[i] << messages[i] << i;

        }
        else if(lastVal[i] && !val )
        {
            emit AlertOFF(activeIx[i]); // вислати сигнал про вилючення аварії
            activeIx[i]=0;
        }

        lastVal[i]=val;
    }



}

bool ErrorManager::redge(QString )
{
    return false;
}

bool ErrorManager::fedge(QString )
{
    return  false;
}

void ErrorManager::slotWriteMessage(QString )
{

}

