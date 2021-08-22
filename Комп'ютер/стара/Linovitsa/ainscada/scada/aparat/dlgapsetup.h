#ifndef DLGAPSETUP_H
#define DLGAPSETUP_H

#include <QDialog>
#include <QVector>
class QSpinBox;
class QDoubleSpinBox;
class IoDev;

namespace Ui {
    class DlgApSetup;
}

class DlgApSetup : public QDialog {
    Q_OBJECT
public:
    DlgApSetup(IoDev &source,QWidget *parent = 0);
    ~DlgApSetup();

public slots:

    void slotSetValue(double);
    void slotSetValue(int);
    void slotSetTZ(int);

    void selectTeg(int v);
    void slotSet(QString);

protected:
    void changeEvent(QEvent *e);

private:
    IoDev &src;
    Ui::DlgApSetup *ui;
    QVector<QDoubleSpinBox*> dsb;

    QStringList tag_name; // список тегів, які треба налаштовувати

};

#endif // DLGAPSETUP_H
