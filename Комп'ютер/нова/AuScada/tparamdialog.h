#ifndef TPARAMDIALOG_H
#define TPARAMDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class TParamDialog;
}

class AuPlc;


class TParamDialog : public QDialog {
    Q_OBJECT
public:
    TParamDialog(AuPlc &src, QWidget *parent = 0);
    ~TParamDialog();

public slots:
    void myAccept(); // реакція на кнопку Ок

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TParamDialog *m_ui;
    AuPlc &s;
};

#endif // TPARAMDIALOG_H
