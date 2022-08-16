
#include "../include/map_widget/bd_map_qnode.hpp"

namespace map_widget {

bd_map_QNode::bd_map_QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

bd_map_QNode::~bd_map_QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
  // delete bd_map_QNode;
}

bool bd_map_QNode::init() {
  ros::init(init_argc,init_argv,"bd_map_widget");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;

  n.param<std::string>("rospackage_path", rospackage_path, "~/zy_new_mavros/cq_gcs/src/cluster_gcs");
  n.param<double>("map_init_lon", map_init_lon, 116);
  n.param<double>("map_init_lat", map_init_lat, 39);


  // Ros Pub 地图中设定的Home点的 GPS 坐标 (TODO to be used)
  pub_home_gps = n.advertise<geometry_msgs::Point>("/gcs/HomePoint_gps",10);

  // Ros Sub 无人机的GPS位置信息
  sub_drone_pos = n.subscribe<mavcomm_msgs::global_pos_int>("/mavcomm/receive/gps_pos", 10, &bd_map_QNode::cb_sub_drone_pos, this);

  // Init Data
  for (int temp_i = 1; temp_i < NNN; temp_i++)
  {
    DataS_drone_gps_flag[temp_i] = false;
    DataS_drone_gps_update_flag[temp_i] = false;
  }

	start();
	return true;
}

void bd_map_QNode::run() {
  // 数据更新 Hz 控制
  ros::Rate loop_rate(20);
  int count = 1;
  while ( ros::ok() )
  {
    if (count == 20)
    {
      emit power();
      count = 0;
    }
    if (count == 15)
    {
      emit power_track_current();
    }
    if ( count % 10 == 0)
    {
      emit power_pose();
    }

    count++;

    ros::spinOnce();
		loop_rate.sleep();
  }

  std::cout << "[bd_map_qnode] Ros shutdown, proceeding to close the gui." << std::endl;
	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

// Ros Pub 地图中设定的Home点的 GPS 坐标 (TODO to be used)
void bd_map_QNode::cb_pb_home_gps(double temp_lon,double temp_lat)
{
  ROSdata_pub_home_gps.x = temp_lon;
  ROSdata_pub_home_gps.y = temp_lat;

  pub_home_gps.publish(ROSdata_pub_home_gps);
}


// ---------------- 回调函数
// Ros Sub 无人机的GPS位置信息
void bd_map_QNode::cb_sub_drone_pos(const mavcomm_msgs::global_pos_int::ConstPtr& msg)
{
  ROSdata_sub_drone_pos = *msg;

  int i;
  i = ROSdata_sub_drone_pos.sysid;

  DataS_drone_gps_flag[i] = true;
  DataS_drone_gps_update_flag[i] = true;
  DataS_drone_gps_lon[i] = ROSdata_sub_drone_pos.lon / 1E7;
  DataS_drone_gps_lat[i] = ROSdata_sub_drone_pos.lat / 1E7;
  DataS_drone_gps_yaw[i] = ROSdata_sub_drone_pos.hdg_yaw;
}

}  // namespace map_widget
