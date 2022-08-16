#ifndef rtsp_multi_WIDGET_H
#define rtsp_multi_WIDGET_H

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

#include "ui_rtsp_multi_widget.h"
#include "rtsp_single_widget.hpp"
// #include "rtsp_multi_qnode.hpp"

#define NNN 11
//  NNN-1 飞机数
#define MMM 4
//  MMM-1 视频数

namespace map_widget {

class rtsp_multi_widget : public QWidget
{
  Q_OBJECT

public:
  explicit rtsp_multi_widget(int argc, char** argv, QWidget *parent = nullptr);
  ~rtsp_multi_widget();

private:

  Ui::rtsp_multi_widget *ui;             //对应 .ui 的 ObjectName
  // rtsp_multi_QNode rtsp_multi_qnode;
  rtsp_single_widget *test_single[MMM]; //显示  状态 子QWidget

  // 获取程序运行路径 - 必须放在 cq_gcs 里
  QString htmlPath;
  QString temp_path_head; // "cq_gcs/src/map_widget/resources/images/";

  // init func
  void initConncetion();  // 初始化槽函数
  void init_showgrpah();  // 显示图标

public slots:


private slots:
    //data from rtsp_multi_QNode; slot in void connections();
    void slot_power( );

};

} // namespace map_widget

#endif // rtsp_multi_WIDGET_H
