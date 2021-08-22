#ifndef DLGTERMCTRL_H
#define DLGTERMCTRL_H

#include <QDialog>

class AuPlc;

namespace Ui {
    class DlgTermCtrl;
}

class DlgTermCtrl : public QDialog {
    Q_OBJECT
public:
    DlgTermCtrl(AuPlc &source,int Nom, QWidget *parent = 0);
    ~DlgTermCtrl();
public slots:
    void updateData();
    void slotSetVal(int);
    void slotSetAm(int);
    void slotSetV(int);

protected:
    void changeEvent(QEvent *e);

private:
    AuPlc &src;
    int n;
    Ui::DlgTermCtrl *ui;

};

#endif // DLGTERMCTRL_H
