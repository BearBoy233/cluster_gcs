
#include "../include/map_widget/rtsp_single_qnode.hpp"

namespace map_widget {

rtsp_single_QNode::rtsp_single_QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

rtsp_single_QNode::~rtsp_single_QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
  // delete rtsp_single_QNode;
}

bool rtsp_single_QNode::init() {
  ros::init(init_argc,init_argv,"rtsp_single_widget");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.

  // receiveImage_sub = it.subscribe("/rtsp_camera_relay_"+ uavname + "/image",5,&tracker_thread::receiveImage_cb,this,image_transport::TransportHints("compressed"));// 订阅 图像信息
  // receiveImage_sub = it.subscribe("/rtsp_camera_relay/image",20,&rtsp_single_QNode::receiveImage_cb,this,image_transport::TransportHints("compressed"));// 订阅 图像信息
  // 广播 pub
  it = new image_transport::ImageTransport(n);

  // 初始化
  flag_1rtsp2_3rostopic4 = 5;
  String_rtsp_id = QString("http://192.168.50.120:8020/stream?topic=/usb_cam/image_rect_color"); //     urls_topic << "/usb_cam/image_rect_color";
  subRosTopic = QString("/rtsp_camera_relay/image");

	start();
	return true;
}


void rtsp_single_QNode::run()
{
  ros::Rate loop_rate(20);

  while ( ros::ok() )
  {
    ros::spinOnce();

    switch(flag_1rtsp2_3rostopic4)
    {
      case 1:
        receiveImage_sub.shutdown();
        flag_1rtsp2_3rostopic4 = 2;
      case 2:
        // vc("http://192.168.50.120:8020/stream?topic=/usb_cam/image_raw");
        vc.open(String_rtsp_id.toStdString());
        if (vc.isOpened())
        {
          vc.read(receiveImage);
          qImageFromBGRimage = MatToQImage(receiveImage);
          Q_EMIT receiveImageSignal();
        }
        break;

      case 3:
        receiveImage_sub.shutdown();
        // receiveImage_sub.operator=()
        receiveImage_sub = it->subscribe(subRosTopic.toStdString(),20,&rtsp_single_QNode::receiveImage_cb,this,image_transport::TransportHints("compressed"));
        // 订阅 图像信息
        flag_1rtsp2_3rostopic4 = 4;
      case 4:

      break;

    case 5:
      receiveImage_sub.shutdown();
      flag_1rtsp2_3rostopic4 = 6;
    case 6:

      break;
    }

		loop_rate.sleep();
  }

  std::cout << "[rtsp_qnode] Ros shutdown, proceeding to close the gui." << std::endl;
	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

// ---------------- 回调函数
//adding
void rtsp_single_QNode::receiveImage_cb(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        receiveImage = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::RGB8)->image;
        qImageFromBGRimage = QImage(receiveImage.data,receiveImage.cols,receiveImage.rows,receiveImage.step[0], QImage::Format_RGB888);
        Q_EMIT receiveImageSignal();
    }
    catch (cv_bridge::Exception& e)
    {
        std::cout << "sub1Image_cb could not convert from " << msg->encoding.c_str() << "to 'brg8'." << std::endl;
    }
}


QImage rtsp_single_QNode::MatToQImage(const cv::Mat &inMat)
{
  switch (inMat.type())
  {
    case CV_8UC4:		// 8-bit, 4 channel
    {
      QImage image(inMat.data,
        inMat.cols, inMat.rows,
        static_cast<int>(inMat.step),
        QImage::Format_ARGB32);

      return image;
    }

    case CV_8UC3:		// 8-bit, 3 channel
    {
      QImage image(inMat.data,
        inMat.cols, inMat.rows,
        static_cast<int>(inMat.step),
        QImage::Format_RGB888);
      return image.rgbSwapped();
    }


    case CV_8UC1:// 8-bit, 1 channel
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
      QImage image(inMat.data,
        inMat.cols, inMat.rows,
        static_cast<int>(inMat.step),
        QImage::Format_Grayscale8); //Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
#else
      static QVector<QRgb>  sColorTable;

      // only create our color table the first time
      if (sColorTable.isEmpty())
      {
        sColorTable.resize(256);

        for (int i = 0; i < 256; ++i)
        {
          sColorTable[i] = qRgb(i, i, i);
        }
      }

      QImage image(inMat.data,
        inMat.cols, inMat.rows,
        static_cast<int>(inMat.step),
        QImage::Format_Indexed8);

      image.setColorTable(sColorTable);
#endif
      return image;
    }

    default:
//			qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
      break;
  }

  return QImage();
}

}  // namespace map_widget
