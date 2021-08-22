#ifndef APSETUPCOMMON_H
#define APSETUPCOMMON_H

#include <QWidget>

namespace Ui {
class ApSetupCommon;
}

class AuBase;
class ApMainForm;

class ApSetupCommon : public QWidget
{
    Q_OBJECT

public:
    explicit ApSetupCommon(AuBase &base, ApMainForm *p, QWidget *parent = nullptr);
    ~ApSetupCommon();
private slots:
    void setValue(double);
    void setChecked(bool);


private:
    Ui::ApSetupCommon *ui;
    AuBase &src;
};

#endif // APSETUPCOMMON_H
