#include "../include/map_widget/tracker_widget.hpp"

using namespace Qt;

kcfTracker_widget::kcfTracker_widget(int argc, char** argv, uint8_t id, QWidget *parent) :
  QWidget(parent),
  trackerThread(argc, argv, parent),
  ui(new Ui::kcfTracker_widget)
{
  ui->setupUi(this);
  this->setWindowTitle(QString::fromStdString("uav"+to_string(id)));

  QObject::connect(this,SIGNAL(selectCompleteSignal()),&trackerThread,SLOT(deal_selectCompleteSignal()));
  QObject::connect(this,SIGNAL(stopTrackSignal()),&trackerThread,SLOT(deal_stopTrackSignal()));
  QObject::connect(&trackerThread,SIGNAL(receiveImageSignal()),this,SLOT(deal_receiveImageSignal()));
  QObject::connect(&trackerThread, SIGNAL(rosShutdown()), this, SLOT(deal_rosShutdown()));

  QObject::connect(ui->graphicsView,SIGNAL(mouseMove_signal(QPoint)), this, SLOT(deal_mouseMove_signal(QPoint)));
  QObject::connect(ui->graphicsView,SIGNAL(mousePress_signal(QPoint)), this, SLOT(deal_mousePress_signal(QPoint)));
  QObject::connect(ui->graphicsView,SIGNAL(mouseRelease_signal(QPoint)), this, SLOT(deal_mouseRelease_signal(QPoint)));

  /* ************** 设置背景图片 ************** */
  ui->kcfTracker_image_label->setPixmap(QPixmap(":/images/load.png"));

  /* ************** 设置 QGraphicsView 控件 ************** */
  ui->graphicsView->setStyleSheet("background: transparent");
  // 关闭滑动条
  ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scene = new QGraphicsScene(this);
  // 重置坐标系原点为左上角
  scene->setSceneRect(0,0,ui->graphicsView->width()-1,ui->graphicsView->height()-1);
  RectItem = new QGraphicsRectItem();
  QPen Polygonpen;
  Polygonpen.setWidth(2);
  Polygonpen.setColor(Qt::red);
  RectItem->setPen(Polygonpen);
  QBrush Polygon_Brush;
  Polygon_Brush.setColor(QColor(177,177,177));
  RectItem->setBrush(Polygon_Brush);
  RectItem->setPos(0,0);
  scene->addItem(RectItem);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->show();

  // 显示图像自适应大小
  ui->kcfTracker_image_label->setScaledContents(true);

  if ( !trackerThread.init(id) )
    showNoMasterMessage();

  ui->lineEdit_ip->setText( QString::fromLocal8Bit( trackerThread.gcs_uavip.c_str() ) );
  ui->lineEdit_rostopic->setText( QString::fromLocal8Bit( trackerThread.uav_video_topic.c_str() ) );

  ui->lineEdit_uav->setText( QString("%1").arg(trackerThread.uavid) );

  // init
  state_rtsp_1on_2off = 2;

}

kcfTracker_widget::~kcfTracker_widget()
{
  if(trackerThread.isRunning())
  {
    trackerThread.trackerThreadStatus = false;
    trackerThread.quit();
    trackerThread.wait();
  }
  delete ui;
}

