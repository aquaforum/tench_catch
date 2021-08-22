#ifndef APTRENDSELECT_H
#define APTRENDSELECT_H

#include <QWidget>

namespace Ui {
class ApTrendSelect;
}
class ApMainForm;
class AuBase;

class ApTrendSelect : public QWidget
{
    Q_OBJECT

public:
    explicit ApTrendSelect(AuBase &base,ApMainForm *p, QWidget *parent = nullptr);
    ~ApTrendSelect();
private slots:
    void slotChangeMnemo();

private:
    Ui::ApTrendSelect *ui;
    AuBase &src;
};

#endif // APTRENDSELECT_H
