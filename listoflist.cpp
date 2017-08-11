#include "listoflist.h"
#include <qDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QInputDialog>

ListOfList::ListOfList(QWidget *parent):
    QWidget(parent)
{
    this->currentSelect = -1;
    this->lists = new QList<MediaList*>;
    this->names = new QList<QString>;
    this->buttons = new QList<QPushButton*>;

    this->addButton = new QPushButton(this);
    this->addButton->setGeometry(0,0,parent->width(),40);
    this->addButton->setObjectName("adddListButton");
    this->setStyleSheet("QPushButton#adddListButton{\
                        border-image: url(:/addListBtn.png);\
                    }\
                    QPushButton:hover#adddListButton{\
                        border-image: url(:/addListBtn_hover.png);\
                    }\
                    QPushButton:pressed#adddListButton{\
                        border-image: url(:/addListBtn_click.png);\
                    }");
    this->addButton->setCursor(Qt::PointingHandCursor);
    connect(this->addButton,&QPushButton::clicked,this,&ListOfList::onAdd);

    this->setGeometry(0,0,parent->width(),parent->height());
    this->load();
    this->init(parent);
}

void ListOfList::addList(MediaList *mlist)
{
    this->lists->append(mlist);
    this->names->append(mlist->getListName());
}

void ListOfList::addList(QString name)
{
    if(size()>8)return;
    this->lists->append(new MediaList(name));
    this->names->append(name);

    this->buttons->at(size()-1)->show();
    this->buttons->at(size()-1)->setText(name);

    this->addButton->move(0,this->addButton->y()+40);
}

void ListOfList::removeList(QString name)
{
    int pos = this->names->indexOf(name);
    if(pos==-1)
        return;
    this->lists->at(pos)->deleteLater();
    this->lists->removeAt(pos);
    this->names->removeAt(pos);
    for(int i = pos;i<9;i++){
        this->buttons->at(i)->setText(buttons->at(i+1)->text());
    }
    buttons->at(this->lists->size())->hide();
    this->addButton->move(0,this->addButton->y()-40);
}

void ListOfList::removeAll()
{
    qDeleteAll(*lists);
    this->lists->clear();
    this->names->clear();
}

MediaList *ListOfList::getListAtPos(int pos)
{
    return this->lists->at(pos);
}



QString *ListOfList::getAllList(int &size)
{
    size = this->size();
    QString *allString = new QString[size];

    for(int i = 0;i<size;i++){
        allString[i] = this->names->at(i);
    }
    return allString;
}

void ListOfList::save()
{
//    QFile file(":/xml/lists.xml");
//    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
//    {
//        QXmlStreamWriter writer(&file);
//        writer.setAutoFormatting(true);
//        writer.writeStartDocument();

//        writer.writeStartElement("lists");
//        int sizeOfLists = this->lists->size();
//        for(int i = 0;i<sizeOfLists;i++)
//        {
//            writer.writeStartElement("list");
//            MediaList *list = lists->at(i);
//            int listSize = list
//        }
//        writer.writeEndElement();
//        writer.writeEndDocument();
//        file.close();
//    }  else{
//        qDebug()<<"gg";
//    }
}

void ListOfList::load()
{
    //todo
}

int ListOfList::size()
{
    return this->lists->size();
}

void ListOfList::init(QWidget *parent)
{
    for(int i = 0;i < 10;i ++)
    {
        buttons->append(new QPushButton(this));
        this->buttons->at(i)->setObjectName("lisBtn"+QString::number(i));
        this->buttons->at(i)->setGeometry(0,i*40,parent->width(),40);
        this->buttons->at(i)->setCursor(Qt::PointingHandCursor);
        this->buttons->at(i)->setFlat(true);
        this->buttons->at(i)->setStyleSheet("QPushButton#lisBtn"+QString::number(i)+"{font: 12pt \"微软雅黑\"; color: rgb(190, 190, 190);background-color: rgb(38, 42, 50);}\
                                            QPushButton:hover#lisBtn"+QString::number(i)+"{\
                                                background-color: rgb(40,45,54);\
                                                color: rgb(25, 181, 240);\
                                            }\
                                            QPushButton:pressed#lisBtn"+QString::number(i)+"{\
                                                background-color: rgb(28,30,37);\
                                                color: rgb(17, 108, 200);\
                                                border-image: url(:/listOfList_select.png);\
                                            }");//todo
        connect(buttons->at(i),&QPushButton::clicked,this,&ListOfList::onSelectList);
        this->buttons->at(i)->hide();
    }
    int size = this->size();
    for(int i = 0;i<size;i++)
    {
        this->buttons->at(i)->show();
        this->buttons->at(i)->setText(names->at(i));

    }
    this->addButton->move(0,size*40);

}

bool ListOfList::isSelect()
{
    return this->currentSelect!=-1;
}

void ListOfList::cancelSelect()
{
    if(currentSelect!=-1)
    {
        buttons->at(currentSelect)->setStyleSheet("QPushButton#lisBtn"+QString::number(currentSelect)+"{font: 12pt \"微软雅黑\"; color: rgb(190, 190, 190);background-color: rgb(38, 42, 50);}\
                                                  QPushButton:hover#lisBtn"+QString::number(currentSelect)+"{\
                                                      background-color: rgb(40,45,54);\
                                                      color: rgb(25, 181, 240);\
                                                  }\
                                                  QPushButton:pressed#lisBtn"+QString::number(currentSelect)+"{\
                                                      background-color: rgb(28,30,37);\
                                                      color: rgb(17, 108, 200);\
                                                      border-image: url(:/listOfList_select.png);\
                                                  }");
    }
}

void ListOfList::onSelectList()
{
    QPushButton *send = dynamic_cast<QPushButton *>(sender());
    qDebug()<<send->text();
    if(currentSelect!=-1)
    {
        buttons->at(currentSelect)->setStyleSheet("QPushButton#lisBtn"+QString::number(currentSelect)+"{font: 12pt \"微软雅黑\"; color: rgb(190, 190, 190);background-color: rgb(38, 42, 50);}\
                                                  QPushButton:hover#lisBtn"+QString::number(currentSelect)+"{\
                                                      background-color: rgb(40,45,54);\
                                                      color: rgb(25, 181, 240);\
                                                  }\
                                                  QPushButton:pressed#lisBtn"+QString::number(currentSelect)+"{\
                                                      background-color: rgb(28,30,37);\
                                                      color: rgb(17, 108, 200);\
                                                      border-image: url(:/listOfList_select.png);\
                                                  }");
    }
    int pos = this->getListPosByName(send->text());
    this->buttons->at(pos)->setStyleSheet("QPushButton#lisBtn"+QString::number(pos)+"{font: 12pt \"微软雅黑\";background-color: rgb(28,30,37);color: rgb(25, 181, 240);border-image: url(:/listOfList_select.png);}");
    currentSelect=pos;
    emit this->listSelected(this->getListAtPos(pos));
}

void ListOfList::onAdd()
{
    if(this->size()>8)return;
    bool isOK;
    QString text = QInputDialog::getText(NULL, "新建歌单",
                                                       "请输入歌单名",
                                                       QLineEdit::Normal,
                                                       "你的歌单名",
                                                       &isOK);
    if(isOK) {
        for(int i =0;i<size();i++)
            if(names->at(i)==text)return;
        this->addList(text);
    }
}

int ListOfList::getListPosByName(QString name)
{
   int pos = this->names->indexOf(name);
   return pos;
}
