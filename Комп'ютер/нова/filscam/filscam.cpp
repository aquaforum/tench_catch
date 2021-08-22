#include "filscam.h"
#include "ui_filscam.h"
#include <QDebug>
#include <QTimer>

FilsCam::FilsCam(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FilsCam)
{
    ui->setupUi(this);

    //preparation of the vlc command
    const char * const vlc_args[] = {
        // "--verbose=2", //be much more verbose then normal for debugging purpose
        "--no-snapshot-preview",
        "--no-osd",
    "--no-audio",
        "--avcodec-hw=vaap"
    };

    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);  //tricky calculation of the char space used..

    // Create a media player playing environement
    _mp = libvlc_media_player_new (_vlcinstance);
    _m = libvlc_media_new_location(_vlcinstance,"rtsp://video:qwerty123@192.168.15.115/");
    libvlc_media_player_set_media (_mp, _m);
    libvlc_media_player_set_xwindow (_mp, this->winId() );
    libvlc_media_player_play(_mp);
    libvlc_audio_set_mute(_mp,0);
    playing=-1;

    QTimer *tmr=new QTimer(this);
    tmr->setInterval(10000);
    connect(tmr,&QTimer::timeout,this,&FilsCam::checkState);
    tmr->start();

}

FilsCam::~FilsCam()
{
    delete ui;
}


void FilsCam::checkState()
{
    if(libvlc_media_player_is_playing(_mp)!=0 )
    {
        if(playing==0)
            qDebug() << (QString("Camera #%1 playing").arg(libvlc_media_player_get_time(_mp)/1000));
        playing=-1;
    }
    else
    {
        libvlc_media_player_stop(_mp);
        libvlc_media_player_play(_mp);
    }
}
