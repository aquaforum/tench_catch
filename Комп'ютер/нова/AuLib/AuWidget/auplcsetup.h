#ifndef AUPLCSETUP_H
#define AUPLCSETUP_H

#include <QWidget>

class AuPlcSetup : public QWidget
{
    Q_OBJECT
public:
    explicit AuPlcSetup(QString plcList=QStringLiteral(":/plc.list"),QWidget *parent = nullptr);
    ~AuPlcSetup();

private slots:
    void slotPlcSet(int);
    void slotSetIface(int);

signals:

private:
    bool restart;
};

#endif // AUPLCSETUP_H
