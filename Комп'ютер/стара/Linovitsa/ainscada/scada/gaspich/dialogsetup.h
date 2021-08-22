#ifndef DIALOGSETUP_H
#define DIALOGSETUP_H

#include <QDialog>
#include <iodev.h>


namespace Ui {
class DialogSetup;
}

class DialogSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetup(IoDev &s, QWidget *parent = 0);
    ~DialogSetup();

private slots:
    void slotAccept();


private:
    bool bAccess;
    Ui::DialogSetup *ui;

    IoDev &src;


};

#endif // DIALOGSETUP_H
