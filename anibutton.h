#ifndef ANIBUTTON
#define ANIBUTTON

#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QPainter>

#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QPainter>
class AniButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int flag READ readFlag WRITE setFlag)
public:
    AniButton(QWidget *parent = 0);
    ~AniButton();
    void setPicture(QString file);
    void setFramesNum(int num);
    void setFlag(int flag);
    int readFlag();
private:
    QPixmap m_pixmap;
    int m_flag;
    int m_num;
    QPropertyAnimation *runAni;
    QPropertyAnimation *backAni;
    QPoint *points;
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
public slots:
    void on_disconnect();
};

#endif // ANIBUTTON

