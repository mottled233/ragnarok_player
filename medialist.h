#ifndef MEDIALIST
#define MEDIALIST
#include <QList>
#include <QObject>
#include "nextstrategy.h"

class MediaList;
class CurrentMediaList;

struct MediaInfo{
    QString name;
    QString url;
    bool isVideo;
    int pos;
    MediaList *parentList;
};

class MediaList : public QObject{
    Q_OBJECT
public:
    MediaList(QString name){this->name = name;urls = new QList<QString>();names = new QList<QString>();}
    ~MediaList(){urls->clear();names->clear();}
    QString getMediaUrl(int pos);
    void addMedia(QString url, QString name, int pos=-1);
    void removeMedia(int pos);
    void getPosInfo(int pos,MediaInfo &info);
    bool isVideo(int pos);
    void setListName(QString name);
    void swapMedia(int pos1,int pos2);
    QString getListName();
    void copyToCrtList(CurrentMediaList *crtList);
    void removeAll();
    void getInfoByName(QString name,MediaInfo &info);
    int size();
protected:
    virtual bool isCurrentList(){return false;}
private:
    QString name;
    QList<QString> *urls;
    QList<QString> *names;
};

class CurrentMediaList : public MediaList{
    Q_OBJECT
public:
    CurrentMediaList(QString name);
    enum WHO{Auto,User};
    enum MODE{RAND,DEFAULT,LOOP};
    void prev(CurrentMediaList::WHO who);
    void next(CurrentMediaList::WHO who);
    void setNextStrategy(CurrentMediaList::MODE mode);
    int getCurrentMediaPos();
    void onSelected(int pos);

protected:
    bool isCurrentList(){return true;}

signals:
    void currentPlayChanged(QString url,int pos,QString name);

private:
    int currentPos;
    NextStrategy *nextstr;
};

#endif // MEDIALIST

