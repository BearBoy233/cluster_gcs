
#include "../include/map_widget/rtsp_single_widget.hpp"

namespace map_widget {

using namespace Qt;

rtsp_single_widget::rtsp_single_widget(int argc, char** argv, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rtsp_single_widget),
    rtsp_single_qnode(argc,argv)
{
    ui->setupUi(this);

    // Init

    urls_ip << "";
    urls_ip << "192.168.50.131:8020";
    urls_ip << "192.168.50.132:8020";
    urls_ip << "192.168.50.133:8020";
    ui->comboBox_ip->addItems(urls_ip);
    ui->comboBox_ip->setCurrentIndex(0);

    urls_topic << "";
    urls_topic << "/usb_cam/image_rect_color";
    urls_topic << "/usb_cam/image_raw";
    urls_topic << "/camera/color/image_raw";
    urls_topic << "/csi_cam_0/image_raw";
    ui->comboBox_topic->addItems(urls_topic);
    ui->comboBox_topic->setCurrentIndex(0);

    urls_rtsp = QString("http://192.168.50.131:8020/stream?topic=/usb_cam/image_raw");
    ui->label_rtsp_final->setText(urls_rtsp);

    urls_rostopic << "";
    urls_rostopic << "/rtsp_camera_relay/image";
    urls_rostopic << "/usb_cam/image_rect_color";
    urls_rostopic << "/usb_cam/image_raw";
    urls_rostopic << "/camera/color/image_raw";
    ui->comboBox_rostopic->addItems(urls_rostopic);
    ui->comboBox_rostopic->setCurrentIndex(0);

    ui->radioButton_rtsp->setChecked(1);
    ui->radioButton_rostopic->setChecked(0);
    ui->comboBox_rostopic->setEnabled(0);

    state_1rtsp_0rostopic = 1;

    initConncetion(); //初始化槽函数
    rtsp_single_qnode.init();

}

rtsp_single_widget::~rtsp_single_widget()
{
    delete ui;
}


// #########################################################################
//  Qt slots
// #########################################################################
// 初始化信号槽函数
void rtsp_single_widget::initConncetion()
{
   // rostopic 图像更新
   QObject::connect(&rtsp_single_qnode,SIGNAL(receiveImageSignal()),this,SLOT(deal_receiveImageSignal()));
}

void rtsp_single_widget::deal_receiveImageSignal()
{
  // if (!flag_use_yolo)
  {
    showImage_mutex_.lock();
    QImage qImg = rtsp_single_qnode.qImageFromBGRimage.copy();

    //放缩图片，以固定大小显示
    qImg = qImg.scaled(ui->camera_image_label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->camera_image_label->setPixmap(QPixmap::fromImage(qImg));
    showImage_mutex_.unlock();
  }
}

// 中断 开关
void rtsp_single_widget::on_pushButton_pause_clicked()
{
  rtsp_single_qnode.flag_1rtsp2_3rostopic4 = 5;
}

// 开始 开关
void rtsp_single_widget::on_pushButton_start_clicked()
{
  if ( state_1rtsp_0rostopic)
  { // rtsp
    rtsp_single_qnode.String_rtsp_id = ui->label_rtsp_final->text();
    rtsp_single_qnode.flag_1rtsp2_3rostopic4 = 1;
  } else
  { // RosTopic
    rtsp_single_qnode.subRosTopic = ui->comboBox_rostopic->currentText().trimmed();
    rtsp_single_qnode.flag_1rtsp2_3rostopic4 = 3;
  }

}

// rtsp
void rtsp_single_widget::on_radioButton_rtsp_clicked()
{
  ui->comboBox_topic->setEnabled(1);
  ui->comboBox_ip->setEnabled(1);
  ui->label_rtsp_final->setEnabled(1);

  ui->radioButton_rostopic->setChecked(0);
  ui->comboBox_rostopic->setEnabled(0);
  state_1rtsp_0rostopic = 1;
}

// rostopic
void rtsp_single_widget::on_radioButton_rostopic_clicked()
{
  ui->radioButton_rtsp->setChecked(0);
  ui->comboBox_topic->setEnabled(0);
  ui->comboBox_ip->setEnabled(0);
  ui->label_rtsp_final->setEnabled(0);

  ui->comboBox_rostopic->setEnabled(1);
  state_1rtsp_0rostopic = 0;
}

void rtsp_single_widget::on_comboBox_topic_currentIndexChanged(const QString &arg)
{
  QString Stemp1 = ui->comboBox_ip->currentText().trimmed();
  QString Stemp2 = ui->comboBox_topic->currentText().trimmed();
  urls_rtsp = rtsp_Temp1 + Stemp1 + rtsp_Temp2 + Stemp2;

  ui->label_rtsp_final->setText(urls_rtsp);
}

void rtsp_single_widget::on_comboBox_ip_currentIndexChanged(const QString &arg)
{
  QString Stemp1 = ui->comboBox_ip->currentText().trimmed();
  QString Stemp2 = ui->comboBox_topic->currentText().trimmed();
  urls_rtsp = rtsp_Temp1 + Stemp1 + rtsp_Temp2 + Stemp2;

  ui->label_rtsp_final->setText(urls_rtsp);
}


}
