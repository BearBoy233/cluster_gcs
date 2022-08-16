
#include "../include/map_widget/mission_widget.hpp"

#define NNN 11

namespace map_widget {

using namespace Qt;
// , mission_qnode(argc,argv)

// -------------------------------------------------------------------------
//  mission_widget 初始化声明
// -------------------------------------------------------------------------
mission_widget::mission_widget(int argc, char** argv, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::mission_widget)
  , mission_qnode(argc,argv)
{
  ui->setupUi(this);

  //-----------------------------
  flag_drone_no = true;
  ui->groupBox_mission->setEnabled(0);
  ui->groupBox_mission_pos->setEnabled(0);

  init_comboBox_select();

  initConncetion(); //初始化槽函数

  initgridLayout();

  mission_qnode.init();

}

mission_widget::~mission_widget()
{
  delete ui;
}

// #########################################################################
//  Qt slots
// #########################################################################
void mission_widget::init_comboBox_select()
{
  QStringList temp_items1;
  temp_items1 << "mavros";
  temp_items1 << "vision_pose";
  temp_items1 << "usb_cam";
  temp_items1 << "csi_cam";
  temp_items1 << "realsense_cam";

  ui->comboBox_select->addItems(temp_items1);
  ui->comboBox_select->setCurrentIndex(0);
}

// 初始化信号槽函数
void mission_widget::initConncetion()
{
   // --- ROS ---
   // mission_QNode 获取界面信息更新
    connect( &mission_qnode, SIGNAL( power() ), this, SLOT( slot_power() ) );

    connect( &mission_qnode, SIGNAL( power_heart() ), this, SLOT( slot_power_heart() ) );

}

// 初始化 gridLayout 显示模块
void mission_widget::initgridLayout()
{
  ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);

  for (int iii=1; iii<NNN; iii++)
  {
    for (int hhh=1; hhh<HHH; hhh++)
    {
      mission_single_ui[iii][hhh] = new mission_single_widget;
      setLED(mission_single_ui[iii][hhh]->ui->label_led, 0, 11);// 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      mission_single_ui[iii][hhh]->led_state = 0;

      connect(mission_single_ui[iii][hhh]->ui->pushButton, &QPushButton::clicked, this, [=](){slot_pub_ros_mission(iii, hhh);});
      connect(mission_single_ui[iii][hhh]->ui->pushButton_cancel, &QPushButton::clicked, this, [=](){slot_pub_ros_mission_cancel(iii, hhh);});
      // connect(btn, &QPushButton::clicked, this, [=](){func(i);});
      // connect(mission_single_ui[iii][hhh]->ui->pushButton,&QPushButton::clicked,(this)[]{m_obj->m_slot(iii，jjj);});
      // connect(mission_single_ui[iii][hhh]->ui->pushButton,&QPushButton::clicked,(this)[]{slot_pub_ros_mission(iii, jjj);});
      // connect(btn,&QPushButton::clicked,(this)[]{m_obj->m_slot(true，false);});
      // btn是按钮，m_obj是对象，m_slot是m_obj的槽函数，可以带参，true和false都是传入m_slot的参数。
      ui->gridLayout->addWidget(mission_single_ui[iii][hhh], hhh,iii);
      mission_single_ui[iii][hhh]->hide();
    }
    current_num_mission_single[iii] = 2;
  }

  for (int iii=1; iii<NNN; iii++)
  {
    int hhh=1;
    mission_single_ui[iii][hhh]->ui->label_node->setText( QString("mavros") );
    mission_single_ui[iii][hhh]->ui->label_param->setText( QString("THS0,921600") );
    mission_single_ui[iii][hhh]->ui->pushButton_cancel->setVisible(0);

    mission_single_ui[iii][hhh]->rosmsg.sysid = 100;  //发送方ID
    mission_single_ui[iii][hhh]->rosmsg.compid = iii; //接收方ID
    mission_single_ui[iii][hhh]->rosmsg.command = mavros;
    mission_single_ui[iii][hhh]->rosmsg.flag = 1;
    mission_single_ui[iii][hhh]->rosmsg.type1 = 3;
    mission_single_ui[iii][hhh]->rosmsg.type2 = 2;

    mission_single_ui[iii][hhh]->show();
  }

