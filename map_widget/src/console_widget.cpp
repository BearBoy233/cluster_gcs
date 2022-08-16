
#include "../include/map_widget/console_widget.hpp"

#define NNN 11

namespace map_widget {

using namespace Qt;
// , console_qnode(argc,argv)

// -------------------------------------------------------------------------
//  console_widget 初始化声明
// -------------------------------------------------------------------------
console_widget::console_widget(int argc, char** argv, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::console_widget)
  , console_qnode(argc,argv)
{
  ui->setupUi(this);

  //-----------------------------
  flag_drone_no = true;
  ui->groupBox_console->setEnabled(0);
  ui->groupBox_console_pos->setEnabled(0);
  ui->groupBox_statemachine->setEnabled(0);

  init_comboBox_select();

  initConncetion(); //初始化槽函数

  initgridLayout();

  console_qnode.init();

}

console_widget::~console_widget()
{
  delete ui;
}

// #########################################################################
//                                                                  Qt slots
// #########################################################################
void console_widget::init_comboBox_select()
{
  QStringList temp_items1;
  temp_items1 << "mavros";
  temp_items1 << "uav_ctrl";
  temp_items1 << "vision_pose";
  temp_items1 << "usb_cam";
  temp_items1 << "csi_cam";
  temp_items1 << "realsense_cam";
  temp_items1 << "web_video_server";
  temp_items1 << "vins";
  temp_items1 << "fast_planner";
  temp_items1 << "yolov3";
  ui->comboBox_select->clear();
  ui->comboBox_select->addItems(temp_items1);
  ui->comboBox_select->setCurrentIndex(0);

  // 无人机状态 changestate /uav_ctrl.cpp#enum FlightState
  // 需要同步修改 px4_ctrl.cpp & console_widget.hpp/cpp & state_single_widget.hpp
  QStringList temp_items2;
  temp_items2 << "IDLE";
  temp_items2 << "TAKING_OFF";
  temp_items2 << "LANDING";
  temp_items2 << "POS_EXECUTION";
  temp_items2 << "FORMATION_FLY";
  ui->comboBox_statemachine_nodestate->clear();
  ui->comboBox_statemachine_nodestate->addItems(temp_items2); // 转为rosmsg需要手动 + 1
  ui->comboBox_statemachine_nodestate->setCurrentIndex(0);

  QStringList temp_items3;
  temp_items3 << "Nan";
  ui->comboBox_statemachine_param->clear();
  ui->comboBox_statemachine_param->addItems(temp_items3); // 转为rosmsg需要手动 + 1
  ui->comboBox_statemachine_param->setCurrentIndex(0);
}

// 初始化信号槽函数
void console_widget::initConncetion()
{
   // --- ROS ---
   // console_QNode 获取界面信息更新
    connect( &console_qnode, SIGNAL( power() ), this, SLOT( slot_power() ) );

    connect( &console_qnode, SIGNAL( power_heart() ), this, SLOT( slot_power_heart() ) );

    connect( &console_qnode, SIGNAL( power_rosnode() ), this, SLOT( slot_power_rosnode() ) );

}

// 初始化 gridLayout 显示模块
void console_widget::initgridLayout()
{
  ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);