void kcfTracker_widget::on_kcfTrackerStart_pBtn_clicked()
{

  trackerThread.uavid = (uint8_t) ui->lineEdit_uav->text().toInt();

  // ui->lineEdit_uav->

  if(trackerThread.is_tracking == false)
  {
    if(trackerThread.trackRoi.width <= 1 || trackerThread.trackRoi.height <= 1)
    {
      QMessageBox::warning(NULL , QString::fromUtf8("提示"), QString::fromUtf8("选择区域太小！ "));
      return;
    }

    Q_EMIT selectCompleteSignal();
    // 清除框选
    // 发送追踪指令
    rect.setRect(0,0,1,1);
    RectItem->setRect(rect);
    trackerThread.is_tracking = true;
    trackerThread.trackerInit_flag = false;
    trackerThread.trackResult.state = 4;
    ui->kcfTrackerStart_pBtn->setText(QString::fromUtf8("停止追踪"));
    QObject::disconnect(ui->graphicsView,SIGNAL(mouseMove_signal(QPoint)), this, SLOT(deal_mouseMove_signal(QPoint)));
    QObject::disconnect(ui->graphicsView,SIGNAL(mousePress_signal(QPoint)), this, SLOT(deal_mousePress_signal(QPoint)));
    QObject::disconnect(ui->graphicsView,SIGNAL(mouseRelease_signal(QPoint)), this, SLOT(deal_mouseRelease_signal(QPoint)));
  }
  else { // trackerThread.is_tracking == true)
    // 取消 追踪按钮
    // 发送 取消追踪指令
    QObject::connect(ui->graphicsView,SIGNAL(mouseMove_signal(QPoint)), this, SLOT(deal_mouseMove_signal(QPoint)));
    QObject::connect(ui->graphicsView,SIGNAL(mousePress_signal(QPoint)), this, SLOT(deal_mousePress_signal(QPoint)));
    QObject::connect(ui->graphicsView,SIGNAL(mouseRelease_signal(QPoint)), this, SLOT(deal_mouseRelease_signal(QPoint)));
    trackerThread.is_tracking = false;
    trackerThread.trackRoi.width = 0;
    trackerThread.trackRoi.height = 0;
    ui->kcfTrackerStart_pBtn->setText(QString::fromUtf8("框选追踪"));
    Q_EMIT stopTrackSignal();
  }
}

void kcfTracker_widget::deal_receiveImageSignal()
{
  showImage_mutex_.lock();
  QImage qImg = trackerThread.qImageFromBGRimage.copy();
  ui->kcfTracker_image_label->setPixmap(QPixmap::fromImage(qImg));
  showImage_mutex_.unlock();

  // TODO 飞机ID 和 追踪目标ID 判断
  if (trackerThread.is_tracking )//&& trackerThread.trackResult.sysid == trackerThread.uavid )
  {
    if(trackerThread.trackResult.state == 0)//追踪失败
    {
      trackerThread.is_tracking = false;
      ui->trackState_label->setStyleSheet("background: rgb(255,0,0)");
      ui->label_track_state->setText( QString("追踪失败") );
      // 红色 追踪失败
      QObject::connect(ui->graphicsView,SIGNAL(mouseMove_signal(QPoint)), this, SLOT(deal_mouseMove_signal(QPoint)));
      QObject::connect(ui->graphicsView,SIGNAL(mousePress_signal(QPoint)), this, SLOT(deal_mousePress_signal(QPoint)));
      QObject::connect(ui->graphicsView,SIGNAL(mouseRelease_signal(QPoint)), this, SLOT(deal_mouseRelease_signal(QPoint)));
      trackerThread.is_tracking = false;
      trackerThread.trackRoi.width = 0;
      trackerThread.trackRoi.height = 0;
      ui->kcfTrackerStart_pBtn->setText(QString::fromUtf8("框选追踪"));
    }
    if(trackerThread.trackResult.state == 1)
    {
      ui->trackState_label->setStyleSheet("background: rgb(0,255,0)");
      ui->label_track_state->setText( QString("追踪中") );
      // 绿色 正在追踪
    }
    if(trackerThread.trackResult.state == 2)
    {
      ui->trackState_label->setStyleSheet("background: rgb(255,255,0)");
      ui->label_track_state->setText( QString("追踪误差较大") );
      // 黄色 追踪误差较大
    }
    if(trackerThread.trackResult.state == 3)
    {
      ui->trackState_label->setStyleSheet("background: rgb(0,255,255)");
      ui->label_track_state->setText( QString("已定位，并发送给其他无人机") );
      // 已定位，并发送给其他无人机
    }
    if(trackerThread.trackResult.state == 4)
    {
      ui->trackState_label->setStyleSheet("background: rgb(0, 0, 255)");
      ui->label_track_state->setText( QString("已发送框选，等待无人机响应") );
      // 已发送框选，等待无人机响应
    }
  }
  else
  {
    ui->trackState_label->setStyleSheet("background: rgb(186, 189, 182)");
    ui->label_track_state->setText( QString("未设置追踪区域") );
  }
}

