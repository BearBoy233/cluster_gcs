
#include "../include/map_widget/state_uav_widget.hpp"

#define NNN 11

namespace map_widget {

using namespace Qt;

// -------------------------------------------------------------------------
//                                                state_uav_widget 初始化声明
// -------------------------------------------------------------------------
state_uav_widget::state_uav_widget(int argc, char** argv, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::state_uav_widget)
  , state_uav_qnode(argc,argv)
{
  ui->setupUi(this);

  // 获取程序所在路径，切换到同目录下的
  htmlPath = QCoreApplication::applicationDirPath();
  temp_path_head = "cq_gcs";
  int temp_path_1 = htmlPath.indexOf(temp_path_head);
  temp_path_head = htmlPath.left(temp_path_1) + "cq_gcs/src/map_widget/resources/images/";

  //--------------------------------------------------------------------------
  initConncetion();   //初始化槽函数
  init_showgrpah();   //初始化界面图标

  state_uav_qnode.init();

  //--------------------------------------------------------------------------
  // 鼠标放置特定label上会弹窗png提示，移走后消失

  // ### info_connect
  m_pLabel_info_connect = new QLabel(this);
  QPixmap pixmap_connect( temp_path_head + "/info_connect.png");
  m_pLabel_info_connect->setPixmap(pixmap_connect);
  m_pLabel_info_connect->hide();
  // 弹窗 安装事件过滤器
  ui->label_state_connected->installEventFilter(this);
  m_pLabel_info_connect->installEventFilter(this);

  // ### info_armed
  m_pLabel_info_armed = new QLabel(this);
  QPixmap pixmap_armed( temp_path_head + "/info_armed.png");
  m_pLabel_info_armed->setPixmap(pixmap_armed);
  m_pLabel_info_armed->hide();
  // 弹窗 安装事件过滤器
  ui->label_state_armed->installEventFilter(this);
  m_pLabel_info_armed->installEventFilter(this);

  // ### info_manual
  m_pLabel_info_manual = new QLabel(this);
  QPixmap pixmap_manual( temp_path_head + "/info_manual.png");
  m_pLabel_info_manual->setPixmap(pixmap_manual);
  m_pLabel_info_manual->hide();
  // 弹窗 安装事件过滤器
  ui->label_state_manual_input->installEventFilter(this);
  m_pLabel_info_manual->installEventFilter(this);

}

state_uav_widget::~state_uav_widget()
{
  delete ui;
}

// 初始化信号槽函数
void state_uav_widget::initConncetion()
{
   // --- ROS ---
   // state_uav_QNode 获取界面信息更新
    connect( &state_uav_qnode, SIGNAL( power( ) ), this, SLOT( slot_power( ) ) );
}

void state_uav_widget::init_showgrpah()
{
  //-------------------------------------------------------------------------------
  //                                                                       设置 图标
  // icon_uav
  QGraphicsScene *scene_icon_uav = new QGraphicsScene;                              //创建显示容器
  QString path_icon_uav = temp_path_head + "icon_quad32.png";                       //向容器中添加文件路径为fileName（QString类型）的文件
  scene_icon_uav->addPixmap(QPixmap(path_icon_uav));                                // scene->addPixmap(QPixmap("/home/XXX/cq_gcs/src/Battery24.png"));
  ui->graphicsView_icon_uav->setStyleSheet("background: transparent;border:0px");   //设置透明
  ui->graphicsView_icon_uav->setScene(scene_icon_uav);                              //借助graphicsView（QGraphicsView类）控件显示容器的内容
  ui->graphicsView_icon_uav->show();                                                //开始显示

  // graphicsView_state
  QGraphicsScene *scene_connect = new QGraphicsScene;                           //创建显示容器
  QString path_connect = temp_path_head + "connect24.png";                      //向容器中添加文件路径为fileName（QString类型）的文件
  scene_connect->addPixmap(QPixmap(path_connect));                              // scene->addPixmap(QPixmap("/home/XXX/cq_gcs/src/Battery24.png"));
  ui->graphicsView_state->setStyleSheet("background: transparent;border:0px");  //设置透明
  ui->graphicsView_state->setScene(scene_connect);                              //借助graphicsView（QGraphicsView类）控件显示容器的内容
  ui->graphicsView_state->show();                                               //开始显示

  // battery
  QGraphicsScene *scene_battery = new QGraphicsScene;                             //创建显示容器
  QString path_battery = temp_path_head + "Battery24.png";                        //向容器中添加文件路径为fileName（QString类型）的文件
  scene_battery->addPixmap(QPixmap(path_battery));                                // scene->addPixmap(QPixmap("/home/XXX/cq_gcs/src/Battery24.png"));
  ui->graphicsView_battery->setStyleSheet("background: transparent;border:0px");  //设置透明
  ui->graphicsView_battery->setScene(scene_battery);                              //借助graphicsView（QGraphicsView类）控件显示容器的内容
  ui->graphicsView_battery->show();                                               //开始显示

  // gps
  QGraphicsScene *scene_gps = new QGraphicsScene;                             //创建显示容器
  QString path_gps = temp_path_head + "gps24.png";                            //向容器中添加文件路径为fileName（QString类型）的文件
  scene_gps->addPixmap(QPixmap(path_gps));                                    // scene->addPixmap(QPixmap("/home/XXX/cq_gcs/src/gps24.png"));
  ui->graphicsView_gps->setStyleSheet("background: transparent;border:0px");  //设置透明
  ui->graphicsView_gps->setScene(scene_gps);                                  //借助graphicsView（QGraphicsView类）控件显示容器的内容
  ui->graphicsView_gps->show();                                               //开始显示

  // px4
  QGraphicsScene *scene_px4 = new QGraphicsScene;                             //创建显示容器
  QString path_px4 = temp_path_head + "px4_f30.png";                          //向容器中添加文件路径为fileName（QString类型）的文件
  scene_px4->addPixmap(QPixmap(path_px4));                                    // scene->addPixmap(QPixmap("/home/XXX/cq_gcs/src/px424.png"));
  ui->graphicsView_px4->setStyleSheet("background: transparent;border:0px");  //设置透明
  ui->graphicsView_px4->setScene(scene_px4);                                  //借助graphicsView（QGraphicsView类）控件显示容器的内容
  ui->graphicsView_px4->show();                                               //开始显示

  // computer
  QGraphicsScene *scene_computer = new QGraphicsScene;                             //创建显示容器
  QString path_computer = temp_path_head + "computer24.png";                       //向容器中添加文件路径为fileName（QString类型）的文件
  scene_computer->addPixmap(QPixmap(path_computer));                               // scene->addPixmap(QPixmap("/home/XXX/cq_gcs/src/computer24.png"));
  ui->graphicsView_computer->setStyleSheet("background: transparent;border:0px");  //设置透明
  ui->graphicsView_computer->setScene(scene_computer);                             //借助graphicsView（QGraphicsView类）控件显示容器的内容
  ui->graphicsView_computer->show();                                               //开始显示

  //-------------------------------------------------------------------------------
  //                                                                   设置 子Widget
  ui->verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
  ui->scrollAreaWidgetContents->setMinimumHeight(40*NNN+40);

  for (int iii=1; iii<NNN; iii++)
  {
    state_single_ui[iii] = new state_single_widget(ui->scrollAreaWidgetContents );
    ui->verticalLayout->addWidget(state_single_ui[iii]);
    state_single_ui[iii]->ui->label_no->setText( QString("%1").arg(iii) );
    state_single_ui[iii]->setVisible(0);
  }

}

// -------------------------------------------------------------------------
//                                                          ROS solt 链接信号
// -------------------------------------------------------------------------
// qt gui 界面数据刷新
void state_uav_widget::slot_power( )
{
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    if (state_uav_qnode.Flag_Data_heartbeat[temp_i] == true)
    {
      state_single_ui[temp_i]->setVisible(1);

      mavcomm_msgs::Heartbeat msg_hb;
      std::string temp_str;

      msg_hb = state_uav_qnode.Data_msg_heartbeat[temp_i];

      // px4_mode
      temp_str = state_single_ui[temp_i]->px4_mode_str[msg_hb.px4_mode];
      state_single_ui[temp_i]->ui->label_px4_mode->setText( QString::fromStdString(temp_str) );
      // px4 sys_state
      temp_str = state_single_ui[temp_i]->px4_sys_state_str[msg_hb.px4_sys_state];
      state_single_ui[temp_i]->ui->label_px4_sys_state->setText( QString::fromStdString(temp_str) );
      // ctrl_state  => label_machine_state
      temp_str = state_single_ui[temp_i]->ctrl_state_type_str[msg_hb.ctrl_state];
      state_single_ui[temp_i]->ui->label_machine_state->setText( QString::fromStdString(temp_str) );

      // gps
      temp_str = state_single_ui[temp_i]->gps_fix_type_str[msg_hb.gps_fix_type];
      state_single_ui[temp_i]->ui->label_gps_fix_type->setText( QString::fromStdString(temp_str) );
      state_single_ui[temp_i]->ui->label_gps_satellites_visible->setText( QString::number(msg_hb.gps_satellites_visible) );

      // msg_heartbeat.px4_state
      // 7
      // if (msg_px4_state.connected)  msg_heartbeat.px4_state |= (1<<7);	//bit 7 connected
      // setLED(ui->label_connect, 1, 11);         // 1红未连接 2绿已连接 color 0:grey 1:red 2:green 3:yellow
      if ( msg_hb.px4_state & (1<<7)  )
        { setLED( state_single_ui[temp_i]->ui->label_connect, 2, 11); }
      else { setLED( state_single_ui[temp_i]->ui->label_connect, 1, 11); }

      // 6
      // if (msg_px4_state.armed)  msg_heartbeat.px4_state |= (1<<6);	//bit 6 connected
      // setLED(ui->label_armed, 0, 11);           // 3黄未解锁 2绿已解锁 color 0:grey 1:red 2:green 3:yellow
      if ( msg_hb.px4_state & (1<<6) )
        { setLED( state_single_ui[temp_i]->ui->label_armed, 2, 11); }
      else { setLED( state_single_ui[temp_i]->ui->label_armed, 3, 11); }

      // 4
      // (msg_px4_state.manual_input)  msg_heartbeat.px4_state |= (1<<4);	//bit 4 connected
      // setLED(ui->label_manual_input, 0, 11);    // 0灰无遥控 2绿有遥控 color 0:grey 1:red 2:green 3:yellow
      if ( msg_hb.px4_state & (1<<4)  )
        { setLED( state_single_ui[temp_i]->ui->label_manual_input, 2, 11); }
      else { setLED( state_single_ui[temp_i]->ui->label_manual_input, 0, 11); }

      // 3
      // bit 3 battery_on 	|in above
      if ( msg_hb.px4_state & (1<<3) )
      { // 有 battery
        state_single_ui[temp_i]->ui->progressBar_battery->setValue( (int)(msg_hb.battery_percentage*100) );
        if (msg_hb.battery_cell_voltage > 4.4)
        {
          state_single_ui[temp_i]->ui->lineEdit_battery->setText( QString::number(msg_hb.battery_cell_voltage/4.0 ,'f',2) + "V" );
        }
        else
        {
          state_single_ui[temp_i]->ui->lineEdit_battery->setText( QString::number(msg_hb.battery_cell_voltage ,'f',2) + "V" );
        }
      } else
      {
        state_single_ui[temp_i]->ui->progressBar_battery->setValue( 0 );
        state_single_ui[temp_i]->ui->lineEdit_battery->setText( "0.00V" );
      }

      // TODO 连接断了的判断

    }
  }
}


