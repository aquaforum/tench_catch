#include "aulibdefs.h"
#include "autag.h"
#include <QStringList>
#include <QVector>
#include <QFile>

#include <QDebug>
#include <QSettings>

// !!! Це дуже важливий для розрахунку шкал дефайн, він економить один дабл, але якщо математику змінити то myB прийдеться повертати
#define myB myZero


// Тег	Адреса	Тип	Читати	Історія		Текст
// ft << "Integer" <<  "Bool"<<  "Real" << "Timer" <<  "Long" << "EBOOL"
/*
if(tag[2]=="Integer"){

}
else if(tag[2]=="Bool") {

}
else if(tag[2]=="Real") {

}
else if(tag[2]=="Long") {

}
else if(tag[2]=="Timer") {

}
else if(tag[2]=="EBOOL") {

}
else {

}
*/

union pack // така збочена конструкція потрібна для того щоб це потім працювало на ARM
{
    float var;
    qint32 nVar;
    struct _Q {
        qint16 hi;
        qint16 lo;
    } q;
};


AuTag::AuTag(QVector<qint16> &plcData, QStringList &tagList, qint32 index, QString &plcName): plcRawData(plcData), myPlcName(plcName),
    myBase(4000.0), // за замовчанням помоєму
    myDecimal(0),myK(1.0)
{
    //setTag(tagLine,index);
    loadTag(tagList,index);
}


void AuTag::loadTag(QStringList tagLine, qint32 index)
{
    myName = tagLine[0];
    myIndex = index; // індекс в data_raw

    if(tagLine[2]!="MX")
        myAddress = static_cast<quint16>(tagLine[1].toInt()); // адрес MODBUS
    else
    {
        int a=tagLine[1].splitRef('.')[0].toInt();
        int b=tagLine[1].splitRef('.')[1].toInt();
        myAddress = static_cast<quint16>(a/2);
        myMxMask = 1 << static_cast<quint16>(a%2*8+b);
    }

    if(tagLine[2]=="Integer"){
        myType=tagType::INT;
    }
    else if(tagLine[2]=="Bool") {
        myType=tagType::BOOL;
    }
    else if(tagLine[2]=="Real") {
        myType=tagType::REAL;
    }
    else if(tagLine[2]=="Long") {
        myType=tagType::LONG;
    }
    else if(tagLine[2]=="Timer") {
        myType=tagType::TIMER;
    }
    else if(tagLine[2]=="EBOOL") {
        myType=tagType::EBOOL;
        myAddress= -myAddress; // !!!!!!!!!!!!!!!!!! можливо не самий гарний варіант але для єбула поміняти знак і по цьому сформувати запит на ппередачу EBOOL по мінусовому адресу
    }
    else if(tagLine[2]=="QX") {
        myType=tagType::QX;
        myAddress= -myAddress; // !!!!!!!!!!!!!!!!!! можливо не самий гарний варіант але для єбула поміняти знак і по цьому сформувати запит на ппередачу EBOOL по мінусовому адресу
    }
    else if(tagLine[2]=="IX") {
        myType=tagType::IX;   
    }
    else if(tagLine[2]=="MX") {
        myType=tagType::MX;
    }
    else {
        myType=tagType::DEF;
    }

    myLogging=tagLine[4].toInt()!=0; // писати історію в базу


    QSettings set; // це то збереже шкали, але всі вони локальні, треба їх якось синхронізувати по мережі
    QString scale=tagLine[5];
    if(scale=="-")
    {
        myScale=tagScale::NONE;
        myZero=0.0; // на всякий випадок
        myFull=100.0;

    }
    else if(scale=="+")
    {
        myZero = set.value(QString("%1/%2/%3").arg(myPlcName).arg("Zero").arg(myName),0.0).toDouble(); // шкала нуль
        myFull = set.value(QString("%1/%2/%3").arg(myPlcName).arg("Full").arg(myName),100.0).toDouble(); // шкала розмах
        myScale=tagScale::SET;
    }
    else if(scale.split(' ').size()==2)
    {
        myZero=scale.split(' ')[0].toDouble();
        myFull=scale.split(' ')[1].toDouble();
        myScale=tagScale::CONST;
    }
    else if(scale=="") // якщо там пусто то автоматично думаємо що 0-100
    {
        myZero=0.0; // на всякий випадок
        myFull=100.0;
        myScale=tagScale::CONST;
    }
    else {
        // тут треба переві
        myScale=tagScale::LINK;
        myLinkTagName=scale;
    }
    myDecimal=set.value(QString("%1/%2/%3").arg(myPlcName).arg("Decimal").arg(myName),0.0).toInt();
    myBase = set.value(QString("%1/%2/%3").arg(myPlcName).arg("Base").arg(myName),4000.0).toDouble(); // шкала розмах
    lineCalc(); // розрахувати параметри

    // qDebug() << tagLine;
    if(tagLine.size()>6)
        myText = tagLine[6]; // опис тега
    if(myText=="")
        myText=myName;

}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// УВАГА для змінних типу Timer треба автоматично перводити із мілісекунд в секунди

