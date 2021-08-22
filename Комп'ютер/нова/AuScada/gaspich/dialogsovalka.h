#ifndef DIALOGSOVALKA_H
#define DIALOGSOVALKA_H

#include <QDialog>


class AuPlc;

namespace Ui {
class DialogSovalka;
}

class DialogSovalka : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSovalka(AuPlc &source, QWidget *parent = 0);
    ~DialogSovalka();

public slots:
    void myAccept();

private:
    AuPlc &src;
    Ui::DialogSovalka *ui;


};

#endif // DIALOGSOVALKA_H
