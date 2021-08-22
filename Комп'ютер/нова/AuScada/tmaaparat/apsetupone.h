#ifndef APSETUPONE_H
#define APSETUPONE_H

#include <QWidget>

class AuPlc;

namespace Ui {
class ApSetupOne;
}

class ApSetupOne : public QWidget
{
    Q_OBJECT

public:
    explicit ApSetupOne(AuPlc &src, QWidget *parent = nullptr);
    ~ApSetupOne();

private slots:
    void slotSetValue(double);
    void slotSetTime(const QTime &);
    void slotSetBool(bool);

    void slotSetTempVak(bool);
    void slotSetDtMt(bool);


private:
    Ui::ApSetupOne *ui;
    AuPlc &plc;
};

#endif // APSETUPONE_H
