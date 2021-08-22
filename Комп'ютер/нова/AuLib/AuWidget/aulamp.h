#ifndef AULAMP_H
#define AULAMP_H

#include <QWidget>
#include <QPixmap>

class AuLamp : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPixmap lampOff READ lampOff WRITE setLampOff) // NOTIFY lanmOffChanged
    Q_PROPERTY(QPixmap lampOn READ lampOn WRITE setLampOn) // NOTIFY lampOnChanged
    Q_PROPERTY(bool checked READ checked WRITE setChecked) // NOTIFY checkedChanged
    Q_PROPERTY(int lamp READ lamp WRITE setLamp) //  NOTIFY lampChanged


public:
    explicit AuLamp(QWidget *parent = nullptr);

public :
    QSize sizeHint() const;
    bool checked();
    void setChecked(bool);

    QPixmap lampOff();
    void setLampOff(QPixmap);

    QPixmap lampOn();
    void setLampOn(QPixmap);

    int lamp();
    void setLamp(int);

    void addLamp(QPixmap);

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);

private:
    int myIndex;
    QVector<QPixmap> myImage;

};

#endif // AULAMP_H
