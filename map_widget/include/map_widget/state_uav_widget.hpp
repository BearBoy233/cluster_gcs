#ifndef state_uav_WIDGET_H
#define state_uav_WIDGET_H

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

#include "ui_state_uav_widget.h"
#include "state_single_widget.hpp"
#include "state_uav_qnode.hpp"

#define NNN 11

namespace map_widget {

class state_uav_widget : public QWidget
{
  Q_OBJECT

public:
  explicit state_uav_widget(int argc, char** argv, QWidget *parent = nullptr);
  ~state_uav_widget();

private:

  Ui::state_uav_widget *ui;             //对应 .ui 的 ObjectName
  state_uav_QNode state_uav_qnode;
  state_single_widget *state_single_ui[NNN]; //显示  状态 子QWidget

  // 获取程序运行路径 - 必须放在 cq_gcs 里
  QString htmlPath;
  QString temp_path_head; // "cq_gcs/src/map_widget/resources/images/";

  // init func
  void initConncetion();  // 初始化槽函数
  void init_showgrpah();  // 显示图标

  // help func
  void setLED(QLabel* label, int color, int size);
  // 鼠标放置特定label上会弹窗png提示，移走后消失
  bool eventFilter(QObject *watched, QEvent *event);
  QLabel * m_pLabel_info_connect;   // ### info_connect
  QLabel * m_pLabel_info_armed;     // ### info_armed
  QLabel * m_pLabel_info_manual;    // ### info_manual

public slots:

private slots:
    //data from state_uav_QNode; slot in void connections();
    void slot_power( );

};

} // namespace map_widget

#endif // state_uav_WIDGET_H