double AuTag::operator=(double value) // отримання даних від контролера та передача їх далі
{
    union pack p;

    // перед тим як все це робити треба ще глянути на чи не шкалюється цей тег ?

    double lVal;

    switch (myScale) {
    case tagScale::SET:
    case tagScale::CONST:
        lVal=(value-myB)/myK; //перешкальювати тег
        break;
    case tagScale::LINK:
        lVal=(value-myLinkTag->zero())/(myLinkTag->full()-myLinkTag->zero())*myLinkTag->baseScale(); //перешкальювати тег
        break;
    case tagScale::NONE:
        lVal=value;
    }


    switch (myType) {
    case tagType::REAL:
        p.var=static_cast<float>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=static_cast<qint16>(lVal);
        break;
    case tagType::TIMER:
        lVal*=1000.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!
        // fall through
    case tagType::LONG:
        p.nVar=static_cast<qint32>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        plcRawData[myIndex]= value==0.0?0:-1;
        break;
    case tagType::MX:
        if(value==0)
            plcRawData[myIndex] &=~myMxMask;
        else
            plcRawData[myIndex] |=myMxMask;

        break;

    }


    // відправляємо
    QVector<qint16> data;
    switch (myType) {
    case tagType::DEF: // відправляємо один байт
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        data << static_cast<qint16>(lVal);
        break;
    case tagType::LONG:
    case tagType::TIMER:
    case tagType::REAL:
        data << p.q.hi << p.q.lo;
        break;
    case tagType::MX:
        data << plcRawData[myIndex] ;
        break;

    }

    if(myType!=tagType::IX)
    {
        emit sendData(myAddress,data);
        emit sendData(myName,myAddress,data);
    }

    return value;
}

int AuTag::operator=(int value)
{
    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 i32;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;

    // перевірити шкали і перешкалювати
    double lVal;
    switch (myScale) {
    case tagScale::SET:
    case tagScale::CONST:
        lVal=(static_cast<double>(value)-myB)/myK; //перешкальювати тег
        break;
    case tagScale::LINK:
        lVal=(static_cast<double>(value)-myLinkTag->zero())/(myLinkTag->full()-myLinkTag->zero())*myLinkTag->baseScale(); //перешкальювати тег
        break;
    case tagScale::NONE:
    default:
        lVal=value;
    }

    // а далі як у дабла, див. вище

    switch (myType) { // перебрати тип тега по факту
    case tagType::REAL:
        p.var=static_cast<float>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=static_cast<qint16>(lVal);
        break;
    case tagType::TIMER:
        lVal*=1000.0;
        // fall through
    case tagType::LONG:
        p.i32=static_cast<qint32>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        plcRawData[myIndex]= value==0.0?0:-1;
        break;
    case tagType::MX:
        if(value==0)
            plcRawData[myIndex] &=~myMxMask;
        else
            plcRawData[myIndex] |=myMxMask;
        break;
    }

    // відправляємо
    QVector<qint16> data;
    switch (myType) {
    case tagType::DEF: // відправляємо один байт
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        data << static_cast<qint16>(lVal);
        break;
    case tagType::LONG:
    case tagType::TIMER:
    case tagType::REAL:
        data << p.q.hi << p.q.lo;
        break;
    case tagType::MX:
        data << plcRawData[myIndex] ;
        break;

    }
    if(myType!=tagType::IX)
    {
        emit sendData(myAddress,data);
        emit sendData(myName,myAddress,data);
    }



    return value;
}


