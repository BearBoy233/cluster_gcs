#include "../include/map_widget/trackerThread.hpp"

// using namespace std;
using namespace cv;

tracker_thread::tracker_thread(int argc, char** argv, QWidget *parent)
  : QThread(parent),
    init_argc(argc),
    init_argv(argv)
{
  mousePosition_start.setX(0);
  mousePosition_start.setY(0);
  mousePosition_current.setX(0);
  mousePosition_current.setY(0);

  mousePress = false;
  is_tracking = false;
  trackerInit_flag = false;
  trackerThreadStatus = true;
  recImage_flag = false;

  trackResult.state = 4;

  trackRoi.target_no = 1;
}

tracker_thread::~tracker_thread() {
  if(ros::isStarted()) {
    ros::shutdown(); // explicitly needed since we use ros::start();
    ros::waitForShutdown();
  }
wait();
}

// 订阅追踪结果
void tracker_thread::trackerResult_cb(const mavcomm_msgs::kcf_target_pos& msg)
{
  // TODO 飞机ID 和 追踪目标ID 判断
  // if (msg.sysid == uavid)
  {
    trackResult.state = msg.state;
  }
}

bool tracker_thread::init(std::uint8_t uavID)
{
  uavid = uavID;

  ros::init(init_argc,init_argv, "kcf_widget_node");
  if ( ! ros::master::check() )
  {
    return false;
  }

  ros::NodeHandle nh;// 第一次创建节点时会自动调用start()
  image_transport::ImageTransport it(nh);

  nh.param<int>("uav1_gcs2", uav1_gcs2, 20);
  nh.param<std::string>("gcs_uavip", gcs_uavip,"192.168.50.132");
  nh.param<std::string>("uav_video_topic", uav_video_topic, "/kcfTracker/image_raw");

  std::cout << "uav1_gcs2 = " << uav1_gcs2 << std::endl;
  std::cout << "gcs_uavip = " << gcs_uavip << std::endl;
  std::cout << "uav_video_topic = " << uav_video_topic << std::endl;

  // init
  String_rtsp_id = QString("http://192.168.50.120:8020/stream?topic=/kcfTracker/image_raw");

  // 订阅 图像信息
  if (uav1_gcs2 == 1)
  {
    // 测试 飞机端
    receiveImage_sub = it.subscribe(uav_video_topic , 5, &tracker_thread::receiveImage_cb, this, image_transport::TransportHints("compressed"));
    // 发布 候选框
    trackRoi_pub= nh.advertise<mavcomm_msgs::kcf_set_target>("/mavcomm/receive/kcf_set_target", 10);
    // 订阅 追踪结果
    trackerResult_sub = nh.subscribe("/mavcomm/send/kcf_target_pos", 1, &tracker_thread::trackerResult_cb, this);
  }
  else if (uav1_gcs2 == 2)
  {
    // 发布 候选框
    trackRoi_pub= nh.advertise<mavcomm_msgs::kcf_set_target>("/mavcomm/send/kcf_set_target", 10);
    // 订阅 追踪结果
    trackerResult_sub = nh.subscribe("/mavcomm/receive/kcf_target_pos", 1, &tracker_thread::trackerResult_cb, this);

    // 地面端
    // QString String_rtsp_id;
    QString temp1, temp2, temp3, temp4;
    temp1 = QString("http://");
    temp2 = QString(QString::fromLocal8Bit(gcs_uavip.c_str()));
    temp3 = QString(":8020/stream?topic=");
    temp4 = QString(QString::fromLocal8Bit(uav_video_topic.c_str()));

    String_rtsp_id = temp1 + temp2 + temp3 + temp4;

    std::cout << "GCS_rtsp_set = " << String_rtsp_id.toStdString() << std::endl;
  }

  start();//开启线程 自动调用run()函数

  return true;
}

void tracker_thread::run()
{
  ros::Rate loop_rate(20);
  while(ros::ok())
  {
    //    cout << "Thread is ok " << QThread::currentThreadId() << endl;
    //    msleep(1000);  // 33.3Hz
    ros::spinOnce();

    if (uav1_gcs2 == 2)
    {
      // vc("http://192.168.50.120:8020/stream?topic=/usb_cam/image_raw");
      rtsp_video();
    }

    loop_rate.sleep();

    // 防止结束线程时死机
    if(!trackerThreadStatus)
      break;
  }
  Q_EMIT rosShutdown();
}

// 地面站 发送框选 xy坐标 指令
void tracker_thread::deal_selectCompleteSignal()
{
  trackRoi.header.stamp = ros::Time::now();

  trackRoi.sysid = 100;
  trackRoi.compid = uavid;

  if ( trackRoi.target_no == 255)
  {
    trackRoi.target_no = 1;
  }
  else
  {
    trackRoi.target_no = trackRoi.target_no + 1;
  }

  trackRoi.state = 1;
  trackRoi_pub.publish(trackRoi);
}

// 地面站 发送 停止 track 指令
void tracker_thread::deal_stopTrackSignal()
{
  trackRoi.header.stamp = ros::Time::now();

  trackRoi.sysid = 100;
  trackRoi.compid = uavid;

  trackRoi.state = 0;

  trackRoi_pub.publish(trackRoi);
}

// ROS 图像订阅
void tracker_thread::receiveImage_cb(const sensor_msgs::ImageConstPtr& msg)
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

void tracker_thread::rtsp_video()
{
  // vc("http://192.168.50.132:8020/stream?topic=/usb_cam/image_raw");
  if ( flag_rtsp_1run == 1 )
  {
    vc.open( String_rtsp_id.toStdString() );
    if (vc.isOpened())
    {
      vc.read(receiveImage);
      qImageFromBGRimage = MatToQImage(receiveImage);
      Q_EMIT receiveImageSignal();
    }
  }
}

QImage tracker_thread::MatToQImage(const cv::Mat &inMat)
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
      // qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
      break;
  }

  return QImage();
}
