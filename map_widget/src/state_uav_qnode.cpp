
#include "../include/map_widget/state_uav_qnode.hpp"

namespace map_widget {

state_uav_QNode::state_uav_QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

state_uav_QNode::~state_uav_QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
  wait();
  // delete state_uav_QNode;
}

bool state_uav_QNode::init() {
  ros::init(init_argc,init_argv,"uav_state_widget");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;

  // 订阅心跳包
  sub_heartbeat = n.subscribe<mavcomm_msgs::Heartbeat>("/mavcomm/receive/heartbeat", 10, &state_uav_QNode::cb_sub_heartbeat, this);

  // 初始化
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    Flag_Data_heartbeat[temp_i] = false;
  }

	start();
	return true;
}

void state_uav_QNode::run() {
  ros::Rate loop_rate(10);
  int count = 1;
  while ( ros::ok() )
  {
    if (count == 10)
    {
      emit power();
      count = 0;
    }

    count++;

    ros::spinOnce();
		loop_rate.sleep();
  }

  std::cout << "[state_uav_qnode] Ros shutdown, proceeding to close the gui." << std::endl;
	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}


// ---------------- 回调函数
// 订阅 来自通信系统的 无人机的位置信息
void state_uav_QNode::cb_sub_heartbeat(const mavcomm_msgs::Heartbeat::ConstPtr& msg)
{
  msg_heartbeat = *msg;

  int i;
  i = (int)msg_heartbeat.sysid;
  Flag_Data_heartbeat[i] = true;
  Data_msg_heartbeat[i] = msg_heartbeat;
}


}  // namespace map_widget