  for (int iii=1; iii<NNN; iii++)
  {
    for (int hhh=1; hhh<HHH; hhh++)
    {
      console_single_ui[iii][hhh] = new console_single_widget;
      console_single_ui[iii][hhh]->led_state = 0;// 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      setLED(console_single_ui[iii][hhh]->ui->label_led, console_single_ui[iii][hhh]->led_state, 11);

      connect(console_single_ui[iii][hhh]->ui->pushButton, &QPushButton::clicked, this, [=](){slot_pub_ros_console(iii, hhh);});
      connect(console_single_ui[iii][hhh]->ui->pushButton_cancel, &QPushButton::clicked, this, [=](){slot_pub_ros_console_cancel(iii, hhh);});
      // connect(btn, &QPushButton::clicked, this, [=](){func(i);});
      // connect(console_single_ui[iii][hhh]->ui->pushButton,&QPushButton::clicked,(this)[]{m_obj->m_slot(iii，jjj);});
      // connect(console_single_ui[iii][hhh]->ui->pushButton,&QPushButton::clicked,(this)[]{slot_pub_ros_console(iii, jjj);});
      // connect(btn,&QPushButton::clicked,(this)[]{m_obj->m_slot(true，false);});
      // btn是按钮，m_obj是对象，m_slot是m_obj的槽函数，可以带参，true和false都是传入m_slot的参数。
      ui->gridLayout->addWidget(console_single_ui[iii][hhh], hhh,iii);
      console_single_ui[iii][hhh]->hide();
    }
    current_num_console_single[iii] = 3;  //这里取决于下面的初始几项
  }

  for (int iii=1; iii<NNN; iii++)
  {
    int hhh=1;
    console_single_ui[iii][hhh]->ui->label_node->setText( QString("mavros") );
    console_single_ui[iii][hhh]->ui->label_param->setText( QString("THS0, 921600") );
    console_single_ui[iii][hhh]->ui->pushButton_cancel->setVisible(0);
    console_single_ui[iii][hhh]->ui->lineEdit->setVisible(1);
    console_single_ui[iii][hhh]->ui->lineEdit->setText( QString("未运行") );

    console_single_ui[iii][hhh]->rosmsg.sysid = 100;  //发送方ID
    console_single_ui[iii][hhh]->rosmsg.compid = iii; //接收方ID
    console_single_ui[iii][hhh]->rosmsg.command = mavros;
    console_single_ui[iii][hhh]->rosmsg.flag = 1;
    console_single_ui[iii][hhh]->rosmsg.type1 = 3;
    console_single_ui[iii][hhh]->rosmsg.type2 = 2;

    int num_j, num_i;
    num_i = mavros;
    num_j = (int) (num_i/8);
    num_i = num_i - 8*num_j;
    console_single_ui[iii][hhh]->ros_node_j = num_j;
    console_single_ui[iii][hhh]->ros_node_i = num_i;

    console_single_ui[iii][hhh]->show();
  }

  for (int iii=1; iii<NNN; iii++)
  {
    int hhh=2;
    console_single_ui[iii][hhh]->ui->label_node->setText( QString("uav_ctrl") );
    console_single_ui[iii][hhh]->ui->label_param->setText( QString("Nan") );
    console_single_ui[iii][hhh]->ui->pushButton_cancel->setVisible(0);
    console_single_ui[iii][hhh]->ui->lineEdit->setVisible(1);
    console_single_ui[iii][hhh]->ui->lineEdit->setText( QString("未运行") );

    console_single_ui[iii][hhh]->rosmsg.sysid = 100;  //发送方ID
    console_single_ui[iii][hhh]->rosmsg.compid = iii; //接收方ID
    console_single_ui[iii][hhh]->rosmsg.command = uav_ctrl;
    console_single_ui[iii][hhh]->rosmsg.flag = 1;
    console_single_ui[iii][hhh]->rosmsg.type1 = 1;
    console_single_ui[iii][hhh]->rosmsg.type2 = 1;

    int num_j, num_i;
    num_i = uav_ctrl;
    num_j = (int) (num_i/8);
    num_i = num_i - 8*num_j;
    console_single_ui[iii][hhh]->ros_node_j = num_j;
    console_single_ui[iii][hhh]->ros_node_i = num_i;

    console_single_ui[iii][hhh]->show();
  }

