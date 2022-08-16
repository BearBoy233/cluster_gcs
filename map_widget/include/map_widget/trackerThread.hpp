
#ifndef Q_MOC_RUN

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <tf/tf.h>

#include <mavcomm_msgs/kcf_set_target.h>
#include <mavcomm_msgs/kcf_target_pos.h>

#endif

#include <iostream>
#include <QThread>
#include <string>
#include <QGraphicsView>
#include <unistd.h>

using namespace std;

class tracker_thread : public QThread
{
    Q_OBJECT
public:
  tracker_thread(int argc, char** argv, QWidget *parent = 0);
  virtual ~tracker_thread();

  // roslaunch param
  int uav1_gcs2;
  string gcs_uavip;
  string uav_video_topic;

  // 本地 测试用 无人机 ID
  std::uint8_t uavid;

  // 窗口鼠标事件
  QPointF mousePosition_start,mousePosition_current;
  bool mousePress;

  mavcomm_msgs::kcf_set_target trackRoi;    // 设定框选区域
  mavcomm_msgs::kcf_target_pos trackResult; // 追踪结果 // state -0 停止 -1 选择

  bool recImage_flag;
  bool is_tracking;                         // 正在追踪
  bool trackerInit_flag;
  bool trackerThreadStatus;
  cv::Mat rgbImage;
  QImage  showImage;

  double scale_x , scale_y;

  image_transport::Subscriber receiveImage_sub;
  ros::Publisher trackRoi_pub;
  ros::Subscriber trackerResult_sub;

  cv::Mat receiveImage;         // 保存接受到的图像
  QImage qImageFromBGRimage;    // 转换为QImage
  bool receiveImageFlag;        // 接受到图像为true 处理完设置为false

  // rtsp
  int flag_rtsp_1run = 0;
  void rtsp_video();
  cv::VideoCapture vc;
  QString String_rtsp_id;
  QImage MatToQImage(const cv::Mat &inMat);

  void run();

  bool init(std::uint8_t uavID);
  void receiveImage_cb(const sensor_msgs::ImageConstPtr& msg);
  void trackerResult_cb(const mavcomm_msgs::kcf_target_pos& msg);

private:
  int init_argc;
  char** init_argv;

public Q_SLOTS:
  void deal_selectCompleteSignal();
  void deal_stopTrackSignal();

Q_SIGNALS://Qt信号
  void receiveImageSignal();
  void rosShutdown();

};



