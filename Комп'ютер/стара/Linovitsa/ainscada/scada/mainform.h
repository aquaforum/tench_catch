#include <QtGui>
#include <QHash>

#include "ui_mainform.h"

#include <trend.h>

class IoNetClient;
class MnemoF1s;
class MnemofTank;
class OneMnemo1;
class CfMnemo;
class Report;
class FabReport;
class RouMnemo;
class BigPumpsMon;
class xUdpClient;

class mMainForm: public QWidget // ,public Ui::Form
{
    Q_OBJECT
public:
        mMainForm(QVector<IoNetClient*> &source,QWidget *p=NULL);
	~mMainForm();
	
public slots:
    void timeOut();
    void about();
    void trRun();
    void showMe();            
    void setupParm();
    
    void slotAlert(QString); // це буде викликатися коли сервер пришле алерт
    void showAlert();
    void showBleding();
    void slotSetPos();
    void slotShowAparat();
    void slotShowCentrif();
    void showOneMnemo1();

    void slotShowCfReport();
    void slotShowFabReport();
    void slotShowGaspich();
    void slotShowRou();



private:
    
    QVector<IoNetClient*> &src; // вказівник на джерело даних
    QHash<QObject*,int> hPos;

    TrendWindow *pTrw; // вказівник на вікно трендів
    struct trendinfo tp;
    Ui::Form *m_ui;
    QStack<QString> alertList;

    MnemoF1s *m_f;
    MnemofTank *m_ft;

    OneMnemo1 *oM1;
     CfMnemo *cFm;
     QWidget *mRep;

     FabReport *mFRep;
    RouMnemo *rmn;
    BigPumpsMon *mBpm;

    xUdpClient *X;


};