qint16 AuTag::operator=(qint16 value)
{
    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 i32;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;

    // перевірити шкали і перешкалювати
    double lVal;
    switch (myScale) {
    case tagScale::SET:
    case tagScale::CONST:
        lVal=(static_cast<double>(value)-myB)/myK; //перешкальювати тег
        break;
    case tagScale::LINK:
        lVal=(static_cast<double>(value)-myLinkTag->zero())/(myLinkTag->full()-myLinkTag->zero())*myLinkTag->baseScale(); //перешкальювати тег
        break;
    case tagScale::NONE:
    default:
        lVal=value;
    }

    // а далі як у дабла, див. вище

    switch (myType) { // перебрати тип тега по факту
    case tagType::REAL:
        p.var=static_cast<float>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=static_cast<qint16>(lVal);
        break;
    case tagType::TIMER:
        lVal*=1000.0;
        // fall through
    case tagType::LONG:
        p.i32=static_cast<qint32>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        plcRawData[myIndex]= value==0.0?0:-1;
        break;
    case tagType::MX:
        if(value==0)
            plcRawData[myIndex] &=~myMxMask;
        else
            plcRawData[myIndex] |=myMxMask;
        break;


    }

    // відправляємо
    QVector<qint16> data;
    switch (myType) {
    case tagType::DEF: // відправляємо один байт
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        data << static_cast<qint16>(lVal);
        break;
    case tagType::LONG:
    case tagType::TIMER:
    case tagType::REAL:
        data << p.q.hi << p.q.lo;
        break;
    case tagType::MX:
        data << plcRawData[myIndex] ;
        break;

    }
    if(myType!=tagType::IX)
    {
        emit sendData(myAddress,data);
        emit sendData(myName,myAddress,data);
    }

    return value;
}


bool AuTag::operator=(bool value)
{
    // а з булом все те саме що й з іншими, тільки не перевіряється шкала
    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 i32;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;

    // перевірити шкали і перешкалювати
    /*
    double lVal;
    switch (myScale) {
    case tagScale::SET:
    case tagScale::CONST:
        lVal=(static_cast<double>(value)-myZero)/(myFull-myZero)*myBase; //перешкальювати тег
        break;
    case tagScale::LINK:
        lVal=(static_cast<double>(value)-myLinkTag->zero())/(myLinkTag->full()-myLinkTag->zero())*myLinkTag->baseScale(); //перешкальювати тег
        break;
    case tagScale::NONE:
        lVal=value;
    }
*/
    // а далі як у дабла, див. вище

    switch (myType) { // перебрати тип тега по факту
    case tagType::REAL:
        p.var=value?-1.0:0.0; // це може бути проблемою
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=value?-1:0;
        break;
    case tagType::LONG:
    case tagType::TIMER:
        p.i32=value?-1:0;
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::MX:
        if(value)
            plcRawData[myIndex] |=myMxMask;
        else
            plcRawData[myIndex] &=~myMxMask;
        break;


    }

    // відправляємо
    QVector<qint16> data;
    switch (myType) {
    case tagType::DEF: // відправляємо один байт
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        data << qint16(value?-1:0);
        break;
    case tagType::LONG:
    case tagType::TIMER:
    case tagType::REAL:
        data << p.q.hi << p.q.lo;
        break;
    case tagType::MX:
        data << plcRawData[myIndex] ;
        break;

    }

    if(myType!=tagType::IX)
    {
        emit sendData(myAddress,data);
        emit sendData(myName,myAddress,data);
    }


    return value;
}

AuTag::operator double() const
{
    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 nVar;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;


//    qDebug() << myName << myIndex << myType;
    switch (myType) {
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        p.var=static_cast<float>(plcRawData[myIndex]);
        break;
    case tagType::TIMER:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar)/1000.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!!
        break;

    case tagType::LONG:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar);
        break;
    case tagType::REAL:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        break;
    case tagType::MX:
        if(plcRawData[myIndex]&myMxMask)
            p.var=1.0;
        else
            p.var=0.0;
        break;

    default:
        p.var=0.0;
    }


     //  перешкалювати
    double result=0.0;// тут буде шкальований результат
    switch (myScale) {
    case tagScale::CONST:
    case tagScale::SET:
        //result=(static_cast<double>(p.var)/myBase*(myFull-myZero))+myZero;
        result=(static_cast<double>(p.var)*myK+myB);
        break;
    case tagScale::LINK:
        result=(static_cast<double>(p.var)/myLinkTag->baseScale()*(myLinkTag->full()-myLinkTag->zero()))+myLinkTag->zero();
        break;
    default:
    case tagScale::NONE:
        result=(static_cast<double>(p.var)); // повернути нешкальованим
        break;
    }

    return result;
}

