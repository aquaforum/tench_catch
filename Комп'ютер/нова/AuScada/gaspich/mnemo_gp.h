#ifndef MNEMO_GP_H
#define MNEMO_GP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class mnemo_gp; }
QT_END_NAMESPACE

class mnemo_gp : public QWidget
{
    Q_OBJECT
public:
    explicit mnemo_gp(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::mnemo_gp *ui;

};

#endif // MNEMO_GP_H
