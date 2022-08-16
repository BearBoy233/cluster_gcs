
#ifndef rtsp_single_qnode_HPP_
#define rtsp_single_qnode_HPP_

#ifndef Q_MOC_RUN

#include <ros/ros.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

#endif

#include <ros/network.h>
#include <sstream>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <QGraphicsView>

#include <std_msgs/String.h>

namespace map_widget {

class rtsp_single_QNode : public QThread {
    Q_OBJECT
public:
  rtsp_single_QNode(int argc, char** argv );
  virtual ~rtsp_single_QNode();
	bool init();

	void run();

  int flag_1rtsp2_3rostopic4; // 1rtsp 3rostopic 5pause
  QString subRosTopic;
  QString String_rtsp_id;

  //adding
  image_transport::Subscriber receiveImage_sub;
  cv::Mat receiveImage;//保存接受到的图像
  QImage qImageFromBGRimage;//转换为QImage
  void receiveImage_cb(const sensor_msgs::ImageConstPtr& msg);
  QImage MatToQImage(const cv::Mat &inMat);

Q_SIGNALS:
  void rosShutdown();

  // adding
  void receiveImageSignal();

private:
	int init_argc;
	char** init_argv;

  ros::NodeHandle n;
  image_transport::ImageTransport *it;
  cv::VideoCapture vc;

};

}  // namespace map_widget

#endif /* map_widget_QNODE_HPP_ */
