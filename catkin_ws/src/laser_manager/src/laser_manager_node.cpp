#include <iostream>
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "nav_msgs/GetMap.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "sensor_msgs/LaserScan.h"
#include "tf/transform_listener.h"
#include "occupancy_grid_utils/ray_tracer.h"
#include "rosbag/bag.h"
#include "rosbag/view.h"
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

bool simulated = false;
bool is_rear = false;
sensor_msgs::LaserScan realLaserScan;
ros::NodeHandle* nh;
ros::Subscriber subRealLaserScan;
ros::ServiceClient srvCltGetMap;
bool dynamicMap = false;
nav_msgs::OccupancyGrid map;
bool first_scan = true;


void callback_laser_scan(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    realLaserScan = *msg;
    if(first_scan)
        first_scan = false;
}



int main(int argc, char** argv)
{
    std::cout << "INITIALIZING LASER MANAGER BY MARCOSOFT..." << std::endl;
    ros::init(argc, argv, "laser_manager");
    ros::NodeHandle n;
    nh = &n;
    
    std::string file_name = "";
    
    ros::Rate loop(60);

    
    std::cout << "LaserManager.->USING LASER IN REAL MODE !!!" << std::endl;
    subRealLaserScan = nh->subscribe("/hardware/real_scan", 1, callback_laser_scan);
    ros::Publisher pubScan = n.advertise<sensor_msgs::LaserScan>("scan", 1);

    
    sensor_msgs::LaserScan scanInfo;
    scanInfo.header.frame_id = "laser_link";
    scanInfo.angle_min = -2;
    scanInfo.angle_max = 2;
    scanInfo.angle_increment = 0.007;
    scanInfo.scan_time = 0.1;
    scanInfo.range_min = 0.01;
    scanInfo.range_max = 4.0;
    
    sensor_msgs::LaserScan simulatedScan;
    sensor_msgs::LaserScan::Ptr msgFromBag;

    
    
    tf::TransformListener listener;
    geometry_msgs::Pose sensorPose;
    sensorPose.position.x = 0;
    sensorPose.position.y = 0;
    sensorPose.position.z = 0;
    sensorPose.orientation.x = 0;
    sensorPose.orientation.y = 0;
    sensorPose.orientation.z = 0;
    sensorPose.orientation.w = 1;


    
    while(ros::ok())
    {
      if(!first_scan)
      {
	if(is_rear) 
	  realLaserScan.header.frame_id = "laser_link_rear";
	else 
	  realLaserScan.header.frame_id = "laser_link";
	pubScan.publish(realLaserScan);
      }
       
      ros::spinOnce();
      loop.sleep();
    }
    return 0;
}
