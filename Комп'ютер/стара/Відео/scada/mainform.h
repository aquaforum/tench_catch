#include <QtGui>

#include "ui_mainform.h"

#include <trend.h>

class IoNetClient;

class mMainForm: public QWidget // ,public Ui::Form
{
    Q_OBJECT
public:
        mMainForm(QWidget *p=NULL);
	~mMainForm();
	
public slots:
    void timeOut();
    void about();
    void trRun();
    void showMe();            
    void setupParm();
    
//    void slotExit();
    void slotAlert(QString); // це буде викликатися коли сервер пришле алерт
    void showAlert();


private:
    

    Ui::Form *m_ui;
    QStack<QString> alertList;
};