  for (int iii=1; iii<NNN; iii++)
  {
    int hhh=3;
    console_single_ui[iii][hhh]->ui->label_node->setText( QString("vision_pose") );
    console_single_ui[iii][hhh]->ui->label_param->setText( QString("vrpn, Nan") );
    console_single_ui[iii][hhh]->ui->pushButton_cancel->setVisible(0);
    console_single_ui[iii][hhh]->ui->lineEdit->setVisible(1);
    console_single_ui[iii][hhh]->ui->lineEdit->setText( QString("未运行") );

    console_single_ui[iii][hhh]->rosmsg.sysid = 100;  //发送方ID
    console_single_ui[iii][hhh]->rosmsg.compid = iii; //接收方ID
    console_single_ui[iii][hhh]->rosmsg.command = vision_pose;
    console_single_ui[iii][hhh]->rosmsg.flag = 1;
    console_single_ui[iii][hhh]->rosmsg.type1 = 1;
    console_single_ui[iii][hhh]->rosmsg.type2 = 1;

    int num_j, num_i;
    num_i = vision_pose;
    num_j = (int) (num_i/8);
    num_i = num_i - 8*num_j;
    console_single_ui[iii][hhh]->ros_node_j = num_j;
    console_single_ui[iii][hhh]->ros_node_i = num_i;

    console_single_ui[iii][hhh]->show();
  }

  ui->scrollAreaWidgetContents->setMinimumHeight(85*NNN);
  // ui->scrollAreaWidgetContents->setMinimum(); // TODO

}

// -------------------------------------------------------------------------
//                                                          ROS solt 链接信号
// -------------------------------------------------------------------------
void console_widget::slot_pub_ros_console_cancel(int iii, int jjj)
{
  int temp_n;
  temp_n = current_num_console_single[iii];

  for (int h=jjj; h<temp_n;h++)
  {
    console_single_ui[iii][h]->ros_node_i = console_single_ui[iii][h+1]->ros_node_i;
    console_single_ui[iii][h]->ros_node_j = console_single_ui[iii][h+1]->ros_node_j;

    console_single_ui[iii][h]->rosmsg = console_single_ui[iii][h+1]->rosmsg;
    console_single_ui[iii][h]->flag_button_clicked = console_single_ui[iii][h+1]->flag_button_clicked;
    console_single_ui[iii][h]->led_state = console_single_ui[iii][h+1]->led_state; // 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
    console_single_ui[iii][h]->ui->label_node->setText( console_single_ui[iii][h+1]->ui->label_node->text() );
    console_single_ui[iii][h]->ui->label_param->setText( console_single_ui[iii][h+1]->ui->label_param->text() );
    setLED(console_single_ui[iii][h]->ui->label_led, console_single_ui[iii][h]->led_state, 11);
    switch ( console_single_ui[iii][h]->led_state )
    {
    case 0:
      console_single_ui[iii][h]->ui->lineEdit->setText( QString("未运行") );
      break;

    case 1:
      console_single_ui[iii][h]->ui->lineEdit->setText( QString("启动失败") );
      break;

    case 2:
      console_single_ui[iii][h]->ui->lineEdit->setText( QString("运行中") );
      break;

    case 3:
      console_single_ui[iii][h]->ui->lineEdit->setText( QString("启动中") );
      break;
    }

    if (console_single_ui[iii][h]->flag_button_clicked==false)
    { // Set 状态
      console_single_ui[iii][h]->ui->pushButton_cancel->setVisible(1);
      console_single_ui[iii][h]->ui->pushButton->setText( QString("Set") );
      console_single_ui[iii][h]->ui->lineEdit->setVisible(0);
    }
    else
    { // kill 状态
      console_single_ui[iii][h]->ui->pushButton_cancel->setVisible(0);
      console_single_ui[iii][h]->ui->pushButton->setText( QString("Kill") );
      console_single_ui[iii][h]->ui->lineEdit->setVisible(1);
    }
  }

  current_num_console_single[iii] = temp_n-1 ;
  console_single_ui[iii][temp_n]->hide();
}