  for (int iii=1; iii<NNN; iii++)
  {
    int hhh=2;
    mission_single_ui[iii][hhh]->ui->label_node->setText( QString("vision_pose") );
    mission_single_ui[iii][hhh]->ui->label_param->setText( QString("vrpn, Nan") );
    mission_single_ui[iii][hhh]->ui->pushButton_cancel->setVisible(0);

    mission_single_ui[iii][hhh]->rosmsg.sysid = 100;  //发送方ID
    mission_single_ui[iii][hhh]->rosmsg.compid = iii; //接收方ID
    mission_single_ui[iii][hhh]->rosmsg.command = vision_pose;
    mission_single_ui[iii][hhh]->rosmsg.flag = 1;
    mission_single_ui[iii][hhh]->rosmsg.type1 = 1;
    mission_single_ui[iii][hhh]->rosmsg.type2 = 1;

    mission_single_ui[iii][hhh]->show();
  }
}

// -------------------------------------------------------------------------
//                                                          ROS solt 链接信号
// -------------------------------------------------------------------------
void mission_widget::slot_pub_ros_mission_cancel(int iii, int jjj)
{
  int temp_n;
  temp_n = current_num_mission_single[iii];

  for (int h=jjj; h<temp_n;h++)
  {
    mission_single_ui[iii][h]->rosmsg = mission_single_ui[iii][h+1]->rosmsg;
    mission_single_ui[iii][h]->flag_button_clicked = mission_single_ui[iii][h+1]->flag_button_clicked;
    mission_single_ui[iii][h]->led_state = mission_single_ui[iii][h+1]->led_state;

    mission_single_ui[iii][h]->ui->label_node->setText( mission_single_ui[iii][h+1]->ui->label_node->text() );
    mission_single_ui[iii][h]->ui->label_param->setText( mission_single_ui[iii][h+1]->ui->label_param->text() );
    setLED(mission_single_ui[iii][h]->ui->label_led, mission_single_ui[iii][h]->led_state, 11);// 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow

    if (mission_single_ui[iii][h]->flag_button_clicked==false)
    { // Set 状态
      mission_single_ui[iii][h]->ui->pushButton_cancel->setVisible(1);
      mission_single_ui[iii][h]->ui->pushButton->setText( QString("Set") );
    }
    else
    { // kill 状态
      mission_single_ui[iii][h]->ui->pushButton_cancel->setVisible(0);
      mission_single_ui[iii][h]->ui->pushButton->setText( QString("Kill") );
    }
  }

  current_num_mission_single[iii] = temp_n-1 ;
  mission_single_ui[iii][temp_n]->hide();
}

void mission_widget::slot_pub_ros_mission(int iii, int jjj)
{
    mission_qnode.cb_pub_mission( mission_single_ui[iii][jjj]->rosmsg );

    if (mission_single_ui[iii][jjj]->flag_button_clicked==true){
      // 重置于 reset
      if (jjj>2)
      { mission_single_ui[iii][jjj]->ui->pushButton_cancel->setVisible(1);
      }
      mission_single_ui[iii][jjj]->flag_button_clicked = false;
      mission_single_ui[iii][jjj]->ui->pushButton->setText( QString("Set") );
      mission_single_ui[iii][jjj]->rosmsg.flag = 1;
      setLED(mission_single_ui[iii][jjj]->ui->label_led, 3, 11);// 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      mission_single_ui[iii][jjj]->led_state = 3;
    }
    else {
      // 置于 kill
      if (jjj>2)
      { mission_single_ui[iii][jjj]->ui->pushButton_cancel->setVisible(0);
      }
      mission_single_ui[iii][jjj]->flag_button_clicked = true;
      mission_single_ui[iii][jjj]->ui->pushButton->setText( QString("Kill") );
      mission_single_ui[iii][jjj]->rosmsg.flag = 3;
      setLED(mission_single_ui[iii][jjj]->ui->label_led, 2, 11);// 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      mission_single_ui[iii][jjj]->led_state = 2;
    }
}

