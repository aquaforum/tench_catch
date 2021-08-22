#ifndef DLGGVODACTRL_H
#define DLGGVODACTRL_H

#include <QDialog>

class IoDev;

namespace Ui {
    class dlgGVodaCtrl;
}

class dlgGVodaCtrl : public QDialog {
    Q_OBJECT
public:
    dlgGVodaCtrl(IoDev &source, QWidget *parent = 0);
    ~dlgGVodaCtrl();
public slots:
    void slotSetData(bool);
    void slotSetData(int);
    void slotSetData(double);
    void slotUpdateData();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dlgGVodaCtrl *ui;
    IoDev &src;

};

#endif // DLGVODACTRL_H
