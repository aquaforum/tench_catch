#ifndef CFPARAM1_H
#define CFPARAM1_H

#include <QDialog>

class AuPlc;
class QSpinBox;
class QDoubleSpinBox;

namespace Ui {
    class CfParam1;
}

class CfParam1 : public QDialog {
    Q_OBJECT
public:
    CfParam1(AuPlc &source, QWidget *parent = 0);
    ~CfParam1();
public slots:
      void slotAccept();
    void setZeroNc();
protected:
    void changeEvent(QEvent *e);

private:
    AuPlc &src;
    Ui::CfParam1 *m_ui;

    QVector<QSpinBox*> sb;
    QVector<QDoubleSpinBox*> dsb;
    QStringList sb_tag,dsb_tag;


};

#endif // CFPARAM_H
