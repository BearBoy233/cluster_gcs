#include "../include/map_widget/trackerQGraphicsView.hpp"

trackerQGraphicsView::trackerQGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void trackerQGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    Q_EMIT mouseMove_signal(point);
}

void trackerQGraphicsView::wheelEvent(QWheelEvent *event)
{
    QPoint point = event->pos();
    int delta = event->delta();
    Q_EMIT mouseWheel_signal(point,delta);
}

void trackerQGraphicsView::mousePressEvent(QMouseEvent *event)
{
  //鼠标左键按下事件
  if (event->button()==Qt::LeftButton)
  {
    QPoint point=event->pos();        //QGraphicsView的坐标
    Q_EMIT mousePress_signal(point);  //释放信号
  }
  QGraphicsView::mousePressEvent(event);
}

void trackerQGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button()==Qt::LeftButton)
  {
    QPoint point=event->pos();          //QGraphicsView的坐标
    Q_EMIT mouseRelease_signal(point);  //释放信号
  }
  QGraphicsView::mouseReleaseEvent(event);
}
