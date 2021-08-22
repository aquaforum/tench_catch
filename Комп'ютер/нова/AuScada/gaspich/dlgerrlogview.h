#ifndef DLGERRLOGVIEW_H
#define DLGERRLOGVIEW_H

#include <QDialog>

namespace Ui {
class DlgErrLogView;
}

class DlgErrLogView : public QDialog
{
    Q_OBJECT

public:
    explicit DlgErrLogView(QWidget *parent = 0);
    ~DlgErrLogView();

private:
    Ui::DlgErrLogView *ui;
};

#endif // DLGERRLOGVIEW_H
