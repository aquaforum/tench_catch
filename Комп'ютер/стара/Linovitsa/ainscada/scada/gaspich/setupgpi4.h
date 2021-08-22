#ifndef SETUPGPI4_H
#define SETUPGPI4_H

#include <QWidget>
#include <QDialog>

class IoDev;


namespace Ui {
class SetupGpi4;
}



class SetupGpi4 : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetupGpi4(IoDev &source, QWidget *parent = 0);
    ~SetupGpi4();
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void slotAccept();
    void slotChangeMass(double);

private:

    IoDev &src;
    bool bAccess;
    Ui::SetupGpi4 *ui;

};

#endif // SETUPGPI4_H
