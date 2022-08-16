#ifndef BD_MAP_WIDGET_H
#define BD_MAP_WIDGET_H

#include <QWidget>
#include <QDebug>

#include <QtGui>
#include <QMessageBox>
#include <iostream>

#include <QLineEdit>
#include <QDateTime>
#include <QUrl>

#include <QtWidgets/QWidget>
#include <QtWidgets/QtWidgets>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebView/QtWebView>

#include <QWebChannel>

#include "ui_bd_map_widget.h"
#include "bd_map_qnode.hpp"

#define NNN 11

namespace map_widget {

class bd_map_widget : public QWidget
{
  Q_OBJECT

public:
  explicit bd_map_widget(int argc, char** argv, QWidget *parent = nullptr);
  ~bd_map_widget();

  // Flag map set_home
  bool flag_Set_Home_GPS = 0;   // 0-未设置Home 1-已设置Home点
  bool flag_Set_marker1 = 0;    // 0-未设置Home 1-已设置Home点
  bool flag_Set_marker1_2 = 0;
  QString Str_Data_Gps_lat, Str_Data_Gps_lon;
  double Data_Gps_lat, Data_Gps_lon;

private:

  // Flag Data
  bool flag_show_track_first_time[NNN]; // 航迹显示 标志

  void initConncetion();    // 初始化槽函数

  QString getJsRetString(); // 获取js函数返回值

  // 重新绘制一遍
  void replot_all();

public slots:
  // map_js 当前选中点的 GPS 坐标
  void recieveJsMessage_Point_GPS(const QString& jsMsg);

private slots:

  //data from bd_map_QNode; slot in void connections();
  void slot_power( );
  void slot_power_pose( );
  void slot_power_track_current();

  // map_js
  // 设置 icon 颜色
  void on_icon_color_radiobutton_b_clicked();
  void on_icon_color_radiobutton_w_clicked();
  // 将当前点设置 home 点
  void on_HomeGPS_pushButton_Set_Home_clicked();
  // 移动 图像中心 到 Set_GPS点
  void on_HomeGPS_pushButton_Set_centerLonLat_clicked();
  // 地图测距
  void on_DisGPS_pushButton_open_clicked();
  void on_DisGPS_pushButton_close_clicked();
  // 获取点击坐标
  void on_HomeGPS_pushButton_Obtain_Point_clicked();
  void on_HomeGPS_pushButton_Obtain_Point_2_clicked();
  // 显示无人机轨迹相关按钮
  void on_showtrack_pushButton_all_on_clicked();
  void on_showtrack_pushButton_all_off_clicked();
  void on_showtrack_pushButton_all_clean_clicked();
  // 选择要清除轨迹的无人机
  void on_showtrack_pushButton_select_clean_clicked();

private:
  Ui::bd_map_widget *ui; //对应 .ui 的 ObjectName

  bd_map_QNode bd_map_qnode;

  // baidu map js html
  QWebEngineView *m_pWebView{ nullptr };
  QWebChannel* m_pWebChannel{ nullptr };

  // 无人机轨迹显示 带参数的信号槽
private:
  QCheckBox * checkBox_showtrack[NNN];
  QSignalMapper *signalMapper;
private slots:
  void doClicked_showtrack(const int btnname); //处理最终信号的槽

};

} // namespace map_widget

#endif // BD_MAP_WIDGET_H
