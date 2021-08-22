#ifndef APMAINFORM_H
#define APMAINFORM_H

#include <QWidget>

class AuBase;
class ApCommon;
class QVBoxLayout;
class AuTrendView;

namespace Ui {
class ApMainForm;
}

class ApMainForm : public QWidget
{
    Q_OBJECT

public:
    explicit ApMainForm(AuBase &base, QWidget *parent = nullptr);
    ~ApMainForm();
private slots:
    void trRun();
    void slotSetTime();
    void slotShowAlert(QString);

private slots:
    void addAlert(qint32,QString);
    void commitAlert(qint32);

public slots:
    void slotChangeMnemo();

private:
    Ui::ApMainForm *ui;
    AuBase &src;

    QWidget *currentMnemo;
    //QVBoxLayout *vbl;

    QHash<qint32,QString> ixAlert; // список активних аварій
    qint32 lastIx;
    QStringList lastAlert;

};

#endif // APMAINFORM_H
