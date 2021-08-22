#ifndef SPARAMDIALOG_H
#define SPARAMDIALOG_H

#include <QDialog>

class AuBase;
class QComboBox;

namespace Ui {
    class SParamDialog;
}

class SParamDialog : public QDialog {
    Q_OBJECT
public:
    SParamDialog(AuBase &source,QWidget *parent = 0);
    ~SParamDialog();



private:
    Ui::SParamDialog *m_ui;
    AuBase &src;

};

#endif // SPARAMDIALOG_H
