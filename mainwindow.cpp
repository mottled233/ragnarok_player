#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "anibutton.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QRegularExpression>
#include <QStringList>
#include <QTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    currentMode = 1;
    mediaplayer = new MediaPlayer(this,ui->videoArea,ui->playingPage,ui->stackedWidget);
    this->listOfList = new ListOfList(ui->listOfListWidget);
    connect(this->listOfList,&ListOfList::listSelected,this,&MainWindow::on_listSelect);

    ui->stackedWidget->setCurrentIndex(1);
    currentSelectList = 0;

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    //音量条
    ui->volSlider->setMaximum(100);
    ui->volSlider->setValue(100);
    //下部播放条
    QPalette palette = ui->controlBar->palette();
    QLinearGradient gradient(0,0,0,50);
    gradient.setColorAt(0.0,QColor(40,42,52));
    gradient.setColorAt(0.4,QColor(34,38,48));
    gradient.setColorAt(0.8,QColor(26,29,37));
    palette.setBrush(QPalette::Window,QBrush(gradient));
    palette.setColor(QPalette::WindowText,QColor(221,221,221));
    ui->controlBar->setPalette(palette);

    //上部搜索框
    m_pSearchLineEdit = new QLineEdit(ui->searchWidget);
    m_pSearchLineEdit->setFixedSize(280,30);
    m_pSearchLineEdit->setStyleSheet("QLineEdit{background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4d4d4d, stop: 0 #646464, stop: 1 #5d5d5d);padding: 1px;border-style: solid;border: 1px solid #1e1e1e;border-radius: 5;}");
    QPushButton *pSearchButton = new QPushButton();
    pSearchButton->setCursor(Qt::PointingHandCursor);
    pSearchButton->setFixedSize(22, 22);
    pSearchButton->setStyleSheet("QPushButton{border-image:url(:/searchIcon.png); background:transparent;} \
                                         QPushButton:hover{border-image:url(:/searchIcon_hover.png)} \
                                         QPushButton:pressed{border-image:url(:/searchIcon_clicked.png)}");
    pSearchButton->setToolTip(QStringLiteral("搜索"));
    //防止文本框输入内容位于按钮之下
    QMargins margins = m_pSearchLineEdit->textMargins();
    m_pSearchLineEdit->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
    m_pSearchLineEdit->setPlaceholderText(QStringLiteral("搜索歌曲/歌手"));
    QHBoxLayout *pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(pSearchButton);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(0, 0, 0, 0);
    m_pSearchLineEdit->setLayout(pSearchLayout);

    //初始化网络模块
    this->baiduWidget = new BaiduWidget(ui->netWorkPage);

    //播放列表初始化
    ui->lis_widget->hide();
    this->listWidget = new ListWidget(ui->scrollAreaWidget);

    //绑定信号槽
    connect(pSearchButton,&QPushButton::clicked,this,&MainWindow::on_SearchButtonClicked);
    connect(baiduWidget,&BaiduWidget::finshGetLink,this,&MainWindow::on_networkReturn);
    connect(ui->addCurrentButton,&QPushButton::clicked,this,&MainWindow::onAddCurrentClicked);
    CurrentMediaList *crt = this->listWidget->getCurrentPlayList();
    connect(crt,&CurrentMediaList::currentPlayChanged,this,&MainWindow::onCurrentMediaChanged);


    //窗口无框化
    setWindowOpacity(1);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

}

void MainWindow::onAddCurrentClicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("添加音乐或视频");
    QStringList filters;
    filters << "音乐文件(*.mp3 *.wav *.wma)"
    << "视频文件(*.avi *.mp4 *.wmv)"
    << "Any files (*)";
    fileDialog.setNameFilters(filters);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    if (fileDialog.exec() == QDialog::Accepted)
    {
        QStringList strPathList = fileDialog.selectedFiles();
        for(int i = 0;i<strPathList.size();i++)
        {
            QString path = strPathList.at(i);
            if(!path.isEmpty()) {
                QStringList list = path.split("/");
                QString name = list.at(list.size()-1);
                this->listWidget->appendMedia(path,name);
                }
        }
        this->listWidget->selectMedia(listWidget->size()-1);
        ui->lis_widget->hide();

    }

}

void MainWindow::onCurrentMediaChanged(QString url, int pos, QString name)
{
    this->mediaplayer->onCurrentPlayChange(url,pos,name);
    ui->nameLabel->setText(name);
}

void MainWindow::changePlayButton(QMediaPlayer::State newState)
{
    if(newState == QMediaPlayer::PlayingState){
        ui->playButton->setStyleSheet("QPushButton#playButton{\
                                      border-image: url(:/pauseIcon.png);\
                                      }\
                                      QPushButton#playButton:hover{\
                                      border-image: url(:/pauseIcon_hover.png);\
                                      }\
                                      QPushButton#playButton:pressed{\
                                      border-image: url(:/pauseIcon_clicked.png);\
                                      }");
    }else{
        ui->playButton->setStyleSheet("QPushButton#playButton{\
                                      border-image: url(:/playIcon.png);\
                                      }\
                                      QPushButton#playButton:hover{\
                                      border-image: url(:/playIcon_hover.png);\
                                      }\
                                      QPushButton#playButton:pressed{\
                                      border-image: url(:/playIcon_clicked.png);\
                                      }");
                                      }
                                      }

