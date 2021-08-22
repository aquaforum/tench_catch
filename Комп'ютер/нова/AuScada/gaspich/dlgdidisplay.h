#ifndef DLGDIDISPLAY_H
#define DLGDIDISPLAY_H

#include <QDialog>

namespace Ui {
class dlgDiDisplay;
}


#include <QVector>

class AuPlc;
class QCheckBox;


class dlgDiDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit dlgDiDisplay(AuPlc &source, QWidget *parent = 0);
    ~dlgDiDisplay();

private slots:
    void updateToolTip();
    void updateDataRaw();

private:
    AuPlc &src;
    Ui::dlgDiDisplay *ui;


    QVector<QCheckBox*> cb;

};

#endif // DLGDIDISPLAY_H
