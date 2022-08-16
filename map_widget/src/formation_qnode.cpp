
#include "../include/map_widget/formation_qnode.hpp"

namespace map_widget {

formation_QNode::formation_QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

formation_QNode::~formation_QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
  // delete formation_QNode;
}

bool formation_QNode::init() {
  ros::init(init_argc,init_argv,"formation_widget");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;

  // Init
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  { // Qtopic[i]
    pub_rviz_drone_pos[temp_i] = n.advertise<nav_msgs::Path>("/show_rviz/path/drone"+std::to_string(temp_i), 1, true);
    pub_rviz_drone_q[temp_i] = n.advertise<geometry_msgs::PoseStamped>("/show_rviz/pose/drone"+std::to_string(temp_i), 1, true);
  }

  // Ros sub 来自通信系统的 无人机 ENU local位置信息
  // 显示无人机位置
  sub_drone_pos = n.subscribe<mavcomm_msgs::local_pos_enu>("/mavcomm/receive/loc_pos_enu", 10, &formation_QNode::cb_sub_drone_pos, this);
  // Ros pub 无人机 ENU 编队误差设计 (flag=2)
  pub_set_local_pos_enu = n.advertise<mavcomm_msgs::local_pos_enu>("/mavcomm/send/set_loc_pos_enu", 10);
  // Ros sub 无人机编队偏差反馈 (flag=3)   // 回传信息 告知地面站 无人机已设置 编队阵行 设置
  sub_set_local_pos_enu = n.subscribe<mavcomm_msgs::local_pos_enu>("/mavcomm/receive/set_loc_pos_enu", 10, &formation_QNode::cb_sub_set_local_pos_enu, this);

  // Init data
  init_msg_path.header.stamp = ros::Time::now();
  init_msg_path.header.frame_id = "world";

  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    DataS_drone_enu_flag[temp_i] = false;
    DataS_drone_enu_update_flag[temp_i] = false;
    path_rviz_drone_pos[temp_i] = init_msg_path;
    flag_save_path_data[temp_i] = 1;
    flag_data_set_local_pos_enu[temp_i] = false;
  }

	start();
	return true;
}

void formation_QNode::run() {
  ros::Rate loop_rate(20);
  int count = 1;
  while ( ros::ok() )
  {
    if (count == 20)
    {
      emit power();
      count = 0;
    }

    if ( count % 5 == 0)
    {
      emit power_pose();
    }

    count++;

    ros::spinOnce();
		loop_rate.sleep();
  }

	std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
  Q_EMIT rosShutdown();
  // used to signal the gui for a shutdown (useful to roslaunch)
}

// ---------------- 回调函数

void formation_QNode::cb_sub_set_local_pos_enu(const mavcomm_msgs::local_pos_enu::ConstPtr& msg)
{
  received_set_local_pos_enu = *msg;
  if (received_set_local_pos_enu.flag == 3)
  {
    int i = (int) received_set_local_pos_enu.sysid;

    data_set_local_pos_enu[i] = received_set_local_pos_enu;
    flag_data_set_local_pos_enu[i] = true;        
    emit power_set_data();
  }
}

// 订阅 无人机的位置信息 ENU
void formation_QNode::cb_sub_drone_pos(const mavcomm_msgs::local_pos_enu::ConstPtr& msg)
{
  received_pos_local = *msg;

  int temp_no;
  temp_no = (int) received_pos_local.sysid;

  DataS_drone_enu_flag[temp_no] = true;
  DataS_drone_enu_update_flag[temp_no] = true;

  geometry_msgs::PoseStamped this_pose_stamped;
  this_pose_stamped.header.stamp = ros::Time::now();
  this_pose_stamped.header.frame_id = "world";
  this_pose_stamped.pose.position.x = double(received_pos_local.x) + data_set_local_pos_enu[temp_no].x;
  this_pose_stamped.pose.position.y = double(received_pos_local.y) + data_set_local_pos_enu[temp_no].y;
  this_pose_stamped.pose.position.z = double(received_pos_local.z) + data_set_local_pos_enu[temp_no].z;

  // data_set_local_pos_enu[temp_no].yaw; TODO 待处理

  {
    double yaw = double(received_pos_local.yaw) + data_set_local_pos_enu[temp_no].yaw;

    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(0 * 0.5);
    double sp = sin(0 * 0.5);
    double cr = cos(0 * 0.5);
    double sr = sin(0 * 0.5);

    this_pose_stamped.pose.orientation.x = cy * cp * sr - sy * sp * cr;
    this_pose_stamped.pose.orientation.y = sy * cp * sr + cy * sp * cr;
    this_pose_stamped.pose.orientation.z = sy * cp * cr - cy * sp * sr;
    this_pose_stamped.pose.orientation.w = cy * cp * cr + sy * sp * sr;

    pose_rviz_drone_pos[temp_no] = this_pose_stamped;
    pub_rviz_drone_q[temp_no].publish( pose_rviz_drone_pos[temp_no] );
   }


  if (flag_save_path_data[temp_no]==2)
  { // path 归 0
    flag_save_path_data[temp_no] = 1;

    init_msg_path.header.stamp = this_pose_stamped.header.stamp;
    init_msg_path.header.frame_id = "world";
    path_rviz_drone_pos[temp_no] = init_msg_path;

    path_rviz_drone_pos[temp_no].poses.push_back(this_pose_stamped);
    pub_rviz_drone_pos[temp_no].publish(path_rviz_drone_pos[temp_no]);
  }
  else if (flag_save_path_data[temp_no]==1)
  {
    path_rviz_drone_pos[temp_no].poses.push_back(this_pose_stamped);
    pub_rviz_drone_pos[temp_no].publish(path_rviz_drone_pos[temp_no]);
  }

}


// pub
void formation_QNode::cb_pub_set_local_pos_enu()
{
  msg_pub_set_local_pos_enu.header.stamp = ros::Time::now();
  msg_pub_set_local_pos_enu.sysid = 100;

  pub_set_local_pos_enu.publish(msg_pub_set_local_pos_enu);
}

}  // namespace map_widget
