#ifndef DIALOGSETUP_H
#define DIALOGSETUP_H

#include <QDialog>
#include <aulib.h>


namespace Ui {
class DialogSetup;
}

class DialogSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetup(AuPlc &s, QWidget *parent = 0);
    ~DialogSetup();

private slots:
    void slotAccept();


private:
    bool bAccess;
    Ui::DialogSetup *ui;

    AuPlc &src;


};

#endif // DIALOGSETUP_H
