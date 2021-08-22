#ifndef MNEMOBOILER2_H
#define MNEMOBOILER2_H

#include <QWidget>
#include <QLabel>

class AuBase;
class AuPlc;
class QPaintEvent;
class AuLamp;
class TrendChart;

namespace Ui {
class MnemoBoiler2;
}

class MnemoBoiler2 : public QWidget
{
    Q_OBJECT

public:
    explicit MnemoBoiler2(AuBase &base, QWidget *parent = nullptr);
    ~MnemoBoiler2();
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void slotUpdate();
    void slotUpdateTrc();
    void slotCallReg();

private:
    Ui::MnemoBoiler2 *ui;
    AuBase &source;
    AuPlc &src;


    QList<QLabel*> label;
    QList<AuLamp*> lamp;

    QList<QIcon> bnPumpIcon;

    QVector<TrendChart*> trc;
    QStringList trcTags;

    QFrame* drawLegend(QString tags,TrendChart *trChar,AuPlc &plc);


};

#endif // MNEMOBOILER2_H
