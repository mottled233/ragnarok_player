#ifndef LISTWIDGET
#define LISTWIDGET
#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include "medialist.h"
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>


class ListWidget;
class ListItemWidget:public QWidget{
    Q_OBJECT
public:
    ListItemWidget(QString name, bool isVideo, int pos,ListWidget *list, QWidget *parent=0);
    void changePos(int pos);
    int getPos();
    void onPlay();
    void onDelete();
    void onSelected();
    void add1();
    void sub1();
    void onCurrentPlayChanged(QString url,int pos,QString name);
    void stop();

protected:
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void enterEvent(QEvent *ev);
    void leaveEvent(QEvent *ev);
    void paintEvent(QPaintEvent *ev);

signals:
    void deleted(int pos);
    void selectToPlay(int pos);

private:
    int pos;
    bool isVideo;
    QString name;
    bool isOnPlay;
    ListWidget *list;
    QPushButton *deleteButton;
    QPushButton *playButton;
    QLabel *nameLabel;
    QColor currentColor;

};

class ListWidget:public QObject{
    Q_OBJECT
public:
    ListWidget(QWidget *viewCont);
    void init();
    void addMedia(QString url,int pos,QString name);
    void appendMedia(QString url,QString name);
    void removeMedia(int pos);
    void removeAll();
    void clearView();
    void next();
    int size();
    void prev();
    void selectMedia(int pos);
    void swapMedia(int pos1,int pos2);
    void onCurrentPlayChanged(QString url,int pos,QString name);
    void setCurrentPlayList(CurrentMediaList *crtList);
    CurrentMediaList* getCurrentPlayList();

signals:
    void deletedAll();
    void added(QString url,int pos,QString name);

private:
    QList<ListItemWidget*> *items;
    CurrentMediaList* current;
    QVBoxLayout *layout;
    QWidget *viewCont;
};

#endif // LISTWIDGET

