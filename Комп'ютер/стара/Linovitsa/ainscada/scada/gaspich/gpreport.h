#ifndef GPREPORT_H
#define GPREPORT_H

#include <QWidget>

class QSqlQueryModel;

namespace Ui {
    class GpReport;
}

class GpReport : public QWidget {
    Q_OBJECT
public:
    GpReport(QWidget *parent = 0);
    ~GpReport();
public slots:

       void slotCallCalendar();
       void slotChangeZm(int);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GpReport *m_ui;
    QString repFld;
    QString agrFld;

    QSqlQueryModel *mRepModel,*mRepAgrModel;

};

#endif // GPREPORT_H
