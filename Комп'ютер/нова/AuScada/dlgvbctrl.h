#ifndef DLGVBCTRL_H
#define DLGVBCTRL_H

#include <QDialog>

class AuPlc;

namespace Ui {
    class dlgVbCtrl;
}

class dlgVbCtrl : public QDialog {
    Q_OBJECT
public:
    dlgVbCtrl(AuPlc &source , int nValve = 0, QWidget *parent = 0);
    ~dlgVbCtrl();

public slots:
    void slotUpdate();
    void slotSet(int);
    void slotSet();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dlgVbCtrl *ui;
    AuPlc &src;
    int nI;
};

#endif // DLGVBCTRL_H
