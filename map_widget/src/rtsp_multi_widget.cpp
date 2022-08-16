
#include "../include/map_widget/rtsp_multi_widget.hpp"

namespace map_widget {

using namespace Qt;

// -------------------------------------------------------------------------
//  rtsp_multi_widget 初始化声明
// -------------------------------------------------------------------------
rtsp_multi_widget::rtsp_multi_widget(int argc, char** argv, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::rtsp_multi_widget)
  // , rtsp_multi_qnode(argc,argv)
{
  ui->setupUi(this);

  // 获取程序所在路径，切换到同目录下的
  htmlPath = QCoreApplication::applicationDirPath();
  temp_path_head = "cq_gcs";
  int temp_path_1 = htmlPath.indexOf(temp_path_head);
  temp_path_head = htmlPath.left(temp_path_1) + "cq_gcs/src/map_widget/resources/images/";

  //--------------------------------------------------------------------------
  initConncetion(); //初始化槽函数
  init_showgrpah(); //初始化界面图标

  // 设置 子Widget
  ui->horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);

  for (int iii=1; iii<MMM ;iii++)
  {
    test_single[iii] = new rtsp_single_widget(argc,argv);
    ui->horizontalLayout->addWidget(test_single[iii]);
  }
  ui->scrollAreaWidgetContents->setMinimumWidth( 340*(MMM-1) + 40 );

}

rtsp_multi_widget::~rtsp_multi_widget()
{
  delete ui;
}

// #########################################################################
//                                                                  Qt slots
// #########################################################################

// 初始化信号槽函数
void rtsp_multi_widget::initConncetion()
{

}

void rtsp_multi_widget::init_showgrpah()
{

}

// -------------------------------------------------------------------------
//                                                          ROS solt 链接信号
// -------------------------------------------------------------------------
// qt gui 界面数据刷新
void rtsp_multi_widget::slot_power( )
{

}


// -------------------------------------------------------------------------
//                                                             Help Function
// -------------------------------------------------------------------------



} // namespace   Ui