// qt gui 界面数据刷新
void mission_widget::slot_power( )
{

}

// qt gui 无人机 已连接编号 刷新
void mission_widget::slot_power_heart( )
{
  // 列表添加无人机编号选项
  ui->comboBox_no->clear();
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    if (mission_qnode.Flag_Data_heartbeat[temp_i] == true)
    {
      if (flag_drone_no)
      {
        flag_drone_no = false;
        ui->groupBox_mission->setEnabled(1);
        ui->groupBox_mission_pos->setEnabled(1);
      }

      ui->comboBox_no->addItem( QString("%1").arg(temp_i) );
      ui->comboBox_no_pos->addItem( QString("%1").arg(temp_i) );
    }
  }
}

void mission_widget::on_pushButton_add_clicked()
{
  int iii, hhh;
  // ui->comboBox_no->currentText().toInt()
  iii = ( ui->comboBox_no->currentText().toInt() );
  if (iii >= NNN || iii < 1)
  {
    std::cout << "Error in mission_widget: iii<1 or iii>NNN" << std::endl;
  } else
  {
    hhh = current_num_mission_single[iii] + 1;
    if (hhh >= HHH)
    {
      std::cout << "Error in mission_widget: Reach Max HHH" << std::endl;
    }  else
    {
      current_num_mission_single[iii] = hhh;

      mission_single_ui[iii][hhh]->ui->label_node->setText( ui->comboBox_select->currentText() );

      mission_single_ui[iii][hhh]->rosmsg.sysid = 100;  //发送方ID
      mission_single_ui[iii][hhh]->rosmsg.compid = iii; //接收方ID
      mission_single_ui[iii][hhh]->rosmsg.command = ui->comboBox_select->currentIndex();
      mission_single_ui[iii][hhh]->rosmsg.flag = 1;
      mission_single_ui[iii][hhh]->rosmsg.type1 = ui->comboBox_type1->currentIndex() + 1; //注意 qt Index 从0开始

      if  ( flag_mission_type_same[ ui->comboBox_select->currentIndex()] == 1 )
      {
        mission_single_ui[iii][hhh]->rosmsg.type2 = mission_single_ui[iii][hhh]->rosmsg.type1;  // flag_mission_type_same == 1
        mission_single_ui[iii][hhh]->ui->label_param->setText( ui->comboBox_type1->currentText() );
      }
      else
      {
        mission_single_ui[iii][hhh]->rosmsg.type2 = ui->comboBox_type2->currentIndex() + 1; //注意 qt Index 从0开始
        mission_single_ui[iii][hhh]->ui->label_param->setText( ui->comboBox_type1->currentText() + ", " + ui->comboBox_type2->currentText() );
      }

      //置于 Set
      mission_single_ui[iii][hhh]->ui->pushButton_cancel->setVisible(1);
      mission_single_ui[iii][hhh]->flag_button_clicked = false;
      mission_single_ui[iii][hhh]->ui->pushButton->setText( QString("Set") );
      mission_single_ui[iii][hhh]->rosmsg.flag = 1;
      setLED(mission_single_ui[iii][hhh]->ui->label_led, 0, 11);// 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      mission_single_ui[iii][hhh]->led_state = 0;

      mission_single_ui[iii][hhh]->show();
      // ui->gridLayout->addWidget(mission_single_ui[iii][hhh], hhh,iii);
    }
  }
}

void mission_widget::on_comboBox_select_currentTextChanged()
{
  int ii;
  ii = ui->comboBox_select->currentIndex();

  switch (ii) {
    case mavros: //mavros
      index_func_mavros();
    break;

    case vision_pose: //vision_pose
      index_func_vision_pose();
    break;

    case usb_cam: //usb_cam
      index_func_usb_cam();
    break;

    case csi_cam: //csi_cam_0
      index_func_csi_cam();
    break;

    case realsense_cam: //realsense_cam
      index_func_realsense_cam();
    break;
  }
}


