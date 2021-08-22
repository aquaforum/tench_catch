#ifndef DLGBLEDINGSETUP_H
#define DLGBLEDINGSETUP_H

#include <QDialog>

#include <QTime>
class AuPlc;
class QTimeEdit;
class QComboBox;
class AuBase;

#include <QVector>

namespace Ui {
    class dlgBledingSetup;
}

class dlgBledingSetup : public QDialog {
    Q_OBJECT
public:
    dlgBledingSetup(AuBase  &source, QWidget *parent = 0);
    ~dlgBledingSetup();

public slots:
    //void slotUpdate(); // в цьому слоті буде поновлюватися інформація на картинці
    void slotSet(int);
    void slotSet(QTime);
    void updateList();

    void d_updateList();

    void d_slotSet(int);
    void d_slotSet(QTime);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dlgBledingSetup *ui;
    AuBase &src;

    QVector<QTimeEdit*> te_m,te_p;
    QVector<QComboBox*> cb_e,cb_k;

    QVector<QTimeEdit*> d_te_m,d_te_p;
    QVector<QComboBox*> d_cb_e,d_cb_k;

};

#endif // DLGBLEDINGSETUP_H
