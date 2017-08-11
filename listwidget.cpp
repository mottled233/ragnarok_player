#include "listwidget.h"
#include <qdebug.h>
#include <QPainter>



ListItemWidget::ListItemWidget(QString name, bool isVideo, int pos,ListWidget *list, QWidget *parent):
    QWidget(parent)
{
    this->name = name;
    this->pos = pos;
    this->isVideo = isVideo;
    this->list = list;
    this->setFixedSize(480,40);
    this->playButton = new QPushButton(this);
    this->deleteButton = new QPushButton(this);
    this->nameLabel = new QLabel(name,this);
    this->isOnPlay = false;

    this->nameLabel->setStyleSheet("font: 10pt \"微软雅黑\";color: rgb(200,200,200);");

    if(pos%2!=0)
    {
        this->currentColor = QColor(41,45,54);
        this->setFixedHeight(41);
    }
    else{
        this->currentColor = QColor(36,39,48);
        this->setFixedHeight(39);
    }

    this->nameLabel->setGeometry(20,10,260,20);
    this->playButton->setGeometry(358,10,20,20);
    this->deleteButton->setGeometry(388,5,30,30);
    this->setAutoFillBackground(true);
    this->playButton->setCursor(Qt::PointingHandCursor);
    this->playButton->setStyleSheet("QPushButton{\
                                    border-image: url(:/listPlayBtn.png);\
                                }\
                                QPushButton:hover{\
                                    border-image: url(:/listPlayBtn_hover.png);\
                                }\
                                QPushButton:pressed{\
                                    border-image: url(:/listPlayBtn_click.png);\
                                }");
    this->deleteButton->setCursor(Qt::PointingHandCursor);
    this->deleteButton->setStyleSheet("QPushButton{\
                                      border-image: url(:/deleteLisItemBtn.png);\
                                  }\
                                  QPushButton:hover{\
                                      border-image: url(:/deleteLisItemBtn_hover.png);\
                                  }\
                                  QPushButton:pressed{\
                                      border-image: url(:/deleteLisItemBtn_click.png);\
                                  }");
    this->playButton->hide();
    this->deleteButton->hide();
    connect(this->deleteButton,&QPushButton::clicked,this,&ListItemWidget::onDelete);
}

void ListItemWidget::changePos(int pos)
{
    this->pos = pos;
    if(pos%2!=0)
    {
        this->currentColor = QColor(41,45,54);
        this->setFixedHeight(41);
    }
    else{
        this->currentColor = QColor(36,39,48);
        this->setFixedHeight(39);
    }
    this->update();
}

int ListItemWidget::getPos()
{
    return pos;
}

void ListItemWidget::onSelected()
{
    this->list->selectMedia(this->pos);
}

void ListItemWidget::add1()
{
    int pos = getPos();
    this->changePos(pos+1);
}

void ListItemWidget::sub1()
{
    int pos = getPos();
    this->changePos(pos-1);
}

void ListItemWidget::onCurrentPlayChanged(QString url, int pos, QString name)
{

    if(pos!=this->pos&&this->isOnPlay)
    {
        stop();
    }

}

void ListItemWidget::onPlay()
{
    this->isOnPlay = true;
    this->nameLabel->setStyleSheet("font: 11pt \"微软雅黑\";color: rgb(25, 181, 240);");

    this->currentColor = QColor(32, 35, 42);
    this->update();
}

void ListItemWidget::onDelete()
{
    emit this->deleted(pos);
}
void ListItemWidget::stop()
{

    if(pos%2!=0)
    {
        this->currentColor = QColor(41, 45, 54);
    }
    else{
        this->currentColor = QColor(36,39,48);
    }
    this->nameLabel->setStyleSheet("font: 11pt \"微软雅黑\";color: rgb(200,200,200);");
    this->isOnPlay=false;
    this->update();
}

void ListItemWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    this->onSelected();
}

void ListItemWidget::enterEvent(QEvent *ev)
{
    this->playButton->show();
    this->deleteButton->show();
    if(this->isOnPlay)return;
    if(pos%2!=0)
    {
        this->currentColor = QColor(44,48,57);
    }
    else{
        this->currentColor = QColor(39,42,51);
    }
    this->update();

}

void ListItemWidget::leaveEvent(QEvent *ev)
{
    this->playButton->hide();
    this->deleteButton->hide();
    if(isOnPlay)return;
    if(pos%2!=0)
    {
        this->currentColor = QColor(41,45,54);
    }
    else{
        this->currentColor = QColor(36,39,48);
    }
    this->update();
}

void ListItemWidget::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);

    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(this->currentColor);
    p.drawRect(rect());
}


ListWidget::ListWidget(QWidget *viewCont)
{
    this->viewCont = viewCont;
    this->layout = new QVBoxLayout();
    this->layout->setSpacing(0);
//    this->layout->addSpacing(200);
    this->viewCont->setLayout(layout);

    this->items = new QList<ListItemWidget*>;
    this->current = new CurrentMediaList ("current");
    connect(this->current,&CurrentMediaList::currentPlayChanged,this,&ListWidget::onCurrentPlayChanged);
    this->init();

}

