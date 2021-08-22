#ifndef KEYBOARDWIN_H
#define KEYBOARDWIN_H

#include <QDialog>

namespace Ui {
    class KeyboardWin;
}
class QString;

class KeyboardWin : public QDialog
{
    Q_OBJECT

public:
    explicit KeyboardWin(double currentValue,double minValue,double maxValue, QWidget *parent = 0);
    ~KeyboardWin();
    double value() { return val;}

protected:
    void changeEvent(QEvent *e);
private slots:
    void digitClicked();
    void dotClicked();
    void signClicked();
    void bsClicked();
    void enterClicked();


private:
    Ui::KeyboardWin *ui;
    double val,min,max;
    QString sVal;

};

#endif // KEYBOARDWIN_H
