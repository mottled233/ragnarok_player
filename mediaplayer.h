#ifndef MEDIAPLAYER
#define MEDIAPLAYER
#include <QObject>
#include <QMediaPlayer>
#include "medialist.h"
#include <QVideoWidget>
#include <QMouseEvent>
#include <QStackedWidget>

class MainWindow;
class MusicWidget;

class MediaPlayer:public QMediaPlayer{
    Q_OBJECT
public:
    MediaPlayer(MainWindow *mainwindow,QWidget *videoarea,QWidget *musicArea, QStackedWidget *parent=0);
    void fastForward();
    void backForward();
    void onSpace();
    void onCurrentPlayChange(QString url, int pos, QString name);
    void onStateChanged(QMediaPlayer::State state);
    void onPosChanged(qint64 pos);
    void onDuraChanged(qint64 pos);
    void hideVideo();

private:
    QStackedWidget *parent;
    QWidget *videoarea;
    QWidget *musicarea;
    QVideoWidget *videoWidget;
    MusicWidget *musicWidget;
    MainWindow *mainwindow;

signals:
    void VideoUnavailable();
    void AudioUnavailable();

};

class MusicWidget:public QWidget{
    Q_OBJECT
public:
    MusicWidget(QWidget *parent=0);
};

class VideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    VideoWidget(MediaPlayer *media,QWidget *parent=0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void space();


private:
    MediaPlayer *mediaplayer;
};

#endif // MEDIAPLAYER

