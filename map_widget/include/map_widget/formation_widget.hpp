#ifndef formation_WIDGET_H
#define formation_WIDGET_H

#include <QWidget>
//#include <QWebFrame>
#include <QDebug>
#include <QObject>

#include <QtGui>
#include <QMessageBox>
#include <iostream>

#include <QLineEdit>
#include <QDateTime>
#include <QUrl>
#include <QPaintEvent>
#include <QPainter>

#include <QtWidgets/QWidget>
#include <QtWidgets/QtWidgets>

#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>
#include <rviz/tool_manager.h>
#include<rviz/tool.h>
#include <rviz/default_plugin/view_controllers/orbit_view_controller.h>
#include <rviz/view_manager.h>

#include "ui_formation_widget.h"
#include "formation_qnode.hpp"

#define NNN 11

namespace map_widget {

// 枚举 编队阵型设置
class Cenum: public QObject
{
    Q_OBJECT
public:
    Cenum() {}
    enum Priority
    {
        unset=1,
        send,
        Set
    };
    Q_ENUM(Priority)
};

class formation_widget : public QWidget
{
  Q_OBJECT

public:
  explicit formation_widget(int argc, char** argv, QWidget *parent = nullptr);
  ~formation_widget();

private:
  // Data + Flag
  // 航迹显示 标志
  int curent_uav_num = 0;               // 当前已知的 无人机架数 UAV No
  bool flag_show_track_first_time[NNN]; // 默认True; 有loc_pos_msg后，对应UAV No置False,Widget更新

  // 无人机编队阵型设置 Data
  int ot_offset_uav_state[NNN];  // Cenum::Priority 1-unsend 2-send 3-Set
  QMetaEnum metaEnum;            // 用于 枚举<==>QString 转换
  bool ot_offset_leader[NNN];    // leader 标志位
  float ot_offset_xy[NNN][4];    // 编队偏移量 x y z yaw/度 [注意转换! ui界面输入度/发送+计算弧度]

  float print_offset_noxy[NNN][1+4];    // 用于编队阵型设置预览

  bool flag_set_all_formation_state = false; // lineEdit_set_formation_state

  void initConncetion();         // 初始化槽函数

public slots:

// signals :
Q_SIGNALS:
  void print_flag();

private slots:
  //data from formation_QNode; slot in void connections();
  void slot_power();
  void slot_power_pose();
  void slot_power_set_data();

  // on_XXX_clicked() widget ui 事件
  // RVIZ lauunch
  void on_pushButton_rviz_clicked();
  // 清除 无人机轨迹
  void on_showtrack_pushButton_all_clean_clicked();
  void on_showtrack_pushButton_select_clean_clicked();
  // 设置 编队队形
  void on_formation_pushButton_check_data_clicked();
  void on_formation_pushButton_set_single_clicked();
  void on_formation_pushButton_set_all_clicked();
  void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
  Ui::formation_widget *ui; //对应 .ui 的 ObjectName
  formation_QNode formation_qnode;

  // 编队队形预览
  void paintEvent(QPaintEvent *event);

  QFont font;    //颜色
  QColor color;  //字体
  QPen pen;      //画笔
  QPainter painter;

};

} // namespace map_widget

#endif // formation_WIDGET_H
