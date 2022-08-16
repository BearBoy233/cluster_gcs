
#ifndef mission_qnode_HPP_
#define mission_qnode_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <ros/network.h>
#include <ros/time.h>
#include <string>
#include <sstream>
#include <QThread>
#include <QStringListModel>

#include <geometry_msgs/Point.h>
#include <std_msgs/String.h>

#include <mavcomm_msgs/Heartbeat.h>
#include <mavcomm_msgs/Console.h>

#define NNN 11

namespace map_widget {

class mission_QNode : public QThread {
    Q_OBJECT
public:
  mission_QNode(int argc, char** argv );
  virtual ~mission_QNode();
  bool init();
  void run();

  // 用于被直接访问的 数据
  bool DataS_drone_gps_flag[NNN];

Q_SIGNALS:
    void rosShutdown();
    void power();        //触发信号  界面1hz信息更新
    void power_heart();        //触发信号  界面1hz信息更新

private:
  int init_argc;
  char** init_argv;

  // 订阅 心跳包
  ros::Subscriber         sub_heartbeat;
  mavcomm_msgs::Heartbeat msg_heartbeat;
  void cb_sub_heartbeat(const mavcomm_msgs::Heartbeat::ConstPtr& msg);

public:
  // 发布 Console
  ros::Publisher          pub_mission;
  void cb_pub_mission(const mavcomm_msgs::Console msg);
  // 发布 loc_pos
  // ros::Publisher          pub_mission;
  // void cb_pub_mission(const mavcomm_msgs::Console msg);


public:
  bool Flag_Data_heartbeat[NNN];
  bool flag_power_heart;

};

}  // namespace map_widget

#endif /* map_widget_QNODE_HPP_ */
