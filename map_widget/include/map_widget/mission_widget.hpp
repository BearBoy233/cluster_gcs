#ifndef mission_WIDGET_H
#define mission_WIDGET_H

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

#include "ui_mission_widget.h"
#include "mission_qnode.hpp"
#include "mission_single_widget.hpp"

#define NNN 11
#define HHH 11

namespace map_widget {

class mission_widget : public QWidget
{
  Q_OBJECT

public:
  explicit mission_widget(int argc, char** argv, QWidget *parent = nullptr);
  ~mission_widget();

private:

  void setLED(QLabel* label, int color, int size);

  void initConncetion();  //初始化槽函数

  // TODO 将数组转换为堆栈[溢出/删减方便？]
  mission_single_widget *mission_single_ui[NNN][HHH]; //显示  状态 子QWidget
  int current_num_mission_single[NNN];
  bool flag_drone_no = true;

  // TODO 转化为 结构体?
  enum Case_mission {
  mavros,
  vision_pose,
  usb_cam,
  csi_cam,
  realsense_cam,
  };
  // 0-不同 1-相同
  int flag_mission_type_same[5] = {
  0, // mavros
  0, // vision_pose
  1, // usb_cam
  1, // csi_cam
  1, // realsense_cam
  };

  void init_comboBox_select();
  void index_func_mavros();
  void index_func_vision_pose();
  void index_func_usb_cam();
  void index_func_csi_cam();
  void index_func_realsense_cam();

  void initgridLayout();

public slots:

private slots:

    void slot_pub_ros_mission_cancel(int iii, int jjj);
    void slot_pub_ros_mission(int iii, int jjj);

    //data from mission_QNode; slot in void connections();
    void slot_power( );
    void slot_power_heart( );

    void on_comboBox_select_currentTextChanged();

    void on_pushButton_add_clicked();

private:
  Ui::mission_widget *ui; //对应 .ui 的 ObjectName

  mission_QNode mission_qnode;

};

} // namespace map_widget

#endif // mission_WIDGET_H
