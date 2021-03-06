#ifndef DLGTERMCTRL_H
#define DLGTERMCTRL_H

#include <QDialog>

class IoDev;

namespace Ui {
    class DlgTermCtrl;
}

class DlgTermCtrl : public QDialog {
    Q_OBJECT
public:
    DlgTermCtrl(IoDev &source,int Nom, QWidget *parent = 0);
    ~DlgTermCtrl();
public slots:
    void updateData();
    void slotSetVal(int);
    void slotSetAm(int);
    void slotSetV(int);

protected:
    void changeEvent(QEvent *e);

private:
    IoDev &src;
    int n;
    Ui::DlgTermCtrl *ui;

};

#endif // DLGTERMCTRL_H
