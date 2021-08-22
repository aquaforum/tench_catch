#include <QWidget>
#include <QHash>
#include <QStack>

#include "ui_mainform.h"

class AuTrendView;

class AuBase;
class fabMnemo;


class mMainForm: public QWidget // ,public Ui::Form
{
    Q_OBJECT
public:
        mMainForm(AuBase &source,QWidget *p=NULL);
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
    void slotShowCentrif();
    void showOneMnemo1();

    void slotShowCfReport();
    void slotShowFabReport();
    void slotShowGaspich();
    void slotShowRou();
    void slotShowBp();
    void showBoiler2Mnemo();



private:
    
    AuBase &src; // вказівник на джерело даних
    Ui::Form *m_ui;
    QStack<QString> alertList;

    QHash<QObject*,int> hPos;

    AuTrendView *pTrw; // вказівник на вікно трендів

    QWidget *currMnemo;
    fabMnemo *fab;

    QVBoxLayout *currVbl;

    QObject *lastBn;

};


