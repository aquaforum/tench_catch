#include <QCoreApplication>
#include "hhreport.h"

#include <IoNetClient.h>
#include <QSettings>
#include <QString>
#include <QStringList>

#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QCoreApplication::setOrganizationName("Lynovitza");
    QStringList hosts;
    bool cMode[5];
    QVector<IoNetClient*> srca;

    {
        QCoreApplication::setApplicationName("difuz");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();

        cMode[0]=false;

    }

    IoNetClient net_d(hosts[0]);
    net_d.setObjectName("difuz");
    net_d.setCmode(cMode[0]);


#ifdef __ALL_STATION__
    {
        QCoreApplication::setApplicationName("satur");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();

        cMode[1]=false;

    }

    {
        QCoreApplication::setApplicationName("viparka");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();

//#define __TEST__

//#ifdef __TEST__
//        cMode[2]=false;
//#else
        cMode[2]=true;
//#endif

    }

    {
        QCoreApplication::setApplicationName("aparat");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();

        cMode[3]=false;

    }


    {
        QCoreApplication::setApplicationName("centrif");
        QSettings s;
        hosts << s.value("/ioserv/hostname","localhost").toString();

        cMode[4]=false;

    }


    IoNetClient net_s(hosts[1]);
    net_s.setObjectName("satur");
    net_s.setCmode(cMode[1]);

    IoNetClient net_v(hosts[2]);
    net_v.setObjectName("viparka");
    net_v.setCmode(cMode[2]);

    IoNetClient net_a(hosts[3],8185);
    net_a.setObjectName("aparat");
    net_a.setCmode(cMode[3]);


    IoNetClient net_c(hosts[4],8185);
    net_c.setObjectName("centrif");
    net_c.setCmode(cMode[4]);


    srca << &net_d << &net_s << &net_v << &net_a << &net_c;
#else
    srca << &net_d ; //<< &net_s << &net_v << &net_a << &net_c;
#endif


    // qDebug() << srca.size();

    HhReport report(srca);

    return a.exec();
}
