
#include "mnemo.h"


#include <QVBoxLayout>
#include <QTimer>

#include <QUiLoader>
#include <QFile>
#include <QGridLayout>

#include <QLineEdit>
#include <QCheckBox>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QDir>
#include <QDateEdit>
#include <QFile>



Mnemo::Mnemo( QWidget *p) : QWidget(p)
{
    QUiLoader *uil= new QUiLoader(this);
    QFile fileui(":/mnemo/mnemo.ui");
    m_video=uil->load(&fileui); // завантажити файл із мордою

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_video,0,0,1,1);
    resize(1850,1020);

    //preparation of the vlc command
    const char * const vlc_args[] = {
        // "--verbose=2", //be much more verbose then normal for debugging purpose
        "--no-snapshot-preview",
        "--no-osd",
	"--no-audio",
        "--avcodec-hw=vaap"
    };

    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);  //tricky calculation of the char space used

    QFrame *window[4]={m_video->findChild<QFrame*>("frame_1"),
                       m_video->findChild<QFrame*>("frame_2"),
                       m_video->findChild<QFrame*>("frame_3"),
                       m_video->findChild<QFrame*>("frame_4")
                      };

    status << m_video->findChild<QLabel*>("status_1")
           << m_video->findChild<QLabel*>("status_2")
           << m_video->findChild<QLabel*>("status_3")
           << m_video->findChild<QLabel*>("status_4");


    QStringList file;
    file << "rtsp://video:123@cam1"
         << "rtsp://video:123@cam2"
         << "rtsp://video:123@cam3"
         << "rtsp://video:123@cam4";

    for(size_t i=0;i<4;++i)
    {
    // Create a media player playing environement
    _mp[i] = libvlc_media_player_new (_vlcinstance);
    _m[i] = libvlc_media_new_location(_vlcinstance, file[i].toLatin1());
    libvlc_media_player_set_media (_mp[i], _m[i]);
    libvlc_media_player_set_xwindow (_mp[i], window[i]->winId() );
    libvlc_media_player_play(_mp[i]);
    libvlc_audio_set_mute(_mp[i],0);
    playing[i]=-1;
    }

    QTimer *tmr=new QTimer(this);
    tmr->setInterval(1000);
    connect(tmr,&QTimer::timeout,this,&Mnemo::checkState);
    tmr->start();

    QTimer *sShot = new QTimer(this);
    sShot->setInterval(120000);
    connect(sShot,&QTimer::timeout,this,&Mnemo::slotSnapshoot);
    sShot->start();

}

Mnemo::~Mnemo()
{

}

void Mnemo::checkState()
{
    for(size_t i=0;i<4;++i)
    {
            if(libvlc_media_player_is_playing(_mp[i])!=0 )
            {
                status[i]->setText(QString("Play %1").arg(libvlc_media_player_get_time(_mp[i])/1000));
                if(playing[i]==0)
                    writeLog(QString("Camera #%1 playing").arg(i+1));
                playing[i]=-1;
            }
            else
            {
                    if(status[i]->text()!="Stop")
                    {
                        playing[i]=30; // Виставити прапор втрати сигналу
                        writeLog( QString("camera #%1 stoping now!!!").arg(i+1));
                        status[i]->setText("Stop");
                    }

                    if(playing[i]==1)
                    {
                        writeLog(QString("Camera #%1 real stop").arg(i+1));
                        libvlc_media_player_stop(_mp[i]);
                    }

                    if(--playing[i]==0)
                    {

                        libvlc_media_player_play(_mp[i]);
                        writeLog( QString("Camera #%1 play now").arg(i+1));
                        playing[i]=30;
                    }


            }

    }


}

void Mnemo::slotSnapshoot()
{
    for(int i=0;i<4;++i)
    libvlc_video_take_snapshot(_mp[i],0,(QString("%1/video/%2/%3.jpg").arg(QDir::homePath()).arg(i+1).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmm"))).toLocal8Bit(),0,0);
}


void Mnemo::writeLog(QString v)
{
    QFile f("error.log");
    f.open(QIODevice::Append);
    f.write(QString("%1 - %2\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy")).arg(v).toUtf8());
    f.close();
}
