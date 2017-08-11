#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "listoflist.h"
#include "listwidget.h"
#include "mediaplayer.h"
#include "baiduwidget.h"
#include <QMainWindow>
#include <qdebug.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int STACKWIDTH=651;
    static const int STACKHEIGHT=507;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initUI();
    void onAddCurrentClicked();
    void onCurrentMediaChanged(QString url,int pos,QString name);
    void changePlayButton(QMediaPlayer::State newState);
    void onPosChanged(qint64 pos);
    void onDuraChanged(qint64 pos);
    CurrentMediaList* getCurrentMediaList();


private slots:
    void on_SearchButtonClicked();

    void on_networkReturn(QString name,QString url);

    void on_listSelect(MediaList *list);

    void on_openListButton_clicked();

    void on_closeCurrentButton_clicked();

    void on_closeAllButton_clicked();

    void on_playButton_clicked();

    void on_deleteCurrentButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_resultListWidget_doubleClicked(const QModelIndex &index);

    void on_playAllBun_clicked();

    void on_listDeleteBtn_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_sliderMoved(int position);

    void on_volSlider_sliderMoved(int position);

    void on_muteButton_clicked();

    void on_modeButton_clicked();

    void on_miniButton_clicked();

private:
    QLineEdit *m_pSearchLineEdit;
    ListOfList *listOfList;
    ListWidget *listWidget;
    MediaPlayer *mediaplayer;
    MediaList *currentSelectList;
    BaiduWidget *baiduWidget;
    Ui::MainWindow *ui;
    int currentMode;
};

#endif // MAINWINDOW_H
