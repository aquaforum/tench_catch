#ifndef FABREPORT_H
#define REPORT_H

#include <QWidget>
#include <QVector>

class QSqlQueryModel;
class IoNetClient;

namespace Ui {
    class FabReport;
}

class IoNetClient;

class FabReport : public QWidget {
    Q_OBJECT
public:
    FabReport(QVector<IoNetClient*> &source,QWidget *parent = 0);
    ~FabReport();

protected:
    void changeEvent(QEvent *e);
private slots:
    void printReport();
    void saveReport();


private:
    QVector<IoNetClient*> &src;
    Ui::FabReport *m_ui;
    QSqlQueryModel *mRepModel;




};

#endif // REPORT_H
