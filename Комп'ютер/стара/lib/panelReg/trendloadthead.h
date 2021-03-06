#ifndef TRENDLOADTHEAD_H
#define TRENDLOADTHEAD_H

#include <QThread>

class QString;
class QStringList;
class TrendChart;

class TrendLoadThead : public QThread
{
public:
    TrendLoadThead(TrendChart *trChart);

    void run();
    void setLen(int v) {len=v;}
    void setQuery(QString v) {sQuery=v;}


private:
    TrendChart *trChart;
    int len;
    QString sQuery;

    // це все діло можна заповнити при створенні об’єкта, далі просто запускати start()
    QString dbHost,dbBase,dbUser,dbPasswd;
    // QStringList fields;
};

#endif // TRENDLOADTHEAD_H