void console_widget::slot_pub_ros_console(int iii, int jjj)
{
    console_qnode.cb_pub_console( console_single_ui[iii][jjj]->rosmsg );

    if (console_single_ui[iii][jjj]->flag_button_clicked==true){
      // 重置于 reset
      if (jjj>2)
      { console_single_ui[iii][jjj]->ui->pushButton_cancel->setVisible(1);
        console_single_ui[iii][jjj]->ui->lineEdit->setVisible(0);
      }
      console_single_ui[iii][jjj]->ui->lineEdit->setText( QString("已取消") );
      console_single_ui[iii][jjj]->flag_button_clicked = false;
      console_single_ui[iii][jjj]->ui->pushButton->setText( QString("Set") );
      console_single_ui[iii][jjj]->rosmsg.flag = 1;
      console_single_ui[iii][jjj]->led_state = 0;  // 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      setLED(console_single_ui[iii][jjj]->ui->label_led,  console_single_ui[iii][jjj]->led_state, 11);
    }
    else {
      // 置于 kill
      if (jjj>2)
      { console_single_ui[iii][jjj]->ui->pushButton_cancel->setVisible(0);
        console_single_ui[iii][jjj]->ui->lineEdit->setVisible(1);
      }
      console_single_ui[iii][jjj]->flag_button_clicked = true;
      console_single_ui[iii][jjj]->ui->pushButton->setText( QString("Kill") );
      console_single_ui[iii][jjj]->rosmsg.flag = 3;
      console_single_ui[iii][jjj]->led_state = 3;  // 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      setLED(console_single_ui[iii][jjj]->ui->label_led,  console_single_ui[iii][jjj]->led_state, 11);
      console_single_ui[iii][jjj]->ui->lineEdit->setText( QString("启动中") );
    }
}

// qt gui 界面数据刷新
void console_widget::slot_power( )
{

}

// qt gui 无人机 已连接编号 刷新
void console_widget::slot_power_heart( )
{
  // 列表添加无人机编号选项
  ui->comboBox_no->clear();
  ui->comboBox_no_pos->clear();
  ui->comboBox_statemachine_no->clear();

  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    if (console_qnode.Flag_Data_heartbeat[temp_i] == true)
    {
      if (flag_drone_no)
      {
        flag_drone_no = false;
        ui->groupBox_console->setEnabled(1);
        ui->groupBox_console_pos->setEnabled(1);
        ui->groupBox_statemachine->setEnabled(1);
      }

      ui->comboBox_no->addItem( QString("%1").arg(temp_i) );
      ui->comboBox_no_pos->addItem( QString("%1").arg(temp_i) );
      ui->comboBox_statemachine_no->addItem( QString("%1").arg(temp_i) );
    }
  }
}

// qt gui 无人机 已连接编号 刷新
void console_widget::slot_power_rosnode( )
{ // 黄色灯 状态变化
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    if (console_qnode.flag_ros_node_change[temp_i] == true)
    { // 对应的飞机状态变化了
      // 查看当前所有的列
      for (int j = 1; j<=current_num_console_single[temp_i]; j++)
      {
        int ros_node_j, ros_node_i, led_state;

        ros_node_j = console_single_ui[temp_i][j]->ros_node_j;
        ros_node_i = console_single_ui[temp_i][j]->ros_node_i;
        led_state = console_single_ui[temp_i][j]->led_state;
        bool last_data, current_data;

        switch (ros_node_j)
        {
        case 0:
          current_data = console_qnode.now_param1[temp_i] & ( 1<<ros_node_i );
        break;

        case 1:
          current_data = console_qnode.now_param2[temp_i] & ( 1<<ros_node_i );
        break;

        case 2:
          current_data = console_qnode.now_param3[temp_i] & ( 1<<ros_node_i );
        break;

        case 3:
          current_data = console_qnode.now_param4[temp_i] & ( 1<<ros_node_i );
        break;

          default:

          break;
         }


        if ( current_data ) //当前状态置于绿灯
        {
          console_single_ui[temp_i][j]->led_state = 2;
          // 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
          setLED(console_single_ui[temp_i][j]->ui->label_led,  2, 11);
          console_single_ui[temp_i][j]->ui->lineEdit->setText( QString("运行中") );
        } else if ( led_state==3 )  // 判断是否是黄灯
        {
          console_single_ui[temp_i][j]->led_state = 1;
          // 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
          setLED(console_single_ui[temp_i][j]->ui->label_led,  1, 11);
          console_single_ui[temp_i][j]->ui->lineEdit->setText( QString("启动失败") );
        } else {  // 不是黄灯
          console_single_ui[temp_i][j]->led_state = 0;
          // 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
          setLED(console_single_ui[temp_i][j]->ui->label_led,  0, 11);
          console_single_ui[temp_i][j]->ui->lineEdit->setText( QString("未运行") );
        }
      }
    }
  }


}

