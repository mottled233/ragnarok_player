#include "anibutton.h"
#include <qDebug>

AniButton::AniButton(/*QString file,*/QWidget *parent)
    : QPushButton(parent),m_flag(0),m_num(8)
{
    this->setFixedSize(200,200);
    runAni = new QPropertyAnimation(this,"flag");
    runAni->setDuration(600);
    runAni->setStartValue(0);
    backAni = new QPropertyAnimation(this,"flag");
    backAni->setDuration(600);
    backAni->setEndValue(0);
    points = new QPoint[3];
    qDebug()<<rect().x();
    qDebug()<<rect().y();
    qDebug()<<rect().width();
    qDebug()<<rect().height();

    points[0] = *new QPoint(20,20);
    points[1] = *new QPoint(20,80);
    points[2] = *new QPoint(80,50);
}
AniButton::~AniButton()
{
}
void AniButton::setPicture(QString file)
{
    m_pixmap.load(file);
    setFixedSize(m_pixmap.width()/m_num,m_pixmap.height());
    update();
}
void AniButton::setFramesNum(int num)
{
    m_num = num;
    setFixedSize(m_pixmap.width()/m_num,m_pixmap.height());
    runAni->setEndValue(m_num-1);
    backAni->setStartValue(m_num-1);
    update();
}
void AniButton::setFlag(int flag)
{
    m_flag = flag;
    update();
}
int AniButton::readFlag()
{
    return m_flag;
}
void AniButton::enterEvent(QEvent *)
{
    runAni->start();
}
void AniButton::leaveEvent(QEvent *)
{
    if(runAni->state() == QAbstractAnimation::Running)
    {
        connect(runAni,SIGNAL(finished()),backAni,SLOT(start()));
        connect(backAni,SIGNAL(finished()),this,SLOT(on_disconnect()));
    }
    else
        backAni->start();
}
void AniButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPixmap(rect(),m_pixmap,QRect(m_pixmap.width()/m_num*m_flag,0,
                                             m_pixmap.width()/m_num,m_pixmap.height()));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(102+m_flag*5,204+m_flag*3,240+m_flag));
    painter.drawEllipse(rect());
    painter.setPen(QPen(Qt::green,6,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);

    painter.drawConvexPolygon(points,3);
}
void AniButton::on_disconnect()
{
    disconnect(runAni,SIGNAL(finished()),backAni,SLOT(start()));
    disconnect(backAni,SIGNAL(finished()),this,SLOT(on_disconnect()));
}

