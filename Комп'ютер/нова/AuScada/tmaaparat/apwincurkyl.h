#ifndef APWINCURKYL_H
#define APWINCURKYL_H

#include <QDialog>

class AuPlc;

namespace Ui {
class ApWinCurkyl;
}

class ApWinCurkyl : public QDialog
{
    Q_OBJECT

public:
    explicit ApWinCurkyl(AuPlc &plc,QWidget *parent = nullptr);
    ~ApWinCurkyl();
private slots:
    void slotUpdate();
    void slotSetAm();
    void slotSetVal(bool);
    void slotSetVal(int);

private:
    Ui::ApWinCurkyl *ui;
    AuPlc &myPlc;

    bool AM;
};

#endif // APWINCURKYL_H