AuTag::operator int() const
{
    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 nVar;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;


//    qDebug() << myName << myIndex << myType;
    switch (myType) {
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        p.var=static_cast<float>(plcRawData[myIndex]);
        break;
    case tagType::TIMER:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar)/1000.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!!
        break;

    case tagType::LONG:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar);
        break;
    case tagType::REAL:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        break;
    case tagType::MX:
        if(plcRawData[myIndex]&myMxMask)
            p.var=1.0;
        else
            p.var=0.0;
        break;

    default:
        p.var=0.0;
    }


     //  перешкалювати
    double result=0.0;// тут буде шкальований результат
    switch (myScale) {
    case tagScale::CONST:
    case tagScale::SET:
        //result=(static_cast<double>(p.var)/myBase*(myFull-myZero))+myZero;
        result=(static_cast<double>(p.var)*myK)+myB;
        break;
    case tagScale::LINK:
        result=(static_cast<double>(p.var)/myLinkTag->baseScale()*(myLinkTag->full()-myLinkTag->zero()))+myLinkTag->zero();
        break;
    case tagScale::NONE:
    default:
        result=(static_cast<double>(p.var)); // повернути нешкальованим
        break;
    }

    return static_cast<int>(result);
}


AuTag::operator qint16() const
{
    // це зкопійовано із метода вище і виправлена тільки стрічка з ретурн
    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 nVar;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;


//    qDebug() << myName << myIndex << myType;
    switch (myType) {
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        p.var=static_cast<float>(plcRawData[myIndex]);
        break;
    case tagType::TIMER:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar)/1000.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!!
        break;

    case tagType::LONG:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar);
        break;
    case tagType::REAL:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        break;
    case tagType::MX:
        if(plcRawData[myIndex]&myMxMask)
            p.var=1.0;
        else
            p.var=0.0;
        break;

    default:
        p.var=0.0;
    }


     //  перешкалювати
    double result=0.0;// тут буде шкальований результат
    switch (myScale) {
    case tagScale::CONST:
    case tagScale::SET:
        //result=(static_cast<double>(p.var)/myBase*(myFull-myZero))+myZero;
        result=(static_cast<double>(p.var)*myK)+myB;
        break;
    case tagScale::LINK:
        result=(static_cast<double>(p.var)/myLinkTag->baseScale()*(myLinkTag->full()-myLinkTag->zero()))+myLinkTag->zero();
        break;
    case tagScale::NONE:
    default:
        result=(static_cast<double>(p.var)); // повернути нешкальованим
        break;
    }

    return static_cast<qint16>(result);
}


AuTag::operator bool() const
{
    bool result;
    if(myType==tagType::MX)
        result= (plcRawData[myIndex]&myMxMask) != 0;
    else
        result=plcRawData[myIndex]!=0; // можливо занадто просто, якщо натравити
    return result;
}

AuTag::operator QString() const
{
    // це все тупо зкопіровано з operator double()
    // треба б його розбити на окремі функції.....

    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 nVar;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;


//    qDebug() << myName << myIndex << myType;
    switch (myType) {
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        p.var=static_cast<float>(plcRawData[myIndex]);
        break;

    case tagType::TIMER:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar)/1000.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!!
        break;

    case tagType::LONG:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar);
        break;
    case tagType::REAL:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        break;
    case tagType::MX:
        if(plcRawData[myIndex]&myMxMask)
            p.var=1.0;
        else
            p.var=0.0;
        break;

    default:
        p.var=0.0;
    }


     //  перешкалювати
    double result=0.0;// тут буде шкальований результат
    int dec=myDecimal;
    switch (myScale) {
    case tagScale::CONST:
    case tagScale::SET:
        //result=(static_cast<double>(p.var)/myBase*(myFull-myZero))+myZero;
        result=(static_cast<double>(p.var)*myK+myB);
        break;
    case tagScale::LINK:
        result=(static_cast<double>(p.var)/myLinkTag->baseScale()*(myLinkTag->full()-myLinkTag->zero()))+myLinkTag->zero();
        dec=myLinkTag->deciaml();
        break;
    case tagScale::NONE:
    default:
        result=(static_cast<double>(p.var)); // повернути нешкальованим
        break;
    }

    return QString::number(result,'f',dec);

}

