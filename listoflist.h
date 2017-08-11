#ifndef LISTOFLIST
#define LISTOFLIST
#include "listwidget.h"
#include <QList>

class ListOfList:public QWidget{
    Q_OBJECT
public:
    ListOfList(QWidget *parent=0);
    ~ListOfList(){if(lists!=0){qDeleteAll(*lists);lists->clear();}if(names!=0)names->clear();}
    void addList(MediaList *mlist);
    void addList(QString name);
    void removeList(QString name);
    void removeAll();
    MediaList* getListAtPos(int pos);
    QString* getAllList(int &size);
    void save();//todo
    void load();//todo
    int size();
    void init(QWidget *parent);
    bool isSelect();
    void cancelSelect();
    void onSelectList();
    void onAdd();
    int getListPosByName(QString name);

signals:
    void listSelected(MediaList *selectedLists);
private:
    QList<MediaList*> *lists;
    QList<QString> *names;
    QList<QPushButton *> *buttons;
    QPushButton *addButton;
    int currentSelect;
};


#endif // LISTOFLIST

