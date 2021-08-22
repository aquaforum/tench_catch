#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <QtGui>
#include <QDialog>

#include "trend.h"


class IoDev;
class IoNetClient;


class UHistorySelect: public QDialog
        //, public QWidget
{
Q_OBJECT
public:
    UHistorySelect(QVector<IoNetClient*>  &src,struct trendinfo *tp,QWidget *p=NULL,QString fileName=":/history.ui");
    UHistorySelect(IoNetClient  &source,struct trendinfo *tp,QWidget *p=NULL,QString fileName=":/history.ui") ;

    ~UHistorySelect() ;
    QString& getNameTrend() {return nameTrend;}

    //struct trendinfo* getTrendParam() { return &TrendParam ;}
private slots:
    void slotAccept();
    void slotConstruct();
    void slotStoreTabIndex(int);

protected:
    void changeEvent(QEvent *e);

private:
    QVector<IoNetClient *> s;
    void initUi(QString fileName);

    QString nameTrend;
    struct trendinfo *TrendParam;

};

#endif

