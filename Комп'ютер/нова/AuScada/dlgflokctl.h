#ifndef DLGFLOKCTL_H
#define DLGFLOKCTL_H

#include <QDialog>
class AuPlc;


namespace Ui {
class DlgFlokCtl;
}

class DlgFlokCtl : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFlokCtl(AuPlc &plc, QStringList tagList,QWidget *parent = nullptr);
    ~DlgFlokCtl();
private slots:
    void updateData();
    void setAM(int);
    void setCtrl(double);
    void setSlC(int);
    void setKC(double);
    void setPump(int);

private:
    AuPlc &src;
    QStringList tags;
    Ui::DlgFlokCtl *ui;

};

#endif // DLGFLOKCTL_H
