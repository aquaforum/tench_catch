#ifndef REPORWRMGR_H
#define REPORWRMGR_H

#include <QObject>


class FabReportWriter;
class QTimer;
class AuBase;

class ReporWrMgr: public QObject
{
    Q_OBJECT
public:
    ReporWrMgr(AuBase &base);

private slots:
    void deleteReportWriter();
    void runReport();
    void runTimer();

private:
    AuBase &src;
    FabReportWriter *writer;



};

#endif // REPORWRMGR_H
