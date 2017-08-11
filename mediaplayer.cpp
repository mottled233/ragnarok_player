#include "mediaplayer.h"
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include "mainwindow.h"

MusicWidget::MusicWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setGeometry(0,0,parent->width(),parent->height());
    QPalette palette =  this->palette();
    palette.setBrush(this->backgroundRole(),QBrush(QPixmap(":/timg.jpg")));
    this->setPalette(palette);
    this->setAutoFillBackground(true);


}

MediaPlayer::MediaPlayer(MainWindow *mainwindow,QWidget *videoarea,QWidget *musicarea, QStackedWidget *parent)
{
    this->parent = parent;
    this->videoarea = videoarea;
    this->musicarea = musicarea;
    this->videoWidget = new VideoWidget(this,0);

    this->setVideoOutput(this->videoWidget);
    this->musicWidget = new MusicWidget(musicarea);
    this->parent->setCurrentIndex(1);
    this->videoWidget->hide();
    this->mainwindow = mainwindow;

    this->videoWidget->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    connect(this,&MediaPlayer::stateChanged,this,&MediaPlayer::onStateChanged);
    connect(this,&MediaPlayer::positionChanged,this,&MediaPlayer::onPosChanged);
    connect(this,&MediaPlayer::durationChanged,this,&MediaPlayer::onDuraChanged);
}

void MediaPlayer::fastForward()
{
    if(duration()-position()>10000)
        this->setPosition(this->position()+5000);
}

void MediaPlayer::backForward()
{
    if(position()>5000)
        this->setPosition(this->position()-5000);
    else
        this->setPosition(0);
}

void MediaPlayer::onSpace()
{
    if(this->state()==QMediaPlayer::StoppedState){
        this->play();
    }else if(this->state()==QMediaPlayer::PlayingState){
        this->pause();
    }else if(this->state()==QMediaPlayer::PausedState){
        this->play();
    }
}

void MediaPlayer::onCurrentPlayChange(QString url, int pos, QString name)
{
    this->stop();

    bool isVideo = (url.indexOf(".wmv")!=-1)||(url.indexOf(".avi")!=-1)||(url.indexOf(".mp4")!=-1);
    qDebug()<<"inMedia:"<<isVideo;
    bool isLocal = url.indexOf("http")!=0;

    if(isVideo){
        this->videoWidget->setGeometry(this->mainwindow->x()+190,this->mainwindow->y()+48,parent->width(),parent->height());
        this->videoWidget->show();
        this->parent->setCurrentIndex(2);
        if(isLocal)
            this->setMedia(QMediaContent(QUrl::fromLocalFile(url)));
        else
            this->setMedia(QMediaContent(QUrl(url)));

//        if(!this->isVideoAvailable())
//        {
//            emit this->VideoUnavailable();
//            return;
//        }
    }else{
        this->videoWidget->hide();
        this->parent->setCurrentIndex(1);
        if(isLocal)
            this->setMedia(QMediaContent(QUrl::fromLocalFile(url)));
        else
            this->setMedia(QMediaContent(QUrl(url)));
//        if(!this->isAudioAvailable())
//        {
//                qDebug()<<"in Media";
//            emit this->AudioUnavailable();
//            return;
//        }
    }
    this->play();
}


void MediaPlayer::onStateChanged(QMediaPlayer::State state)
{
    this->mainwindow->changePlayButton(state);
}

void MediaPlayer::onPosChanged(qint64 pos)
{
    if(this->duration()-pos<50)
    {
        return;
    }
    this->mainwindow->onPosChanged(pos);
}

void MediaPlayer::onDuraChanged(qint64 pos)
{
    this->mainwindow->onDuraChanged(pos);
}

void MediaPlayer::hideVideo()
{
    this->videoWidget->hide();
}

VideoWidget::VideoWidget(MediaPlayer *media,QWidget *parent)
    : QVideoWidget(parent)
{
    this->mediaplayer = media;
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);

    setAttribute(Qt::WA_OpaquePaintEvent);
}

void VideoWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
        setFullScreen(false);
        event->accept();
    } else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) {
        setFullScreen(!isFullScreen());
        event->accept();
    } else if (event->key() == Qt::Key_Space) {
        this->mediaplayer->onSpace();
        event->accept();
    } else {
        QVideoWidget::keyPressEvent(event);
    }
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    setFullScreen(!isFullScreen());
    event->accept();
}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    QVideoWidget::mousePressEvent(event);
}