void MainWindow::onPosChanged(qint64 pos)
{
    int currentInfo = pos/1000;
    int duration = this->mediaplayer->duration()/1000;
    ui->horizontalSlider->setSliderPosition(currentInfo);

    QString tStr;

    QTime currentTime(currentInfo/3600,(currentInfo%3600)/60,currentInfo%60);
    QTime totalTime(duration/3600,duration%3600/60,duration%60);
    QString format = "mm:ss";
    if (duration > 3600)
        format = "hh:mm:ss";
    tStr = currentTime.toString(format) + " / " + totalTime.toString(format);

    ui->timeLabel->setText(tStr);
}

void MainWindow::onDuraChanged(qint64 pos)
{
    qDebug()<<"aa";
    ui->horizontalSlider->setMaximum(mediaplayer->duration()/1000);
    ui->horizontalSlider->setSliderPosition(0);
    int currentInfo = 0;
    int duration = mediaplayer->duration()/1000;
    QString tStr;
    QTime currentTime(currentInfo/3600,(currentInfo%3600)/60,currentInfo%60);
    QTime totalTime(duration/3600,duration%3600/60,duration%60);
    QString format = "mm:ss";
    if (duration > 3600)
        format = "hh:mm:ss";
    tStr = currentTime.toString(format) + " / " + totalTime.toString(format);

    ui->timeLabel->setText(tStr);

}

CurrentMediaList *MainWindow::getCurrentMediaList()
{
    return this->listWidget->getCurrentPlayList();
}