void console_widget::on_pushButton_add_clicked()
{
  int iii, hhh;

  iii = ( ui->comboBox_no->currentText().toInt() );
  if (iii >= NNN || iii < 1)
  {
    std::cout << "Error in Console_widget: iii<1 or iii>NNN" << std::endl;
  } else
  {
    hhh = current_num_console_single[iii] + 1;
    if (hhh >= HHH)
    {
      std::cout << "Error in Console_widget: Reach Max HHH" << std::endl;
    }  else
    {
      current_num_console_single[iii] = hhh;

      console_single_ui[iii][hhh]->ui->label_node->setText( ui->comboBox_select->currentText() );

      console_single_ui[iii][hhh]->rosmsg.sysid = 100;  //发送方ID
      console_single_ui[iii][hhh]->rosmsg.compid = iii; //接收方ID
      console_single_ui[iii][hhh]->rosmsg.command = ui->comboBox_select->currentIndex();
      console_single_ui[iii][hhh]->rosmsg.flag = 1;
      console_single_ui[iii][hhh]->rosmsg.type1 = ui->comboBox_type1->currentIndex() + 1; //注意 qt Index 从0开始

      int num_j, num_i;
      num_i = ui->comboBox_select->currentIndex();
      num_j = (int) (num_i/8);
      num_i = num_i - 8*num_j;
      console_single_ui[iii][hhh]->ros_node_j = num_j;
      console_single_ui[iii][hhh]->ros_node_i = num_i;

      if  ( flag_console_type_same[ ui->comboBox_select->currentIndex()] == 1 )
      {
        console_single_ui[iii][hhh]->rosmsg.type2 = console_single_ui[iii][hhh]->rosmsg.type1;  // flag_console_type_same == 1
        console_single_ui[iii][hhh]->ui->label_param->setText( ui->comboBox_type1->currentText() );
      }
      else
      {
        console_single_ui[iii][hhh]->rosmsg.type2 = ui->comboBox_type2->currentIndex() + 1; //注意 qt Index 从0开始
        console_single_ui[iii][hhh]->ui->label_param->setText( ui->comboBox_type1->currentText() + ", " + ui->comboBox_type2->currentText() );
      }

      //置于 Set
      console_single_ui[iii][hhh]->ui->pushButton_cancel->setVisible(1);
      console_single_ui[iii][hhh]->ui->lineEdit->setVisible(0);
      console_single_ui[iii][hhh]->ui->lineEdit->setText( QString("未运行") );

      console_single_ui[iii][hhh]->flag_button_clicked = false;
      console_single_ui[iii][hhh]->ui->pushButton->setText( QString("Set") );
      console_single_ui[iii][hhh]->rosmsg.flag = 1;
      console_single_ui[iii][hhh]->led_state = 0;     // 0灰无 2绿Run 3黄kill color 0:grey 1:red 2:green 3:yellow
      setLED(console_single_ui[iii][hhh]->ui->label_led,  console_single_ui[iii][hhh]->led_state, 11);
      console_single_ui[iii][hhh]->show();
      // ui->gridLayout->addWidget(console_single_ui[iii][hhh], hhh,iii);
    }
  }
}

