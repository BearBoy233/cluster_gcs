#ifndef KCFTRACKER_WIDGET_H
#define KCFTRACKER_WIDGET_H

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#endif

#include "trackerThread.hpp"
#include <QMetaType>
#include <QMessageBox>
#include <QWidget>
#include <QImage>
#include <QMutex>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QWaitCondition>

#include "ui_kcftracker_widget.h"

namespace Ui {
class kcfTracker_widget;
}

class kcfTracker_widget : public QWidget
{
  Q_OBJECT

public:
  explicit kcfTracker_widget(int argc, char** argv, uint8_t id, QWidget *parent = nullptr);
  ~kcfTracker_widget();

  /*用来保护一个对象、数据结构、代码段、使得它们在同一一时刻，只有一个线程有访问权限*/
  mutable QMutex showImage_mutex_;

  cv::Mat receiveImage;//保存接受到的图像
  QImage ImageToQImage;//转换为QImage
  bool receiveImageFlag;//接受到图像为true 处理完设置为false

  // 绘制多边形
  QGraphicsScene  *scene;
  QGraphicsRectItem *RectItem;   //多边形
  QRectF rect; //（x，y）为左上角并具有给定width和height的矩形

  tracker_thread trackerThread;
  //订阅回调函数
  void receiveImage_cb(const sensor_msgs::ImageConstPtr& msg);

  void closeEvent(QCloseEvent *event); // Overloaded function

  void showNoMasterMessage();

private slots:
  void deal_receiveImageSignal();
  void deal_rosShutdown();
  //  鼠标信号
  void deal_mouseMove_signal(QPoint point);
  void deal_mousePress_signal(QPoint point); //鼠标单击
  void deal_mouseRelease_signal(QPoint point);

  void on_kcfTrackerStart_pBtn_clicked();
  void on_show_video_clicked();

private:

  int state_rtsp_1on_2off; //rtsp 视频传输状态

  Ui::kcfTracker_widget *ui;
  int init_argc;
  char** init_argv;

Q_SIGNALS://Qt信号
  void selectCompleteSignal();
  void stopTrackSignal();
};

#endif // KCFTRACKER_WIDGET_H