qint16 AuTag::value() // це не дороблено!!! треба доробити
{
    qint16 result = plcRawData[myIndex] ;
    if(myType==tagType::MX)
        result &= myMxMask;
    return result;
}


qint32 AuTag::value32()
{
    union pack p;

    //    qDebug() << myName << myIndex << myType;
        switch (myType) {
        case tagType::INT:
        case tagType::BOOL:
        case tagType::EBOOL:
        case tagType::QX:
        case tagType::IX:
            p.nVar=static_cast<qint32>(plcRawData[myIndex]);
            break;
        case tagType::LONG:
        case tagType::TIMER:
            p.q.hi=plcRawData[myIndex];
            p.q.lo=plcRawData[myIndex+1];
            break;
        case tagType::REAL:
            p.q.hi=plcRawData[myIndex];
            p.q.lo=plcRawData[myIndex+1];
            p.nVar=static_cast<qint32>(p.var);
            break;
        case tagType::MX:
            p.nVar=plcRawData[myIndex] & myMxMask;
            break;

        default:
            p.nVar=0.0;
        }

    return p.nVar;
}


double AuTag::valueReal()
{

    union // така збочена конструкція потрібна для того щоб це потім працювало на ARM
    {
        float var;
        qint32 nVar;
        struct _Q {
            qint16 hi;
            qint16 lo;
        } q;
    } p;

//    p.q.hi=plcRawData[myIndex];
//    p.q.lo=plcRawData[myIndex+1];

    switch (myType) {
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        p.var=static_cast<qint32>(plcRawData[myIndex]);
        break;
    case tagType::LONG:
    case tagType::TIMER:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<double>(p.nVar);
        break;
    case tagType::REAL:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        break;
    case tagType::MX:
        if(plcRawData[myIndex]&myMxMask)
            p.var=1.0;
        else
            p.var=0.0;
        break;

    default:
        p.var=0.0;
    }


    return p.var;

}

QString AuTag::valueText() // це теж повертається з поправкою на тип і якщо воно Real тоді і на myDecimal
{
    QString result="plug";
    return result; // поки-що заглушка
}


// мені таке не подобається, треба придумати кращий варіант
void AuTag::setValue(qint16 Value)
{
    union pack p;

    switch (myType) {
    case tagType::REAL:
        p.var=static_cast<float>(Value);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=Value;
        break;
    case tagType::LONG:
    case tagType::TIMER:
        p.nVar=static_cast<qint32>(Value);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        plcRawData[myIndex]= Value==0.0?0:-1;
        break;
    case tagType::MX:
        if(Value&mxMask())
            plcRawData[myIndex] |= mxMask();
        else
            plcRawData[myIndex] &= ~mxMask();
        break;

    }


    // відправляємо
    QVector<qint16> data;
    switch (myType) {
    case tagType::DEF: // відправляємо один байт
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        data << Value;
        break;
    case tagType::LONG:
    case tagType::TIMER:
    case tagType::REAL:
        data << p.q.hi << p.q.lo;
        break;
    case tagType::MX:
        data << plcRawData[myIndex];
        break;

    }

    if(myType!=tagType::IX)
    {
        emit sendData(myAddress,data);
        emit sendData(myName,myAddress,data);
    }
}

void AuTag::setValue(qint32 Value)
{
    union pack p;

    switch (myType) {
    case tagType::REAL:
        p.var=static_cast<float>(Value);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=static_cast<qint16>(Value);
        break;
    case tagType::LONG:
    case tagType::TIMER:
        p.nVar=Value;
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        plcRawData[myIndex]= Value==0.0?0:-1;
        break;
    case tagType::MX:
        if(Value&mxMask())
            plcRawData[myIndex] |= mxMask();
        else
            plcRawData[myIndex] &= ~mxMask();
        break;


    }


    // відправляємо
    QVector<qint16> data;
    switch (myType) {
    case tagType::DEF: // відправляємо один байт
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        data << static_cast<qint16>(Value);
        break;
    case tagType::LONG:
    case tagType::TIMER:
    case tagType::REAL:
        data << p.q.hi << p.q.lo;
        break;
    case tagType::MX:
        data << plcRawData[myIndex];
        break;

    }

    if(myType!=tagType::IX)
    {
        emit sendData(myAddress,data);
        emit sendData(myName,myAddress,data);
    }

}