void console_widget::on_pushButton_pos_set_clicked()
{
  int iii;

  iii = ( ui->comboBox_no_pos->currentText().toInt() );
  if (iii >= NNN || iii < 1)
  {
    std::cout << "Error in Console_widget: iii<1 or iii>NNN" << std::endl;
  } else
  {
    console_qnode.msg_set_loc_pos_enu.sysid = 100;   // 发送方ID
    console_qnode.msg_set_loc_pos_enu.compid = iii;  // 接收方ID
    console_qnode.msg_set_loc_pos_enu.x = (float) (ui->doubleSpinBox_x->value());
    console_qnode.msg_set_loc_pos_enu.y = (float) (ui->doubleSpinBox_y->value());
    console_qnode.msg_set_loc_pos_enu.z = (float) (ui->doubleSpinBox_z->value());
    float temp_yaw;
    temp_yaw = ((float) (ui->spinBox_yaw->value())) /180.0 * pi;
    if ( temp_yaw >= pi )
    {temp_yaw = temp_yaw - 2*pi;  }
    if ( temp_yaw < -pi )
    {temp_yaw = temp_yaw + 2*pi;  }
    console_qnode.msg_set_loc_pos_enu.yaw = temp_yaw;

    console_qnode.msg_set_loc_pos_enu.flag = 1;      // 1-设置loc地址 2-设置编队偏差

    console_qnode.cb_pub_set_loc_pos_enu(console_qnode.msg_set_loc_pos_enu);
  }
}

void console_widget::on_pushButton_statemachine_set_clicked()
{
  int iii, hhh;

  iii = ( ui->comboBox_statemachine_no->currentText().toInt() );
  if (iii >= NNN || iii < 1)
  {
    std::cout << "Error in Console_widget: iii<1 or iii>NNN" << std::endl;
  } else
  {
    hhh = ui->comboBox_statemachine_nodestate->currentIndex();

    console_qnode.msg_changestate.sysid = 100;  //发送方ID
    console_qnode.msg_changestate.compid = iii;  //接收方ID
    console_qnode.msg_changestate.node_state = hhh + 1; //需要+1 qIndex 从0开始
    console_qnode.msg_changestate.param = 1;  //Nan
    console_qnode.cb_pub_changestate(console_qnode.msg_changestate);
  }
}

void console_widget::on_comboBox_select_currentTextChanged()
{
  int ii;
  ii = ui->comboBox_select->currentIndex();

  switch (ii) {
    case mavros: //mavros
      index_func_mavros();
    break;

  case uav_ctrl: //uav_ctrl
    index_func_uav_ctrl();
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

    case web_video_server: //web_video_server
      index_func_web_video_server();
    break;

    case vins:        //vins
      index_func_vins();
    break;

    case fast_planner: //fast_planner
      index_func_fast_planner();
    break;

    case yolov3:      //yolov3
      index_func_yolov3();
    break;
  }
}


void console_widget::index_func_mavros()
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

void console_widget::index_func_uav_ctrl()
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


void console_widget::index_func_vision_pose()
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

void console_widget::index_func_usb_cam()
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

void console_widget::index_func_csi_cam()
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

void console_widget::index_func_realsense_cam()
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

void console_widget::index_func_web_video_server()
{
  QStringList temp_items1, temp_items2;
  temp_items1 << "192.168.50.";
  temp_items1 << "192.168.1.";

  temp_items2 << "Nan";

  ui->comboBox_type1->clear();
  ui->comboBox_type1->addItems(temp_items1);
  ui->comboBox_type1->setCurrentIndex(0);

  ui->comboBox_type2->clear();
  ui->comboBox_type2->addItems(temp_items2);
  ui->comboBox_type2->setCurrentIndex(0);
}

void console_widget::index_func_vins()
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

void console_widget::index_func_fast_planner()
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

void console_widget::index_func_yolov3()
{
  QStringList temp_items1, temp_items2;
  temp_items1 << "/csi_cam_0/image_raw";			//1
  temp_items1 << "/usb_cam/image_raw";        //2
  temp_items1 << "/camera/color/image_raw";		//3

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
void console_widget::setLED(QLabel* label, int color, int size)
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