void mission_widget::index_func_mavros()
{
  QStringList temp_items1, temp_items2;
  temp_items1 << "ACM0";
  temp_items1 << "USB0";
  temp_items1 << "THS0";
  temp_items1 << "THS1";
  temp_items1 << "THS2";

  temp_items2 << "57600";
  temp_items2 << "921600";

  ui->comboBox_type1->clear();
  ui->comboBox_type1->addItems(temp_items1);
  ui->comboBox_type1->setCurrentIndex(0);

  ui->comboBox_type2->clear();
  ui->comboBox_type2->addItems(temp_items2);
  ui->comboBox_type2->setCurrentIndex(0);
}

void mission_widget::index_func_vision_pose()
{
  QStringList temp_items1, temp_items2;
  temp_items1 << "Vrpn";
  temp_items1 << "VIO";
  temp_items1 << "Vrpn+VIO";

  temp_items2 << "/camera/odom/sample";
  temp_items2 << "/vins_fusion/odometry";

  ui->comboBox_type1->clear();
  ui->comboBox_type1->addItems(temp_items1);
  ui->comboBox_type1->setCurrentIndex(0);

  ui->comboBox_type2->clear();
  ui->comboBox_type2->addItems(temp_items2);
  ui->comboBox_type2->setCurrentIndex(0);
}

void mission_widget::index_func_usb_cam()
{
  QStringList temp_items1, temp_items2;
  temp_items1 << "640 480";
  temp_items1 << "960 720";
  temp_items1 << "1440 1080";

  temp_items2 << "Nan";

  ui->comboBox_type1->clear();
  ui->comboBox_type1->addItems(temp_items1);
  ui->comboBox_type1->setCurrentIndex(0);

  ui->comboBox_type2->clear();
  ui->comboBox_type2->addItems(temp_items2);
  ui->comboBox_type2->setCurrentIndex(0);
}

void mission_widget::index_func_csi_cam()
{
  QStringList temp_items1, temp_items2;
  temp_items1 << "1080 720";
  temp_items1 << "1920 1080";
  temp_items1 << "3264 2464";

  temp_items2 << "Nan";

  ui->comboBox_type1->clear();
  ui->comboBox_type1->addItems(temp_items1);
  ui->comboBox_type1->setCurrentIndex(0);

  ui->comboBox_type2->clear();
  ui->comboBox_type2->addItems(temp_items2);
  ui->comboBox_type2->setCurrentIndex(0);
}

void mission_widget::index_func_realsense_cam()
{
  QStringList temp_items1, temp_items2;
  temp_items1 << "Nan";

  temp_items2 << "Nan";

  ui->comboBox_type1->clear();
  ui->comboBox_type1->addItems(temp_items1);
  ui->comboBox_type1->setCurrentIndex(0);

  ui->comboBox_type2->clear();
  ui->comboBox_type2->addItems(temp_items2);
  ui->comboBox_type2->setCurrentIndex(0);
}



// -------------------------------------------------------------------------
//                                                             Help Function
// -------------------------------------------------------------------------
void mission_widget::setLED(QLabel* label, int color, int size)
{
    // 将label中的文字清空
    label->setText("");
    // 先设置矩形大小
    // 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
    // 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
    QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
    QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
    QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
    QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
    // 再设置边界形状及边框
    QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
    QString border = QString("border:1px solid black;");                    // 边框为1px黑色
    // 最后设置背景颜色
    QString background = "background-color:";
    switch (color) {
    case 0:   // 灰色
        background += "rgb(190,190,190)";
        break;
    case 1:   // 红色
        background += "rgb(200,0,0)";
        break;
    case 2:   // 绿色
        background += "rgb(0,200,0)";
        break;
    case 3:   // 黄色
        background += "rgb(200,200,0)";
        break;
    default:
        break;
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    label->setStyleSheet(SheetStyle);
}

} // namespace   Ui
