#include "nextstrategy.h"
#include "medialist.h"
#include <QTime>

LoopMode *NextFactory::loopMode = new LoopMode();
RandomMode *NextFactory::randMode = new RandomMode();
LoopOneMode *NextFactory::loopOneMode = new LoopOneMode();


int LoopMode::next()
{
    int size = this->list->size();
    int cPos = list->getCurrentMediaPos();
    if(cPos==size-1)
        return 0;
    return cPos+1;
}

int LoopMode::prev()
{
    int size = this->list->size();
    int cPos = list->getCurrentMediaPos();
    if(cPos==0)
        return size-1;
    return cPos-1;
}

int LoopMode::next_auto()
{
    next();
}

int LoopMode::prev_auto()
{
    prev();
}

int RandomMode::next()
{
    int size = this->list->size();
    int cPos = list->getCurrentMediaPos();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return qrand()%size;

}

int RandomMode::prev()
{
    int size = this->list->size();
    int cPos = list->getCurrentMediaPos();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return qrand()%size;
}

int RandomMode::next_auto()
{
    next();
}

int RandomMode::prev_auto()
{
    prev();
}


int LoopOneMode::next()
{
    int size = this->list->size();
    int cPos = list->getCurrentMediaPos();
    if(cPos==size-1)
        return 0;
    return cPos+1;
}

int LoopOneMode::prev()
{
    int size = this->list->size();
    int cPos = list->getCurrentMediaPos();
    if(cPos==0)
        return size-1;
    return cPos-1;
}

int LoopOneMode::next_auto()
{
    return list->getCurrentMediaPos();
}

int LoopOneMode::prev_auto()
{
    return list->getCurrentMediaPos();
}
