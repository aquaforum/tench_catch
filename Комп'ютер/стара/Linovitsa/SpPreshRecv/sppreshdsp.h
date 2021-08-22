#ifndef SPPRESHDSP_H
#define SPPRESHDSP_H

#include <QWidget>

class QUdpSocket;

namespace Ui {
class SpPreshDsp;
}

class SpPreshDsp : public QWidget
{
    Q_OBJECT

public:
    explicit SpPreshDsp(QWidget *parent = 0);
    ~SpPreshDsp();
private slots:
    void slotRecv();
    void slotSend(int);

private:
    Ui::SpPreshDsp *ui;

    QUdpSocket *sock;
};

#endif // SPPRESHDSP_H
