#ifndef DLGAPONE_H
#define DLGAPONE_H

#include <QDialog>
#include <QHash>

class ApOne;
class AuPlc;
class QStringList;
class QString;


namespace Ui {
    class DlgApOne;
}

class DlgApOne : public QDialog {
    Q_OBJECT
public:
    DlgApOne(AuPlc &source, QWidget *parent = 0);
    ~DlgApOne();

public slots:
    void updateData();
    void slotCallSetup();
    void slotCommand();
    void slotSetKor(int v);
    void slotSendProd(int v);

protected:
    void changeEvent(QEvent *e);

private:
    AuPlc &src;
    Ui::DlgApOne *ui;
    ApOne *ap;

    QStringList state;
    qint16 lastKor;
    QHash<QString,qint16> cmd;

};

#endif // DLGAPONE_H
