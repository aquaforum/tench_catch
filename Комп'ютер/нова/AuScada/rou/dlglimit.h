#ifndef DLGLIMIT_H
#define DLGLIMIT_H

#include <QDialog>
#include <QHash>

class AuPlc;

namespace Ui {
class DlgLimit;
}

class DlgLimit : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLimit(AuPlc &source, QWidget *parent = 0);
    ~DlgLimit();

private slots:
    void myAccept();

private:
    AuPlc &src;
    Ui::DlgLimit *ui;

    QHash<QString,double> oldVal;


};

#endif // DLGLIMIT_H
