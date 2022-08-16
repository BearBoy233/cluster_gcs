
#ifndef formation_qnode_HPP_
#define formation_qnode_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <ros/network.h>
#include <sstream>
#include <string>
#include <QThread>
#include <QDebug>
#include <QStringListModel>

#include <geometry_msgs/Point.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>

#include <mavcomm_msgs/local_pos_enu.h>
#include <geometry_msgs/PoseStamped.h>

#define NNN 11

namespace map_widget {

class formation_QNode : public QThread {
    Q_OBJECT

public:
  formation_QNode(int argc, char** argv );
  virtual ~formation_QNode();
	bool init();
	void run();

  // 用于被直接访问的 数据
  bool DataS_drone_enu_flag[NNN];
  bool DataS_drone_enu_update_flag[NNN];

  int flag_save_path_data[NNN]; // path 0不记录 1记录 2归0

  // 编队阵行设置
  ros::Publisher pub_set_local_pos_enu;
  mavcomm_msgs::local_pos_enu msg_pub_set_local_pos_enu;
  void cb_pub_set_local_pos_enu();

  ros::Subscriber sub_set_local_pos_enu;
  void cb_sub_set_local_pos_enu(const mavcomm_msgs::local_pos_enu::ConstPtr& msg);
  mavcomm_msgs::local_pos_enu received_set_local_pos_enu;

  mavcomm_msgs::local_pos_enu data_set_local_pos_enu[NNN]; // 收到的 无人机的编队设置反馈
  bool flag_data_set_local_pos_enu[NNN]; // flag 收到 无人机 编队设置反馈

Q_SIGNALS:

    void rosShutdown();

    void power();               //触发信号  界面1hz信息更新
    void power_pose();          //触发信号  无人机位置 4hz信息更新
    void power_set_data();      //触发信号  无人机编队阵型 已设置

private:
	int init_argc;
	char** init_argv;

  // 广播 无人机 path
  ros::Publisher pub_rviz_drone_pos[NNN];
  nav_msgs::Path path_rviz_drone_pos[NNN];
  nav_msgs::Path init_msg_path;
  // 广播 无人机 pos
  ros::Publisher pub_rviz_drone_q[NNN];
  geometry_msgs::PoseStamped pose_rviz_drone_pos[NNN];

  // 订阅 来自通信系统的 无人机的位置信息 local ENU
  ros::Subscriber sub_drone_pos;
  void cb_sub_drone_pos(const mavcomm_msgs::local_pos_enu::ConstPtr& msg);
  mavcomm_msgs::local_pos_enu received_pos_local;

public:
  float qnode_ot_offset_xy[NNN][4];    // 编队偏移量 x y z yaw/度 [注意转换! ui界面输入度/发送+计算弧度]

};

}  // namespace map_widget

#endif /* formation_QNODE_HPP_ */
