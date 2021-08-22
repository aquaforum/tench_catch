#ifndef APZAGTRENDSELECT_H
#define APZAGTRENDSELECT_H

#include <QWidget>
class AuBase;

namespace Ui {
class ApZagTrendSelect;
}

class ApZagTrendSelect : public QWidget
{
    Q_OBJECT

public:
    explicit ApZagTrendSelect(AuBase &base, QWidget *parent = nullptr);
    ~ApZagTrendSelect();
private slots:
    void slotShowTrend();
private:
    Ui::ApZagTrendSelect *ui;
    AuBase &src;
};

#endif // APZAGTRENDSELECT_H
