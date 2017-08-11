#include "medialist.h"
#include <QDebug>

QString MediaList::getMediaUrl(int pos)
{
    return this->urls->at(pos);
}

void MediaList::addMedia(QString url, QString name, int pos)
{
    if(pos==-1)
    {
        this->urls->append(url);
        this->names->append(name);
    }
    else
    {
        this->urls->insert(pos,url);
        this->names->insert(pos,name);
    }

}

void MediaList::removeMedia(int pos)
{
    this->names->removeAt(pos);
    this->urls->removeAt(pos);
}

void MediaList::getPosInfo(int pos, MediaInfo &info)
{
    info.pos = pos;
    info.isVideo = this->isVideo(pos);
    info.name = this->names->at(pos);
    info.parentList = this;
    info.name = this->names->at(pos);
    info.url = this->urls->at(pos);
}

bool MediaList::isVideo(int pos)
{
    QString url = this->urls->at(pos);
    bool isVideo = (url.indexOf(".wmv")!=-1)||(url.indexOf(".avi")!=-1)||(url.indexOf(".mp4")!=-1);
        return true;
    return false;
}

void MediaList::setListName(QString name)
{
    this->name = name;
}

void MediaList::swapMedia(int pos1, int pos2)
{
    this->urls->swap(pos1,pos2);
    this->names->swap(pos1,pos2);
}

QString MediaList::getListName()
{
    return this->name;
}

void MediaList::copyToCrtList(CurrentMediaList *crtList)
{
    crtList->removeAll();
    int size = this->size();
    for(int i = 0;i<size;i++)
        crtList->addMedia(this->urls->at(i),this->names->at(i));
}

void MediaList::removeAll()
{
    this->urls->clear();
    this->names->clear();
}

void MediaList::getInfoByName(QString name, MediaInfo &info)
{
    int pos = this->names->indexOf(name);
    if(pos==-1){
        info.name = "歌曲未找到";
        return;
    }
    this->getPosInfo(pos,info);
}

int MediaList::size()
{
    return this->urls->size();
}


CurrentMediaList::CurrentMediaList(QString name):MediaList(name)
{
    NextFactory::init();
    this->nextstr = NextFactory::getLoopMode(this);
    this->currentPos = 0;
}

void CurrentMediaList::prev(CurrentMediaList::WHO who)
{
    int pos = 0;
    if(who==CurrentMediaList::Auto)
        pos = this->nextstr->prev_auto();
    else
        pos = this->nextstr->prev();
    this->currentPos = pos;
    MediaInfo info;
    this->getPosInfo(pos,info);
    emit this->currentPlayChanged(info.url,info.pos,info.name);

}

void CurrentMediaList::next(CurrentMediaList::WHO who)
{
    qDebug()<<"next!"<<who<<"as auto is"<<CurrentMediaList::Auto;
    int pos = 0;
    if(who==CurrentMediaList::Auto)
        pos = this->nextstr->next_auto();
    else
        pos = this->nextstr->next();

    this->currentPos = pos;
    MediaInfo info;
    this->getPosInfo(pos,info);

    emit this->currentPlayChanged(info.url,info.pos,info.name);
}

void CurrentMediaList::setNextStrategy(CurrentMediaList::MODE mode)
{
    NextFactory::init();
    switch (mode) {
    case CurrentMediaList::RAND:
        this->nextstr = NextFactory::getRandomMode(this);
        break;
    case CurrentMediaList::DEFAULT:
        this->nextstr = NextFactory::getLoopMode(this);
        break;
    case CurrentMediaList::LOOP:
        this->nextstr = NextFactory::getLoopOneMode(this);
        break;
    default:
        break;
    }
}

int CurrentMediaList::getCurrentMediaPos()
{
    return this->currentPos;
}

void CurrentMediaList::onSelected(int pos)
{
    this->currentPos = pos;
    MediaInfo info;
        qDebug()<<"in currentList:"<<pos;
    this->getPosInfo(pos,info);    qDebug()<<"in currentList:"<<info.url;
    emit this->currentPlayChanged(info.url,info.pos,info.name);

}