void AuTag::setValue(double Value)
{
    union pack p;

    switch (myType) {
    case tagType::REAL:
        p.var=static_cast<float>(Value);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=static_cast<qint16>(Value);
        break;
    case tagType::LONG:
    case tagType::TIMER:
        p.nVar=static_cast<qint32>(Value);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        plcRawData[myIndex]= Value==0.0?0:-1;
        break;
    case tagType::MX:
        if(Value!=0.0)
            plcRawData[myIndex] |= mxMask();
        else
            plcRawData[myIndex] &= ~mxMask();
        break;

    }


    // відправляємо
    QVector<qint16> data;
    switch (myType) {
    case tagType::DEF: // відправляємо один байт
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
    case tagType::QX:
    case tagType::IX:
        data << static_cast<qint16>(Value);
        break;
    case tagType::LONG:
    case tagType::TIMER:
    case tagType::REAL:
        data << p.q.hi << p.q.lo;
        break;
    case tagType::MX:
        data << plcRawData[myIndex];

        break;

    }

    if(myType!=tagType::IX)
    {
        emit sendData(myAddress,data);
        emit sendData(myName,myAddress,data);
    }

}


void AuTag::setValues(QVector<qint16>& values)
{
    emit sendData(myAddress,values);// ????????????????????? тут може бути проблема з IX
    emit sendData(myName,myAddress,values);
}



double  AuTag::full()
{
    return myScale==tagScale::LINK?myLinkTag->full():myFull;
}

double  AuTag::zero()
{
    return myScale==tagScale::LINK?myLinkTag->zero():myZero;
}

double AuTag::k()
{
    return  myScale==tagScale::LINK? ((myLinkTag->full()- myLinkTag->zero())/myLinkTag->baseScale())
                                   :myK;
}

double AuTag::b()
{
    return myScale==tagScale::LINK?myLinkTag->zero():myZero;
}

double AuTag::baseScale()
{
    return myScale==tagScale::LINK?myLinkTag->baseScale():myBase;
}

// перевстановлення та збереження шкал.
// в перспективі, станція, яка міняє шкали розсилає їх броадкастом по всій мережі, але є шанс що хтось не отримає
// треба придумати гарний механізм синхронізації шкал по мережі.
void  AuTag::setFull(double v)
{
    myFull=v;
    QSettings set; // це то збереже шкали, але всі вони локальні, треба їх якось синхронізувати по мережі
     set.setValue(QString("%1/%2/%3").arg(myPlcName).arg("Full").arg(myName),myFull); // шкала нуль
     lineCalc(); // перерахувати коефіцієнти
}

void  AuTag::setZero(double v)
{
    myZero=v;
    QSettings set; // це то збереже шкали, але всі вони локальні, треба їх якось синхронізувати по мережі
    set.setValue(QString("%1/%2/%3").arg(myPlcName).arg("Zero").arg(myName),myZero); // шкала нуль
    lineCalc();
}

void AuTag::setLinkTag(AuTag* tag)
{
    myLinkTag=tag;
}

tagScale AuTag::scaleType()
{
    return  myScale;
}


QString AuTag::linkTagName()
{
    return  myLinkTagName;
}

QString &AuTag::name()
{
    return myName;
}


void AuTag::setData(int val)
{
    *this=val; // визиваємо перегружений метод, правда ж КРАСИВО!!!
}

void AuTag::setData(double val)
{
    *this=val;
}

void AuTag::setData(bool val)
{
    *this=val;
}

// тут все просто як валянок
qint32 AuTag::deciaml()
{
    return myScale==tagScale::LINK?myLinkTag->deciaml():myDecimal;
}

void AuTag::setDecimal(qint32 val)
{
    myDecimal=val;
    QSettings set; // це то збереже шкали, але всі вони локальні, треба їх якось синхронізувати по мережі
    set.setValue(QString("%1/%2/%3").arg(myPlcName).arg("Decimal").arg(myName),myDecimal); // шкала нуль

}