void ListWidget::init()
{
    if(this->items->size()!=0)
        this->clearView();
    int size = this->current->size();
    if(size == 0)return;
    MediaInfo media;
    for(int i = 0;i<size;i++)
    {
        current->getPosInfo(i,media);
        this->items->append(new ListItemWidget(media.name,media.isVideo,media.pos,this));
        connect(this->current,&CurrentMediaList::currentPlayChanged,this->items->at(i),&ListItemWidget::onCurrentPlayChanged);
        connect(this->items->at(i),&ListItemWidget::deleted,this,&ListWidget::removeMedia);
        this->layout->addWidget(this->items->at(i));
    }

}

void ListWidget::addMedia(QString url, int pos, QString name)
{
    this->current->addMedia(url, name, pos);
    bool isVideo = (url.indexOf(".wmv")!=-1||url.indexOf(".avi")!=-1);
    this->items->insert(pos,new ListItemWidget(name,isVideo,pos,this));
    connect(this->current,&CurrentMediaList::currentPlayChanged,this->items->at(pos),&ListItemWidget::onCurrentPlayChanged);
    connect(this->items->at(pos),&ListItemWidget::deleted,this,&ListWidget::removeMedia);
    this->layout->insertWidget(pos,items->at(pos));
    int size = this->items->size();
    for(int i = pos+1;i<size;i++)
        this->items->at(i)->add1();
}

void ListWidget::appendMedia(QString url, QString name)
{

    this->current->addMedia(url,name);
    int pos = size()-1;
    qDebug()<<"in listWidget:appendMedia:url"<<url;
    qDebug()<<"in listWidget:appendMedia:url"<<pos;
    qDebug()<<"in listWidget:appendMedia:url"<<name;
    bool isVideo = (url.indexOf(".wmv")!=-1||url.indexOf(".avi")!=-1);
    this->items->append(new ListItemWidget(name,isVideo,pos,this));
    connect(this->items->at(pos),&ListItemWidget::deleted,this,&ListWidget::removeMedia);
    connect(this->current,&CurrentMediaList::currentPlayChanged,this->items->at(pos),&ListItemWidget::onCurrentPlayChanged);
    this->layout->addWidget(items->at(pos));
}



void ListWidget::removeMedia(int pos)
{
    qDebug()<<"in ListWidget:delete"<<pos;
    if(this->current->getCurrentMediaPos()==pos)
        this->next();
    this->current->removeMedia(pos);
    this->layout->removeWidget(this->items->at(pos));

    int size = this->items->size();
    for(int i = pos+1;i<size;i++)
        this->items->at(i)->sub1();
    disconnect(this->current,&CurrentMediaList::currentPlayChanged,this->items->at(pos),&ListItemWidget::onCurrentPlayChanged);
    this->items->at(pos)->deleteLater();
    this->items->removeAt(pos);

}

void ListWidget::removeAll()
{
    this->current->removeAll();
    delete this->layout;
    this->layout = new QVBoxLayout();
    this->viewCont->setLayout(this->layout);
    qDeleteAll(*items);
    this->items->clear();
}

void ListWidget::clearView()
{
    delete this->layout;
    this->layout = new QVBoxLayout();
    this->viewCont->setLayout(this->layout);
    qDeleteAll(*items);
    this->items->clear();

}

void ListWidget::next()
{
    this->current->next(CurrentMediaList::User);
}

int ListWidget::size()
{
    return current->size();
}

void ListWidget::prev()
{
    this->current->prev(CurrentMediaList::User);
}

void ListWidget::selectMedia(int pos)
{
    qDebug()<<"in ListWidget:selectMedia:pos"<<pos;
    this->current->onSelected(pos);
    this->items->at(pos)->onPlay();
}

void ListWidget::swapMedia(int pos1, int pos2)
{
    if(pos1==pos2)return;
    this->current->swapMedia(pos1,pos2);
    ListItemWidget *w1 = this->items->at(pos1);
    ListItemWidget *w2 = this->items->at(pos2);
    int p1 = this->layout->indexOf(w1);
    int p2 = this->layout->indexOf(w2);
    if(p1>p2)std::swap(p1,p2);
    this->layout->removeWidget(w1);
    this->layout->removeWidget(w2);
    this->layout->insertWidget(p1,w2);
    this->layout->insertWidget(p2,w1);

}

void ListWidget::onCurrentPlayChanged(QString url, int pos, QString name)
{
    qDebug()<<"in ListWidget:url"<<url;
    qDebug()<<"in ListWidget:name"<<name;
    qDebug()<<"in ListWidget:pos"<<pos;
    this->items->at(pos)->onPlay();
    qDebug()<<"in ListWidget:"<<pos;
}


CurrentMediaList *ListWidget::getCurrentPlayList()
{
    return this->current;
}


