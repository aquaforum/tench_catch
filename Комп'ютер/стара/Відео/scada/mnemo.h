#ifndef __MNEMO_H__
#define __MNEMO_H__

#include <QWidget>
#include <QVector>

#include <vlc/vlc.h>

class TrendChart;
class IoNetClient;
class QCheckBox;
class QLineEdit;
class QStringList;
class QWidget;
class QProgressBar;
class QPushButton;
class QFrame;
class QLabel;

class Mnemo: public QWidget
{
    Q_OBJECT
public:
    Mnemo(QWidget *p=NULL);
    ~Mnemo();

private slots:
    void checkState();
    void slotSnapshoot();


private:
    QWidget *m_video;
    QVector<QFrame*>  vf;

    QVector<QLabel*> status;

    libvlc_instance_t *_vlcinstance;
    libvlc_media_player_t *_mp[4];
    libvlc_media_t *_m[4];

    void writeLog(QString);
    qint32 playing[4];

};

#endif