// -------------------------------------------------------------------------
//                                                             Help Function
// -------------------------------------------------------------------------
// 该函数将label控件变成一个圆形指示灯，需要指定颜色color以及直径size
// color 0:grey 1:red 2:green 3:yellow
// size  单位是像素
void state_uav_widget::setLED(QLabel* label, int color, int size)
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

// -------------------------------------------------------------------------
//                                                              弹窗事件过滤器
// -------------------------------------------------------------------------
bool state_uav_widget::eventFilter(QObject *watched, QEvent *event)
{
  // info_connect
  if(ui->label_state_connected == watched || m_pLabel_info_connect == watched) {
      if(QEvent::Enter == event->type()) {            //鼠标进入
          if (m_pLabel_info_connect->isHidden()) {    //已经隐藏就显示出来
              m_pLabel_info_connect->show();
              QPoint point = ui->label_state_connected->pos();
              point.rx() = point.x() ; // -  m_pLabel_info_connect->width() + 20;
              point.ry() = point.y() + ui->label_state_connected->height() + 10;
              m_pLabel_info_connect->move(point);
              m_pLabel_info_connect->raise();         //显示最顶层
              // 其他hide
              m_pLabel_info_manual->hide();
              m_pLabel_info_armed->hide();
              return true;
          }
      }
      else if (QEvent::Leave == event->type()) {      //鼠标离开
        if (!m_pLabel_info_connect->isHidden()) {
          if(!ui->label_state_connected->geometry().contains(this->mapFromGlobal(QCursor::pos())) //判断鼠标是否在控件上
             &&!m_pLabel_info_connect->geometry().contains(this->mapFromGlobal(QCursor::pos())) )
          {
            m_pLabel_info_connect->hide();
            return true;
          }
        }
      }
  }

  // info_armed
  if(ui->label_state_armed == watched || m_pLabel_info_armed == watched) {
      if(QEvent::Enter == event->type()) {            //鼠标进入
          if (m_pLabel_info_armed->isHidden()) {    //已经隐藏就显示出来
              m_pLabel_info_armed->show();
              QPoint point = ui->label_state_armed->pos();
              point.rx() = point.x() ; // -  m_pLabel_info_armed->width() + 20;
              point.ry() = point.y() + ui->label_state_armed->height() + 10;
              m_pLabel_info_armed->move(point);
              m_pLabel_info_armed->raise();         //显示最顶层
              // 其他　hide
              m_pLabel_info_manual->hide();
              m_pLabel_info_connect->hide();
              return true;
          }
      }
      else if (QEvent::Leave == event->type()) {      //鼠标离开
        if (!m_pLabel_info_armed->isHidden()) {
          if(!ui->label_state_armed->geometry().contains(this->mapFromGlobal(QCursor::pos())) //判断鼠标是否在控件上
             &&!m_pLabel_info_armed->geometry().contains(this->mapFromGlobal(QCursor::pos())) )
          {
            m_pLabel_info_armed->hide();
            return true;
          }
        }
      }
  }

  // info_manual
  if(ui->label_state_manual_input == watched || m_pLabel_info_manual == watched) {
      if(QEvent::Enter == event->type()) {            //鼠标进入
          if (m_pLabel_info_manual->isHidden()) {    //已经隐藏就显示出来
              m_pLabel_info_manual->show();
              QPoint point = ui->label_state_manual_input->pos();
              point.rx() = point.x() ; // -  m_pLabel_info_manual->width() + 20;
              point.ry() = point.y() + ui->label_state_manual_input->height() + 10;
              m_pLabel_info_manual->move(point);
              m_pLabel_info_manual->raise();         //显示最顶层
              // 其他　hide
              m_pLabel_info_armed->hide();
              m_pLabel_info_connect->hide();
              return true;
          }
      }
      else if (QEvent::Leave == event->type()) {      //鼠标离开
        if (!m_pLabel_info_manual->isHidden()) {
          if(!ui->label_state_manual_input->geometry().contains(this->mapFromGlobal(QCursor::pos())) //判断鼠标是否在控件上
             &&!m_pLabel_info_manual->geometry().contains(this->mapFromGlobal(QCursor::pos())) )
          {
            m_pLabel_info_manual->hide();
            return true;
          }
        }
      }
  }

   return QWidget::eventFilter(watched, event);
}

} // namespace
