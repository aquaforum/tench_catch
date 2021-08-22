#ifndef DLGMASSVIEW_H
#define DLGMASSVIEW_H

#include <QDialog>
#include <QVector>

class AuPlc;
class QLineEdit;

namespace Ui {
class DlgMassView;
}


class DlgMassView : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMassView(AuPlc &source, QWidget *parent = 0);
    ~DlgMassView();
private slots:

    void updateData();

private:
    Ui::DlgMassView *ui;
    AuPlc &src;

    QVector<QLineEdit*> leMass,leSum;
    QLineEdit *mass1,*mass2,*massSum;


};

#endif // DLGMASSVIEW_H
