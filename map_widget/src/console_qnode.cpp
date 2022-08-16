
#include "../include/map_widget/console_qnode.hpp"

namespace map_widget {

console_QNode::console_QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

console_QNode::~console_QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
  // delete console_QNode;
}

bool console_QNode::init() {
  ros::init(init_argc,init_argv,"console_widget");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;

  // Ros sub 心跳包
  sub_heartbeat = n.subscribe<mavcomm_msgs::Heartbeat>("/mavcomm/receive/heartbeat", 10, &console_QNode::cb_sub_heartbeat, this);
  // 发布 Console
  pub_console = n.advertise<mavcomm_msgs::Console>("/mavcomm/send/console", 10);
  // 订阅 Console_monitor
  sub_console_monitor = n.subscribe<mavcomm_msgs::Console_monitor>("/mavcomm/receive/console_monitor", 10, &console_QNode::cb_sub_console_monitor, this);



  // 发布 changeState
  pub_changestate = n.advertise<mavcomm_msgs::ChangeState>("/mavcomm/send/changestate", 10);

  // 发布 set_loc_pos_enu
  pub_set_loc_pos_enu = n.advertise<mavcomm_msgs::local_pos_enu>("/mavcomm/send/set_loc_pos_enu", 10);


  // 初始化
  flag_power_heart = 0;
  flag_power_rosnode = 0;
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    Flag_Data_heartbeat[temp_i] = false;
    last_param1[temp_i] = 0;
    last_param2[temp_i] = 0;
    last_param3[temp_i] = 0;
    last_param4[temp_i] = 0;
    now_param1[temp_i] = 0;
    now_param2[temp_i] = 0;
    now_param3[temp_i] = 0;
    now_param4[temp_i] = 0;
    flag_ros_node_change[temp_i] = false;
  }

	start();
	return true;
}


void console_QNode::run() {
  ros::Rate loop_rate(20);
  int count = 1;
  while ( ros::ok() )
  {
    if (flag_power_heart)
    {
      flag_power_heart = 0;
      emit power_heart();
    }

    if (flag_power_rosnode)
    {
      flag_power_rosnode = 0;
      emit power_rosnode();
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

  std::cout << "[console_qnode] Ros shutdown, proceeding to close the gui." << std::endl;
	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

// ---------------- 回调函数
// 订阅 心跳包
void console_QNode::cb_sub_heartbeat(const mavcomm_msgs::Heartbeat::ConstPtr& msg)
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

// 订阅 console_monitor
void console_QNode::cb_sub_console_monitor(const mavcomm_msgs::Console_monitor::ConstPtr& msg)
{
  int from_no;

  msg_console_monitor = *msg;
  from_no = (int) msg_console_monitor.sysid;
  if ( now_param1[from_no] != msg_console_monitor.param1 ||
       now_param2[from_no] != msg_console_monitor.param2 ||
       now_param3[from_no] != msg_console_monitor.param3 ||
       now_param4[from_no] != msg_console_monitor.param4 )
  {
    flag_ros_node_change[from_no] = 1;
    flag_power_rosnode = 1;

    last_param1[from_no] = now_param1[from_no];
    last_param2[from_no] = now_param2[from_no];
    last_param3[from_no] = now_param3[from_no];
    last_param4[from_no] = now_param4[from_no];

    now_param1[from_no] = msg_console_monitor.param1;
    now_param2[from_no] = msg_console_monitor.param2;
    now_param3[from_no] = msg_console_monitor.param3;
    now_param4[from_no] = msg_console_monitor.param4;
  }
}


void console_QNode::cb_pub_console(mavcomm_msgs::Console msg)
{
  msg.header.stamp=ros::Time::now();
  pub_console.publish(msg);
}

void console_QNode::cb_pub_changestate(mavcomm_msgs::ChangeState msg)
{
  msg.header.stamp=ros::Time::now();
  pub_changestate.publish(msg);
}

void console_QNode::cb_pub_set_loc_pos_enu(mavcomm_msgs::local_pos_enu msg)
{
  msg.header.stamp=ros::Time::now();
  pub_set_loc_pos_enu.publish(msg);
}

}  // namespace map_widget
