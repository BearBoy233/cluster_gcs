
#ifndef bd_map_qnode_HPP_
#define bd_map_qnode_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <ros/network.h>
#include <sstream>
#include <string>
#include <QThread>
#include <QStringListModel>

#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <mavcomm_msgs/global_pos_int.h>

#define NNN 11

namespace map_widget {

class bd_map_QNode : public QThread {
    Q_OBJECT

public:

  bd_map_QNode(int argc, char** argv );
  virtual ~bd_map_QNode();
	bool init();
	void run();

  // 广播 地图中设定的Home点的 GPS 坐标
  void cb_pb_home_gps(double temp_lon,double temp_lat);

  // widget 可直接访问的 Data
  bool DataS_drone_gps_flag[NNN];
  bool DataS_drone_gps_update_flag[NNN];
  double DataS_drone_gps_lon[NNN];
  double DataS_drone_gps_lat[NNN];
  double DataS_drone_gps_yaw[NNN];

Q_SIGNALS:
    void rosShutdown();
    void power();               //触发信号 界面信息      1Hz更新
    void power_pose();          //触发信号 无人机位置信息 4Hz更新
    void power_track_current(); //触发信号 无人机运动位置 轨迹显示

private:
	int init_argc;
	char** init_argv;

  // Ros Pub 地图中设定的Home点的 GPS 坐标 (TODO to be used)
  ros::Publisher pub_home_gps;
  geometry_msgs::Point ROSdata_pub_home_gps;

  // Ros Sub 无人机的GPS位置信息
  ros::Subscriber sub_drone_pos;
  void cb_sub_drone_pos(const mavcomm_msgs::global_pos_int::ConstPtr& msg);
  mavcomm_msgs::global_pos_int ROSdata_sub_drone_pos;

};

}  // namespace map_widget

#endif /* map_widget_QNODE_HPP_ */