/*
union pack AuTag::dataUnPack() // код який буде використано повторно винісь в окрему функцію
{
    union pack p;

    p.nVar=0;

    switch (myType) {
    case tagType::INT:
    case tagType::BOOL:
    case tagType::EBOOL:
        p.var=static_cast<float>(plcRawData[myIndex]);
        break;
    case tagType::LONG:
    case tagType::TIMER:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        p.var=static_cast<float>(p.nVar);
        break;
    case tagType::REAL:
        p.q.hi=plcRawData[myIndex];
        p.q.lo=plcRawData[myIndex+1];
        break;
    default:
        p.var=0.0;
    }

    return p;
}


void AuTag::dataPack(union pack &p)
{
    switch (myType) {
    case tagType::REAL:
        p.var=static_cast<float>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;

        break;
    case tagType::INT:
    case tagType::DEF:
        plcRawData[myIndex]=static_cast<qint16>(lVal);
        break;
    case tagType::LONG:
    case tagType::TIMER:
        p.i32=static_cast<qint32>(lVal);
        plcRawData[myIndex]=p.q.hi;
        plcRawData[myIndex+1]=p.q.lo;
        break;
    case tagType::BOOL:
    case tagType::EBOOL:
        plcRawData[myIndex]= value==0.0?0:-1;
        break;

    }
}
*/

void AuTag::setBaseScale(double v)
{
    myBase=v;
    QSettings set; // це то збереже шкали, але всі вони локальні, треба їх якось синхронізувати по мережі
    set.setValue(QString("%1/%2/%3").arg(myPlcName).arg("Base").arg(myName),myBase); // шкала нуль
    lineCalc();

}


void AuTag::lineCalc()
{
    // цією схемою я економлю на розрахунках шкальованих значень дві операції - та / але втрачаю пам'яті один дабл
    // враховуючи що кожен сам собі рахує шкальовані значення а пам'ять сьогодні дешева таке рішення рахую виправданим.
    // myB=myZero;
    myK=(myFull-myZero)/myBase;

}

// арифметичн оператори
 double operator+ (AuTag & left, AuTag &right)
{
    return (double)left+(double)right;
}

 double operator+ (AuTag & left, double right)
{
    return (double)left+right;
}

 int operator+ (AuTag &left, int right)
{
    return (int)left+right;
}

//    friend  double operator- (AuTag&, AuTag&);
 double operator- (AuTag &left, AuTag &right)
{
        return (double)left-(double)right;
}

 double operator- (AuTag &left, double right)
{
        return (double)left-right;
}

 int operator- (AuTag &left, int right)
{
    return (int)left-right;
}

 double operator* (AuTag &left, AuTag &right)
{
    return (double)left*(double)right;
}

 double operator* (AuTag &left, double right)
{
    return (double)left*right;
}

 int operator* (AuTag &left, int right)
{
        return (int)left*right;
}


 double operator/ (AuTag &left, AuTag &right)
{
        return (double)left/(double)right;
}

 double operator/ (AuTag &left, double right)
{
        return (double)left/right;
}

 int operator/ (AuTag &left, int right)
{
        return (int)left/right;
}

 int operator% (AuTag &left, AuTag &right)
{
        return (int)left%(int)right;
}

 int operator% (AuTag &left, int right)
{
        return (int)left%right;
}

 bool operator== (AuTag &left, double right)
{
        return (double)left==right;
}

 bool operator== (AuTag &left, int right)
{
        return (int)left==right;
}

 bool operator!= (AuTag &left, double right)
{
        return (double)left!=right;
}

 bool operator!= (AuTag &left, int right)
{
        return (int)left!=right;
}

/*
double AuTag::operator+ (double)
{
    return  0.0;
}

double AuTag::operator- (double)
{
    return  0.0;

}


double AuTag::operator* (double)
{
    return  0.0;

}


double AuTag::operator/ (double)
{
    return  0.0;

}



int AuTag::operator+ (int)
{
    return  0;

}

int AuTag::operator- (int)
{
    return  0;

}

int AuTag::operator* (int)
{
    return  0;

}

int AuTag::operator/ (int)
{
    return  0;

}


double AuTag::operator+ (AuTag&)
{
    return  0.0;
}

double AuTag::operator- (AuTag&)
{
    return  0.0;

}

double AuTag::operator* (AuTag&)
{
    return  0.0;

}

double AuTag::operator/ (AuTag&)
{
    return  0.0;

}


bool AuTag::operator== (int)
{
    return  false;

}

bool AuTag::operator== (double)
{
    return  false;

}


bool AuTag::operator!= (int)
{
    return  false;

}

bool AuTag::operator!= (double)
{
    return  false;
}

*/
