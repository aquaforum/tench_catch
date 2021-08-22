#ifndef APSGKL_H
#define APSGKL_H

#include <QDialog>
#include "tmaaparat/bitmask.h"

namespace Ui {
class ApSgKl;
}

class AuPlc;

class ApSgKl : public QDialog
{
    Q_OBJECT

public:
    explicit ApSgKl(AuPlc &plc, QStringList &tags,QWidget *parent = nullptr);
    ~ApSgKl();

private slots:
    void slotUpdate();
    void slotSetAm();
    void slotSetVal(bool);

private:
    Ui::ApSgKl *ui;
    AuPlc &myPlc;
    BitMask &plcBM;
    QStringList myTags;
    bool AM;

};

#endif // APSGKL_H
