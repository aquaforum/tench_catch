#ifndef __AMVATER_H__
#define __AMVATER_H__

#include <QDialog>
#include <QStringList>

namespace Ui {
    class AmVater;
}

class IoDev;


class RAmVater: public QDialog
{
Q_OBJECT
public:
    RAmVater(IoDev &source,QStringList &tagList,QWidget *p=NULL);
    ~RAmVater();

    
private slots:
    void updateData(); // поновлення даних у вікні
    void slotSetAM(int v); // перемикач ручний-автомат
    void slotSetValue(double v); // встановлення параметрів
    void slotSetValue(int v); // встановлення парамертів
    void slotManY(bool v);  //ручне упарвління клапаном

private:
    Ui::AmVater *ui;

    IoDev &src;
    QStringList  &tags;
    

};

#endif
