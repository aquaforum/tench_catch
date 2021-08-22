/*
 * Цей клас слугує сховищем даних
 * в конструктор передається ім'я файла із списками тегів
 * також є пустий контсруктор для створення фіктивного пустого сховища.
 * подумати над необхідністю існування такого пустого сховища.....
 *
 * Опис значної частини функціоналу розміщено у файлі xudpclient.cpp
 *
*/

#include "xudpstore.h"
#include <QFile>
#include <QStringList>
#include <QString>

#include <QDebug>

#define BASE 4000.0 // базове значення програми 0-100% = 0-BASE

xUdpStore::xUdpStore()
{

}

xUdpStore::xUdpStore(QString fileName)
{
    size_t index=0;
    QFile file(fileName);
    if(file.exists()) // перевірити чи такий файл є в наявності
    {
        file.open(QIODevice::ReadOnly); // відкрити файл, бажано б перевірити чи відкрився.....
        QStringList fields; // поля із файла
        for(;!file.atEnd();) // крутимся в циклі поки не кінець файла
        {

            fields.clear();
            foreach (QString f, file.readLine().trimmed().split('\t')) { // прочитати один рядок та розібрати на частини
                fields << f; // а частини записати сюди
            }
            tags[fields[0]].address=fields[1].toInt();

            if(fields.size()>4) // якщо вистачає даних
            {
                tags[fields[0]].index=index;
                // розрахувати новий індекс
                if(fields[2]=="Real" || fields[2]=="Long" || fields[2]=="Timer") // якщо двобайтове поле
                    index+=2;
                else // інакше поле однобайтове Bool EBOOL(для шнайдера) Integer
                    index++;

                // шкали
                // поки-що просто прочитати з файла
                if(fields[5].size()>0) // якщо там щось є
                {
                    if(fields[5].trimmed().split(' ').size()>1)
                    {
                        tags[fields[0]].zero=fields[5].trimmed().split(' ')[0].toDouble();
                        tags[fields[0]].full=fields[5].trimmed().split(' ')[1].toDouble();
                        //qInfo() << fields[0] << scale[fields[0]].zero << scale[fields[0]].full;
                    }
                }
                else // шкали по замовчанню, може це й неправильно, а може й правильно, це різко спростить налаштування, можна буде викинкти все зайве
                {
                    tags[fields[0]].zero=0.0;
                    tags[fields[0]].full=100.0;
                }
                // підписи
                if(fields.size()>6) // перевірити наявність підписів.
                {
                    tags[fields[0]].text=fields[6].trimmed();
                }
            }
        }
        file.close();
    }

}

// це повертає шкальоване значення тега
double xUdpStore::operator[](QString tagName) const
{
    double value=0.0;
    union fPack
    {
        float v;
        struct
        {
            qint16 hi;
            qint16 lo;
        };

    }pack;

    if(tags.contains(tagName)) // перевірити чи є такий тег
    {
        //value=((float*)(data_raw.data()+tags[tagName].index))[0]; // дістати нешкальоване значення
        pack.hi=data_raw[tags[tagName].index];
        pack.lo=data_raw[tags[tagName].index+1];
        value=pack.v;

        if(tags.contains(tagName)) // це місце треба переписати, інакше всі теги будуть шкалюватися
            value= value*(tags[tagName].full-tags[tagName].zero)/BASE + tags[tagName].zero;  // перерахувати значення
    }
        return value;
}

// це повертає реальні значення
// у всіх наступних функціях треба перевірити тип тега і при необхідності привести до потрібного
// в такому випадку повертати посилання погана ідея.....
qint16 &xUdpStore::value16(QString tagName) // базовий
{
    if(tags.contains(tagName)) // перевірити чи є такий тег
    {
        return data_raw[tags[tagName].index];
    }
    else
        return sZero;
}

qint32 &xUdpStore::value32(QString tagName)
{
    if(tags.contains(tagName)) // перевірити чи є такий тег
        return ((qint32*)(data_raw.data()+tags[tagName].index))[0];
    else
        return iZero;
}

float xUdpStore::valueFl(QString tagName) // нешкальоване значення
{
    union fPack
    {
        float v;
        struct
        {
            qint16 hi;
            qint16 lo;
        };

    }pack;

    pack.v=.0;


    if(tags.contains(tagName)) // перевірити чи є такий тег
    {
        //return ((float*)(data_raw.data()+tags[tagName].index))[0]; // для початку для перевірки верну нешкальовані значення
        pack.hi=data_raw[tags[tagName].index];
        pack.lo=data_raw[tags[tagName].index+1];
    }

        return pack.v;
}


