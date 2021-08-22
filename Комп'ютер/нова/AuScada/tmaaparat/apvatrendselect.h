#ifndef APVATRENDSELECT_H
#define APVATRENDSELECT_H

#include <QWidget>

namespace Ui {
class ApVaTrendSelect;
}

class AuBase;

class ApVaTrendSelect : public QWidget
{
    Q_OBJECT

public:
    explicit ApVaTrendSelect(AuBase &base, QWidget *parent = nullptr);
    ~ApVaTrendSelect();
private slots:
    void slotShowTrend();

private:
    Ui::ApVaTrendSelect *ui;
    AuBase &src;
};

#endif // APVATRENDSELECT_H
