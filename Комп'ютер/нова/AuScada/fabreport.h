#ifndef FABREPORT_H
#define REPORT_H

#include <QWidget>
#include <QVector>

class AuBase;

namespace Ui {
    class FabReport;
}

class AuBase;

class FabReport : public QWidget {
    Q_OBJECT
public:
    FabReport(AuBase &source,QWidget *parent = 0);
    ~FabReport();

protected:
    void changeEvent(QEvent *e);
private slots:
    void printReport();
    void saveReport();

    void slotSqlRes(qint32, const QList<QStringList> &);

    // це тимчасовий код для тестування звису звітів.
    //void slotRunReport();
    //void slotRunEmit();


private:
    AuBase &src;
    Ui::FabReport *m_ui;

    qint32 qIx;
    bool openDb;
};

#endif // REPORT_H
