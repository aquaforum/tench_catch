#ifndef DLGPTSCTRL_H
#define DLGPTSCTRL_H

#include <QDialog>

namespace Ui {
    class DlgPtsCtrl;
}

class IoDev;

class DlgPtsCtrl : public QDialog {
    Q_OBJECT
public:
    DlgPtsCtrl(IoDev &source, QWidget *parent = 0);
    ~DlgPtsCtrl();
public slots:
    void slotSetBool(int); // приймає сигнали від  комбобоксів
    void slotSetInt(int);
    void updateData();
    void slotOffCl(bool);

protected:
    void changeEvent(QEvent *e);

private:
    IoDev &src;
    Ui::DlgPtsCtrl *ui;

};

#endif // DLGPTSCTRL_H
