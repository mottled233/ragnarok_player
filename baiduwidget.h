#ifndef BAIDUWIDGET
#define BAIDUWIDGET
#include "baidumusic.h"
#include <QTableWidget>
#include <QList>

class BaiduWidget : public QTableWidget
{
    Q_OBJECT
public:
    BaiduWidget(QWidget *parent);
    void search(QString keyword);

private slots:
    void onSearchComplete(int currentPage,int pageCount,QString keyword, QString songList);
    void onGetSongLinkComplete(QString songLink);
    void onDoubleClicked(const QModelIndex &index);

signals:
    void finshGetLink(QString name,QString url);

private:
    QString formatString(QString str);
    int requestPos;
    QString currentSelectedName;
    BaiduMusic *api;
    QList<QString> *songIds;
    QList<QString> *names;

};

#endif // BAIDUWIDGET

