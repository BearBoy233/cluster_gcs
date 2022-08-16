
#include "../include/map_widget/mission_qnode.hpp"

namespace map_widget {

mission_QNode::mission_QNode(int argc, char** argv ) :
  init_argc(argc),
  init_argv(argv)
  {}

mission_QNode::~mission_QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
  wait();
  // delete mission_QNode;
}

bool mission_QNode::init() {
  ros::init(init_argc,init_argv,"mission_widget");
  if ( ! ros::master::check() ) {
    return false;
  }
  ros::start(); // explicitly needed since our nodehandle is going out of scope.
  ros::NodeHandle n;

  // 广播 地图中设定的Home点的 GPS 坐标
  // pub_home_gps = n.advertise<geometry_msgs::Point>("/gcs/HomePoint_gps",10);

  // 订阅 心跳包
  sub_heartbeat = n.subscribe<mavcomm_msgs::Heartbeat>("/mavcomm/receive/heartbeat", 10, &mission_QNode::cb_sub_heartbeat, this);
  // 发布 Console
  pub_mission = n.advertise<mavcomm_msgs::Console>("/mavcomm/send/console", 10);


  // 初始化
  flag_power_heart = 0;
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    Flag_Data_heartbeat[temp_i] = false;
  }


  start();
  return true;
}


void mission_QNode::run() {
  ros::Rate loop_rate(20);
  int count = 1;
  while ( ros::ok() )
  {
    if (flag_power_heart)
    {
      flag_power_heart = 0;
      emit power_heart();
    }

    if (count == 20)
    {
      // emit power();
      count = 0;
    }
    count++;

    ros::spinOnce();
    loop_rate.sleep();
  }

  std::cout << "[mission_qnode] Ros shutdown, proceeding to close the gui." << std::endl;
  Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

// ---------------- 回调函数
// 订阅 心跳包
void mission_QNode::cb_sub_heartbeat(const mavcomm_msgs::Heartbeat::ConstPtr& msg)
{
  msg_heartbeat = *msg;

  int i;
  i = (int)msg_heartbeat.sysid;
  if ( Flag_Data_heartbeat[i] == false)
  {
    Flag_Data_heartbeat[i] = true;
    flag_power_heart = 1;
  }
  // Data_msg_heartbeat[i] = msg_heartbeat;
}

void mission_QNode::cb_pub_mission(const mavcomm_msgs::Console msg)
{
  // msg.header.stamp=ros::Time::now();
  pub_mission.publish(msg);
}

}  // namespace map_widget
