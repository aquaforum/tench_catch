#include "aulibdefs.h"

#include "auplc.h"
#include "autag.h"

#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QHash>
#include <QApplication>

AuPlc::AuPlc(QString name, QString tagFileList, quint16 inDex): myName(name)
  , myFileList(tagFileList)
  , iX(inDex)
  , myNet(false)
  , myActive(false)
  , myInterval(5)
  , myDbReal(false)
  , myOffset(1)
{
    //qDebug() << myName;
    loadList(tagFileList);
    setObjectName(name);
}



AuPlc::~AuPlc()
{
    //  при штатномі виході зберегти дані на диск
    QFile file;
        file.setFileName(QString("%1/.auscada/%2.plc").arg(QDir::homePath()).arg(iX,4,16,QChar('0')));
        file.open(QIODevice::WriteOnly);
        QDataStream dataS;
        dataS.setDevice(&file);
        dataS << plcRawData;
        file.close();

    foreach(AuTag* p,tagList)
    {
        delete p;
    }
    tagList.clear();
}

AuTag &AuPlc::operator[](QString tag)
{
    if(!tagList.contains(tag))
    {
        throw myName+"."+tag;
        throw THROWN_NOTAG;
    }
    return  *tagList[tag];
}

qint32 AuPlc::loadList(QString txt)
{
    qint32 count=0;
    qint32 wCount=0;

    QFile fList(txt);
    if(fList.open(QIODevice::ReadOnly))
    {
        while(!fList.atEnd())
        {
            int pos=fList.pos();
            QString line=fList.readLine().trimmed();
            if(line.size()>0)
            {
                QStringList tag=line.split('\t');
                // qDebug() << tag;
                if(tag[2]=="MX")
                {
                    fList.seek(pos);
                    wCount=loadMx(fList,wCount,tag[1].split('.')[0].toInt()/2);
                }
                else
                {
                    tagList[tag[0]]=new AuTag(plcRawData,tag,wCount,myName); // створити тег

                    if(tag[2]=="Bool"){ // порахувати індекс наступного тега
                        ++wCount;
                    }
                    else if(tag[2]=="Integer") {
                        ++wCount;
                    }
                    else if(tag[2]=="Real") {
                        wCount+=2;
                    }
                    else if(tag[2]=="Long") {
                        wCount+=2;
                    }
                    else if(tag[2]=="Timer") {
                        wCount+=2;
                    }
                    else if(tag[2]=="EBOOL") {
                        ++wCount;
                    }
                    else if(tag[2]=="QX") {
                        ++wCount;
                    }
                    else if(tag[2]=="IX") {
                        ++wCount;
                    }
                    else {
                        ++wCount;
                    }
                    ++count;
                }
            }
        }

        // тут треба вписати код який завантажить дані із диску.
        QFile file;
        file.setFileName(QString("%1/.auscada/%2.plc").arg(QDir::homePath()).arg(iX,4,16,QChar('0')));
        if(file.exists())
        {
            file.open(QIODevice::ReadOnly);
            QDataStream dataS;
            dataS.setDevice(&file);
            dataS >> plcRawData;
            file.close();

            if(plcRawData.size()<wCount) // якщо потрібно виділити більше даних ніж записано в файл
            { // значить файл застарів, відкинути його
                plcRawData.clear();
                plcRawData.fill(0,wCount);
            }

        }
        else {
            plcRawData.clear();
            plcRawData.fill(0,wCount);
        }

        // qDebug() << "plcRawData" << plcRawData.size();
        fList.close();

    }

    // встановити шкали зв'язаних тегів
    foreach(AuTag *p,tagList)
    {
        if(p->scaleType()==tagScale::LINK)
        {
            if(tagList.keys().contains(p->linkTagName()))
            {
                p->setLinkTag(tagList[p->linkTagName()]);
            }
            else {
                qDebug() << "Scale tag not gound" << p->name() << "->" << p->linkTagName();
            }
        }
    }
    return  count;

}

qint32 AuPlc::loadMx(QFile &fList, qint32 wCount,qint32 addr)
{
    int pos;
    int a=0;// ,b=0; // %MXa.b це для адреси

    while(!fList.atEnd())
    {
        pos=fList.pos();
        QString line=fList.readLine().trimmed();
        if(line.size()>0)
        {
            QStringList tag=line.split('\t');
            if(tag[2]!="MX")
            {
                fList.seek(pos);// перемотати назад
                break;
            }
            //інакше треба його розбирати
            a=tag[1].split('.')[0].toInt()/2;
            // рахувати слова

            // реєструвати тег
            if(a!=addr)
                ++wCount;
            tagList[tag[0]]=new AuTag(plcRawData,tag,wCount,myName); // створити тег


            addr=a; //
        }

    }
    return  ++wCount;
}

QStringList AuPlc::tagsList()
{
    return tagList.keys();
}

QString &AuPlc::name()
{
    return myName;

}

QString &AuPlc::fileList()
{
    return myFileList;
}

quint16 AuPlc::index()
{
    return iX;
}


void AuPlc::setData() // просто транслювати сигнал далі
{
    emit updateData();
    //qDebug() << objectName();
}

bool AuPlc::isOk()
{
    return true; // поки що заглушка
    // можливо тут ще додам якийсб сигнал при втраті чи встановленні зв'язку
}

