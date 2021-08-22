#ifndef FILSCAM_H
#define FILSCAM_H

#include <QWidget>
#include <vlc/vlc.h>

QT_BEGIN_NAMESPACE
namespace Ui { class FilsCam; }
QT_END_NAMESPACE

class FilsCam : public QWidget
{
    Q_OBJECT

public:
    FilsCam(QWidget *parent = nullptr);
    ~FilsCam();
private slots:
    void checkState();

private:
    Ui::FilsCam *ui;

    libvlc_instance_t *_vlcinstance;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;
    qint32 playing;

};
#endif // FILSCAM_H
