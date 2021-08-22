#ifndef GPREPORT_H
#define GPREPORT_H

#include <QWidget>
class AuBase;

namespace Ui {
    class GpReport;
}

class GpReport : public QWidget {
    Q_OBJECT
public:
    GpReport(AuBase &source, QWidget *parent = 0);
    ~GpReport();

private slots:

       void slotCallCalendar();
       void slotChangeZm(int);
       void slotSqlRes(qint32 id, const QList<QStringList> &);
       void slotSqlError(QString);
       void slotSendQuery();

protected:
    void changeEvent(QEvent *);

private:
    Ui::GpReport *m_ui;
    AuBase &src;
    qint32 state; // якщо фалсе -відправили запит на список, якщо тру  - відправили запит на середні значення

    int count,maxId,first;
};

#endif // GPREPORT_H
