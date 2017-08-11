#ifndef NEXTSTRATEGY
#define NEXTSTRATEGY

class CurrentMediaList;
class NextStrategy{
public:
    virtual int next()=0;
    virtual int prev()=0;
    virtual int next_auto()=0;
    virtual int prev_auto()=0;
    void setCurrentList(CurrentMediaList *p_mList){list = p_mList;}
protected:
    CurrentMediaList *list;
};

class LoopMode : public NextStrategy{
public:
    int next();
    int prev();
    int next_auto();
    int prev_auto();
};

class RandomMode : public NextStrategy{
public:
    int next();
    int prev();
    int next_auto();
    int prev_auto();
};

class LoopOneMode : public NextStrategy{
public:
    int next();
    int prev();
    int next_auto();
    int prev_auto();
};


class NextFactory{
    static LoopMode *loopMode;
    static RandomMode *randMode;
    static LoopOneMode *loopOneMode;

public:
    static NextStrategy* getLoopMode(CurrentMediaList *crt)
    {loopMode->setCurrentList(crt);return loopMode;}
    static NextStrategy* getRandomMode(CurrentMediaList *crt){randMode->setCurrentList(crt);return randMode;}
    static NextStrategy* getLoopOneMode(CurrentMediaList *crt){loopOneMode->setCurrentList(crt);return loopOneMode;}
    static void init(){
        if (loopMode)
            return;
        loopMode = new LoopMode();
        randMode = new RandomMode();
        loopOneMode = new LoopOneMode();
    }
};



#endif // NEXTSTRATEGY

