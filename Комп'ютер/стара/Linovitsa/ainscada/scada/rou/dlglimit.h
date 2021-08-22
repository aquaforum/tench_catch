#ifndef DLGLIMIT_H
#define DLGLIMIT_H

#include <QDialog>
#include <QHash>

class IoDev;

namespace Ui {
class DlgLimit;
}

class DlgLimit : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLimit(IoDev &source, QWidget *parent = 0);
    ~DlgLimit();

private slots:
    void myAccept();

private:
    IoDev &src;
    Ui::DlgLimit *ui;

    QHash<QString,double> oldVal;


};

#endif // DLGLIMIT_H
