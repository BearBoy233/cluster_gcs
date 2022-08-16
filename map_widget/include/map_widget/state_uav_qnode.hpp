
#ifndef state_uav_qnode_HPP_
#define state_uav_qnode_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <ros/network.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <std_msgs/String.h>
#include <sstream>
#include <geometry_msgs/Point.h>

#include <mavcomm_msgs/global_pos_int.h>
#include <mavcomm_msgs/Heartbeat.h>

#define NNN 11

namespace map_widget {

class state_uav_QNode : public QThread {
    Q_OBJECT

public:

  state_uav_QNode(int argc, char** argv );
  virtual ~state_uav_QNode();
	bool init();
	void run();

Q_SIGNALS:

    void rosShutdown();

    void power();        //触发信号  界面1hz信息更新
    // void power_pose();   //触发信号  无人机位置 4hz信息更新
    // void power_track_current(); //触发信号 无人机运动位置 轨迹显示

private:
	int init_argc;
	char** init_argv;

  // 订阅 心跳包
  ros::Subscriber         sub_heartbeat;
  mavcomm_msgs::Heartbeat msg_heartbeat;
  void cb_sub_heartbeat(const mavcomm_msgs::Heartbeat::ConstPtr& msg);

public:
  // 心跳包
  mavcomm_msgs::Heartbeat Data_msg_heartbeat[NNN];
  bool Flag_Data_heartbeat[NNN];

};

}  // namespace map_widget

#endif /* map_widget_QNODE_HPP_ */