void MainWindow::on_SearchButtonClicked()
{
    QString keyword = this->m_pSearchLineEdit->text();
    if(keyword=="")return;
    this->baiduWidget->search(keyword);
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_networkReturn(QString name, QString url)
{
    this->listWidget->appendMedia(url,name);
    this->listWidget->selectMedia(this->listWidget->size()-1);
}

void MainWindow::on_listSelect(MediaList *list)
{
    ui->listNameLabel->setText(list->getListName());
    //清空QListWidget中原有的item
    ui->resultListWidget->clear();
    //设置为当前选中列表
    this->currentSelectList = list;

    int size = list->size();
    for(int i = 0;i<size;i++)
    {
        //获取列表中的歌曲信息
        MediaInfo info;
        list->getPosInfo(i,info);

        //向QListWidget中添加条目
        QListWidgetItem *newItem = new QListWidgetItem();
        QWidget *widget = new QWidget();
        widget->setStyleSheet("color:rgb(190,190,190);font: 10pt \"微软雅黑\";");
        widget->setFixedSize(651,50);
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(new QLabel(QString::number(i+1)));
        layout->addStretch(1);
        layout->addWidget(new QLabel(info.name));
        layout->addStretch(5);
        widget->setLayout(layout);
        //设置背景颜色
        if(ui->resultListWidget->count()%2!=0)
            newItem->setBackgroundColor(QColor(40,43,50));
        if(ui->resultListWidget->count()%2==0)
            newItem->setBackgroundColor(QColor(33,38,47));
        newItem->setSizeHint(QSize(651,50));

        ui->resultListWidget->addItem(newItem);
        ui->resultListWidget->setItemWidget(newItem,widget);
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_openListButton_clicked()
{
    if(ui->lis_widget->isVisible())
        ui->lis_widget->hide();
    else
        ui->lis_widget->show();
}

void MainWindow::on_closeCurrentButton_clicked()
{
    ui->lis_widget->hide();
}

void MainWindow::on_closeAllButton_clicked()
{
    exit(0);
}

void MainWindow::on_playButton_clicked()
{
    if(this->mediaplayer->state()==MediaPlayer::PlayingState)
        this->mediaplayer->pause();
//    else if(this->mediaplayer->state()==MediaPlayer::PausedState)
    else
        this->mediaplayer->play();
}

void MainWindow::on_deleteCurrentButton_clicked()
{
    this->listWidget->removeAll();
}

void MainWindow::on_pushButton_clicked()
{
    this->listWidget->next();
}



void MainWindow::on_pushButton_2_clicked()
{
    this->listWidget->prev();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(currentSelectList==0)return;
    //打开音乐文件对话框
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("添加音乐");
    QStringList filters;
    filters << "音乐文件(*.mp3 *.wav *.wma)";
    fileDialog.setNameFilters(filters);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    //执行对话框，若对话框被接受
    if (fileDialog.exec() == QDialog::Accepted)
    {
        QStringList strPathList = fileDialog.selectedFiles();
        for(int i = 0;i<strPathList.size();i++)
        {
            QString path = strPathList.at(i);
            if(!path.isEmpty()) {
                QStringList list = path.split("/");
                QString name = list.at(list.size()-1);
                //将数据添加到选定的歌单
                this->currentSelectList->addMedia(path,name);
                //将添加的数据展示在QListWidget中
                QListWidgetItem *newItem = new QListWidgetItem();
                //item内部布局
                QWidget *widget = new QWidget();
                widget->setStyleSheet("color:rgb(190,190,190);font: 10pt \"微软雅黑\";");
                widget->setFixedSize(651,50);
                QHBoxLayout *layout = new QHBoxLayout();
                layout->addWidget(new QLabel(QString::number(this->currentSelectList->size())));
                layout->addStretch(1);
                layout->addWidget(new QLabel(name));
                layout->addStretch(5);
                widget->setLayout(layout);
                //给item绘背景色
                if(ui->resultListWidget->count()%2!=0)
                    newItem->setBackgroundColor(QColor(40,43,50));
                if(ui->resultListWidget->count()%2==0)
                    newItem->setBackgroundColor(QColor(33,38,47));
                //设置大小
                newItem->setSizeHint(QSize(651,50));
                //将item添加到listWidget并设置其widget布局
                ui->resultListWidget->addItem(newItem);
                ui->resultListWidget->setItemWidget(newItem,widget);
                }
        }

    }

}

void MainWindow::on_resultListWidget_doubleClicked(const QModelIndex &index)
{
    int pos = index.row();
    MediaInfo info;
    this->currentSelectList->getPosInfo(pos,info);
    this->listWidget->appendMedia(info.url,info.name);
    this->listWidget->selectMedia(listWidget->size()-1);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_playAllBun_clicked()
{
    if(this->currentSelectList==0||this->currentSelectList->size()==0)return;
    this->currentSelectList->copyToCrtList(this->listWidget->getCurrentPlayList());
    this->listWidget->init();
    this->listWidget->selectMedia(0);
}

void MainWindow::on_listDeleteBtn_clicked()
{
    if(this->currentSelectList==0)return;
    this->listOfList->removeList(this->currentSelectList->getListName());
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{

}

void MainWindow::on_horizontalSlider_sliderMoved(int value)
{
    if (this->mediaplayer->state()!=QMediaPlayer::PlayingState&&this->mediaplayer->state()!=QMediaPlayer::PausedState)
    {
        ui->horizontalSlider->setValue(0);
        return;
    }
    int currentPos = this->mediaplayer->position()/1000;

    if(qAbs(value-currentPos)>2&&ui->horizontalSlider->value()!=0)
    {
        if(this->mediaplayer->state()==QMediaPlayer::PlayingState)
        {
            this->mediaplayer->stop();
            this->mediaplayer->setPosition(value*1000);
            this->mediaplayer->play();
        }
        else
        {
            this->mediaplayer->stop();
            this->mediaplayer->setPosition(value*1000);
        }

    }
}

void MainWindow::on_volSlider_sliderMoved(int position)
{
    this->mediaplayer->setVolume(position);
}

void MainWindow::on_muteButton_clicked()
{
    if(this->mediaplayer->isMuted())
    {
        this->mediaplayer->setMuted(false);
        ui->muteButton->setStyleSheet("QPushButton{border-image: url(:/volIcon.png);}\
                                      QPushButton:hover{border-image: url(:/volIcon_hover.png);}\
                                      QPushButton:pressed{border-image: url(:/volIcon_click.png);}");
    }else{
        this->mediaplayer->setMuted(true);
        ui->muteButton->setStyleSheet("QPushButton{border-image: url(:/muteIcon.png);}\
                                      QPushButton:hover{border-image: url(:/muteIcon_hover.png);}\
                                      QPushButton:pressed{border-image: url(:/muteIcon_click.png);}");
    }
}



void MainWindow::on_modeButton_clicked()
{
    currentMode = (currentMode)%3+1;
    switch (currentMode) {
    case 1:
        this->listWidget->getCurrentPlayList()->setNextStrategy(CurrentMediaList::DEFAULT);
        ui->modeButton->setStyleSheet("QPushButton{border-image: url(:/loopIcon.png);}\
                                      QPushButton:hover{border-image: url(:/loopIcon_hover.png);}\
                                      QPushButton:pressed{border-image: url(:/loopIcon_click.png);}");
        break;
    case 2:
        ui->modeButton->setStyleSheet("QPushButton{border-image: url(:/loopOneIcon.png);}\
                                      QPushButton:hover{border-image: url(:/LoopOneIcon_hover.png);}\
                                      QPushButton:pressed{border-image: url(:/loopOneIcon_click.png);}");
        this->listWidget->getCurrentPlayList()->setNextStrategy(CurrentMediaList::LOOP);
        break;
    case 3:
        ui->modeButton->setStyleSheet("QPushButton{border-image: url(:/randIcon.png);}\
                                      QPushButton:hover{border-image: url(:/randIcon_hover.png);}\
                                      QPushButton:pressed{border-image: url(:/randIcon_click.png);}");
        this->listWidget->getCurrentPlayList()->setNextStrategy(CurrentMediaList::RAND);
        break;
    }
}

void MainWindow::on_miniButton_clicked()
{
    this->showMinimized();
}
