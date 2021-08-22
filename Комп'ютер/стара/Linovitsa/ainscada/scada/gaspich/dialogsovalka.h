#ifndef DIALOGSOVALKA_H
#define DIALOGSOVALKA_H

#include <QDialog>


class IoDev;

namespace Ui {
class DialogSovalka;
}

class DialogSovalka : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSovalka(IoDev &source, QWidget *parent = 0);
    ~DialogSovalka();

public slots:
    void myAccept();

private:
    IoDev &src;
    Ui::DialogSovalka *ui;


};

#endif // DIALOGSOVALKA_H
