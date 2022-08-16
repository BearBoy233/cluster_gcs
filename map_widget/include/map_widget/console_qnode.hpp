
#ifndef console_qnode_HPP_
#define console_qnode_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>

#include <ros/network.h>
#include <ros/time.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>

#include <geometry_msgs/Point.h>

#include <mavcomm_msgs/Heartbeat.h>
#include <mavcomm_msgs/Console.h>
#include <mavcomm_msgs/Console_monitor.h>
#include <mavcomm_msgs/ChangeState.h>
#include <mavcomm_msgs/local_pos_enu.h>

#define NNN 11

namespace map_widget {

class console_QNode : public QThread {
    Q_OBJECT
public:
  console_QNode(int argc, char** argv );
  virtual ~console_QNode();
	bool init();
	void run();

  // 用于被直接访问的 数据
  bool DataS_drone_gps_flag[NNN];

Q_SIGNALS:
    void rosShutdown();
    void power();              //触发信号  界面1hz信息更新
    void power_heart();        //触发信号  界面1hz信息更新
    void power_rosnode();      //触发信号  界面1hz信息更新

private:
	int init_argc;
	char** init_argv;

  // 订阅 心跳包
  ros::Subscriber         sub_heartbeat;
  mavcomm_msgs::Heartbeat msg_heartbeat;
  void cb_sub_heartbeat(const mavcomm_msgs::Heartbeat::ConstPtr& msg);
  // 订阅 Console_monitor
  ros::Subscriber               sub_console_monitor;
  mavcomm_msgs::Console_monitor msg_console_monitor;
  void cb_sub_console_monitor(const mavcomm_msgs::Console_monitor::ConstPtr& msg);

public:
  // 发布 Console
  ros::Publisher          pub_console;
  void cb_pub_console(mavcomm_msgs::Console msg);
  // 发布 Changestate
  ros::Publisher          pub_changestate;
  void cb_pub_changestate(mavcomm_msgs::ChangeState msg);
  mavcomm_msgs::ChangeState msg_changestate;
  // 发布 set_loc_pos_enu
  ros::Publisher          pub_set_loc_pos_enu;
  void cb_pub_set_loc_pos_enu(mavcomm_msgs::local_pos_enu msg);
  mavcomm_msgs::local_pos_enu msg_set_loc_pos_enu;

public:
  bool Flag_Data_heartbeat[NNN];
  bool flag_power_heart;

  bool flag_ros_node_change[NNN];
  bool flag_power_rosnode;
  uint8_t last_param1[NNN];
  uint8_t last_param2[NNN];
  uint8_t last_param3[NNN];
  uint8_t last_param4[NNN];

  uint8_t now_param1[NNN];
  uint8_t now_param2[NNN];
  uint8_t now_param3[NNN];
  uint8_t now_param4[NNN];

};

}  // namespace map_widget

#endif