/* ************** 鼠标信号处理函数************** */
void kcfTracker_widget::deal_mouseMove_signal(QPoint point)
{
  if(trackerThread.mousePress)
  {
     double err_x,err_y;
     err_x = point.x() - trackerThread.mousePosition_start.x();
     err_y = point.y() - trackerThread.mousePosition_start.y();
     rect.setRect(trackerThread.mousePosition_start.x(),trackerThread.mousePosition_start.y(),err_x,err_y);

    RectItem->setRect(rect);
    trackerThread.mousePosition_current = point;
  }
}

void kcfTracker_widget::deal_mousePress_signal(QPoint point)
{
  trackerThread.mousePosition_start = point;
  trackerThread.mousePress = true;
  //计算缩放比例
  double scale_x =trackerThread.qImageFromBGRimage.width()*1.0 / ui->graphicsView->width();
  double scale_y =trackerThread.qImageFromBGRimage.height()*1.0 / ui->graphicsView->height();

  trackerThread.trackRoi.x = point.x() * scale_x;
  trackerThread.trackRoi.y = point.y() * scale_y;
}

void kcfTracker_widget::deal_mouseRelease_signal(QPoint point)
{
  trackerThread.mousePosition_current = point;
  trackerThread.mousePress = false;
  //计算缩放比例
  double scale_x =trackerThread.qImageFromBGRimage.width()*1.0 / ui->graphicsView->width();
  double scale_y =trackerThread.qImageFromBGRimage.height()*1.0 / ui->graphicsView->height();

  trackerThread.trackRoi.width = (point.x() - trackerThread.mousePosition_start.x()) * scale_x;
  trackerThread.trackRoi.height = (point.y() - trackerThread.mousePosition_start.y()) * scale_y;
}

/* ************** Help 函数************** */
void kcfTracker_widget::closeEvent(QCloseEvent *event)
{
  if(trackerThread.isRunning())
  {
    trackerThread.trackerThreadStatus = false;
    trackerThread.quit();
    trackerThread.wait();
  }
}

void kcfTracker_widget::showNoMasterMessage() {
  QMessageBox msgBox;
  msgBox.setText("Couldn't find the ros master.");
  msgBox.exec();
}

void kcfTracker_widget::deal_rosShutdown()
{
  if(trackerThread.isRunning())
  {
    trackerThread.quit();
    trackerThread.wait();
  }
  cout << "KcfTracker ROS_node Shutdown ! " << endl;
}

/* ************** gcs 相关函数************** */
// gcs 开始 开关
void kcfTracker_widget::on_show_video_clicked()
{
  if ( state_rtsp_1on_2off == 1)
  { // rtsp 设置为关闭状态
    state_rtsp_1on_2off = 2;
    trackerThread.flag_rtsp_1run = 0;
    ui->show_video->setText( "显示视频" );
  } else
  {
    // rtsp 设置为传输状态
    QString temp1, temp2, temp3, temp4;
    temp1 = QString("http://");
    temp2 = ui->lineEdit_ip->text();
    temp3 = QString(":8020/stream?topic=");
    temp4 = ui->lineEdit_rostopic->text();
    trackerThread.String_rtsp_id =  temp1 + temp2 + temp3 + temp4;

    state_rtsp_1on_2off = 1;
    trackerThread.flag_rtsp_1run = 1;
    ui->show_video->setText( "暂停视频" );
  }
}
