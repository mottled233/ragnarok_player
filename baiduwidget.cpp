#include "baiduwidget.h"
#include <QStringList>
#include <QHeaderView>
#include <qdebug.h>
#include <QTextCodec>

QString unicodeToString(QString str);
QString u2s(QString str);


BaiduWidget::BaiduWidget(QWidget *parent):
QTableWidget(parent)
{
    //初始化成员
    this->requestPos = -1;
    this->currentSelectedName = "";
    this->songIds = new QList<QString>;
    this->names = new QList<QString>;
    this->api = new BaiduMusic;

    //初始化ui
    this->setGeometry(-1,-1,parent->width()+2,parent->height()+2);


    //设置表属性
    this->setColumnCount(2);
    this->setRowCount(16);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//每次选中一行
    this->setSelectionMode(QAbstractItemView::SingleSelection);//且只能选一行

    //设置颜色
    this->setStyleSheet("selection-background-color:rgb(60, 62, 68);color:rgb(190,190,190);");//选中颜色
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base,QColor(41,45,54));
    palette.setColor(QPalette::AlternateBase,QColor(38,42,48));
    this->setPalette(palette);
    this->setAlternatingRowColors(true);

    //设置表头
    this->horizontalHeader()->setStyleSheet("background-color:rgb(50,52,58);color:rgb(190,190,190);");
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
//    QStringList headers;
//    headers<<"歌曲名"<<"歌手";
//    this->setHorizontalHeaderLabels(headers);
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();//隐藏竖直表头

    //连接信号槽
    connect(this,&BaiduWidget::doubleClicked,this,&BaiduWidget::onDoubleClicked);
    connect(api,&BaiduMusic::searchComplete,this,&BaiduWidget::onSearchComplete);
    connect(api,&BaiduMusic::getSongLinkComplete,this,&BaiduWidget::onGetSongLinkComplete);

}

void BaiduWidget::search(QString keyword)
{
    this->songIds->clear();
    this->names->clear();
    this->clear();
    this->api->search(keyword,1);

}

void BaiduWidget::onSearchComplete(int currentPage, int pageCount,
                                   QString keyword, QString songList)
{
    songList = u2s(songList);

    //从json中筛选出信息的正则模型
    QRegularExpression idRe("\"sid\":.+?,");
    QRegularExpression authorRe("\"singer\":.+?,");
    QRegularExpression nameRe("\"sname\":.+?,");


    //全文检索
    QRegularExpressionMatchIterator idit = idRe.globalMatch(songList);
    QRegularExpressionMatchIterator auit = authorRe.globalMatch(songList);
    QRegularExpressionMatchIterator nameit = nameRe.globalMatch(songList);

    int i = 0;
    while(idit.hasNext()&&auit.hasNext()&&nameit.hasNext())
    {
        //获取字符串并处理去掉无用信息
        QString id = this->formatString(idit.next().captured(0));
        QString author = this->formatString(auit.next().captured(0));
        QString name = this->formatString(nameit.next().captured(0));
        //设为widget中显示的数据
        QTableWidgetItem *item = new QTableWidgetItem(name);
        this->setItem(i,0,item);
        item = new QTableWidgetItem(author);
        this->setItem(i,1,item);

        //在类中保存id和歌名数据
        this->songIds->append(id);
        this->names->append(name);
        i++;
    }
    update();
}

void BaiduWidget::onGetSongLinkComplete(QString songLink)
{
    int pos = this->requestPos;
    songLink = songLink.replace("\\","");

    QRegularExpression re("http://yinyue(.+?)\"");
    songLink = re.match(songLink).captured(0).replace("(\\)","");
    songLink = songLink.left(songLink.size()-1);
    qDebug()<<songLink;
    emit this->finshGetLink(this->names->at(pos),songLink);
    this->requestPos = -1;//还原,表示此时不在请求连接
}

void BaiduWidget::onDoubleClicked(const QModelIndex &index)
{

    if(this->requestPos!=-1)return;//如果此时已经有请求
    int pos = index.row();
    this->requestPos = pos;//设置此位置正在请求
    if(pos>songIds->size()-1)
    {
        qDebug()<<"in NetWork:pos out of index";
        return;
    }
    this->api->getSongLink(songIds->at(pos));

}

QString BaiduWidget::formatString(QString str)
{
    int index = str.indexOf(":");
    int end = str.indexOf(",");
    QString result = str.mid(index+1,end-index-1);
    result =  result.replace("\"","");
    return result;

}

QString u2s(QString str)
{
    int pos = 0;
    while(true)
    {
        pos = str.indexOf("\\u",pos);
        if(pos==-1)break;
        QString sub = str.mid(pos,6);
        str = str.replace(sub,unicodeToString(sub.right(4)));
    }
    return str;
}

QString unicodeToString(QString str)
{
    //例如 str="4F60597D";
    int temp[400];
    QChar qchar[100];
    QString strOut;
    bool ok;
    int count=str.count();
    int len=count/4;
    for(int i=0;i<count;i+=4)
    {
     temp[i]=str.mid(i,4).toInt(&ok,16);//每四位转化为16进制整型
     qchar[i/4]=temp[i];
     QString str0(qchar, len);
     strOut=str0;
    }
    return strOut;
}

