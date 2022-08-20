#ifndef console_WIDGET_H
#define console_WIDGET_H

#include <QWidget>
//#include <QWebFrame>
#include <QDebug>

#include <QtGui>
#include <QMessageBox>
#include <iostream>

#include <QLineEdit>
#include <QDateTime>
#include <QUrl>

#include <QtWidgets/QWidget>

// #include <QtWidgets/QtWidgets>
// #include <QtWebEngineWidgets/QWebEngineView>
// #include <QtWebView/QtWebView>

// #include <QWebChannel>

#include "ui_console_widget.h"
#include "console_qnode.hpp"
#include "console_single_widget.hpp"

#define NNN 11
#define HHH 11
#define pi 3.1415926

namespace map_widget {

class console_widget : public QWidget
{
  Q_OBJECT

public:
  explicit console_widget(int argc, char** argv, QWidget *parent = nullptr);
  ~console_widget();

private:

  void setLED(QLabel* label, int color, int size);

  void initConncetion();  //初始化槽函数

  // TODO 将数组转换为堆栈[溢出/删减方便？]
  console_single_widget *console_single_ui[NNN][HHH]; //显示  状态 子QWidget
  int current_num_console_single[NNN];
  bool flag_drone_no = true;

  // 无人机状态 changestate /uav_ctrl.cpp#enum FlightState
  // 需要同步修改 px4_ctrl.cpp & console_widget.hpp/cpp & state_single_widget.hpp
  enum FlightState {
    UNINIT,                   // 未知状态
    IDLE,                     // 什么都不干 地面待机状态
    TAKING_OFF,               // 当前 loc 点起飞
    LANDING,                  // 当前 loc 点降落
    POS_EXECUTION,            // 位置控制
    Vel_EXECUTION,            // 速度控制
    WAITING_FOR_HOME_POSE,    // 此和下面的状态 无法通过地面站切换到
    TAKEOFF_FAIL,
    LANDED,
  };

  // TODO 转化为 结构体?
  enum Case_console {
    mavros,
    uav_ctrl,
    vision_pose,
    usb_cam,
    csi_cam,
    realsense_cam,
    web_video_server,
    vins,
    fast_planner,
    yolov3,
  };
  // 0-不同 1-相同
  int flag_console_type_same[10] = {
    0, // mavros
    1, // uav_ctrl
    0, // vision_pose
    1, // usb_cam
    1, // csi_cam
    1, // realsense_cam
    1, // web_video_server
    1, // vins
    1, // fast_planner
    1, // yolov3
  };

  void init_comboBox_select();
  void index_func_mavros();
  void index_func_uav_ctrl();
  void index_func_vision_pose();
  void index_func_usb_cam();
  void index_func_csi_cam();
  void index_func_realsense_cam();
  void index_func_web_video_server();
  void index_func_vins();
  void index_func_fast_planner();
  void index_func_yolov3();

  void initgridLayout();

public slots:


private slots:

    void slot_pub_ros_console_cancel(int iii, int jjj);
    void slot_pub_ros_console(int iii, int jjj);

    //data from console_QNode; slot in void connections();
    void slot_power( );
    void slot_power_heart( );
    void slot_power_rosnode( );

    void on_comboBox_select_currentTextChanged();
    void on_pushButton_statemachine_set_clicked();

    void on_pushButton_pos_set_clicked();
    //ui里限制了xyz的取值[-200,200]/[0,100]

    void on_pushButton_add_clicked();

private:
  Ui::console_widget *ui; //对应 .ui 的 ObjectName

  console_QNode console_qnode;

};

} // namespace map_widget

#endif // console_WIDGET_H
