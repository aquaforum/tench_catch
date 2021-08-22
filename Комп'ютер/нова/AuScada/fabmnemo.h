#ifndef FABMNEMO_H
#define FABMNEMO_H

#include <QScrollArea>
class AuBase;

class fabMnemo : public QScrollArea
{
    Q_OBJECT
public:
    fabMnemo(AuBase &base,QWidget *parent);
    ~fabMnemo();
private slots:
    void slotStorePos(int);

private:
    QWidget *myMnemo;
};

#endif // FABMNEMO_H
