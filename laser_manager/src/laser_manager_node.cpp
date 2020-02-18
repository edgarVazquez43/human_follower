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

/*
  This code is for transform laser sensor with respect to base_link  if the robot
*/
sensor_msgs::LaserScan realLaserScan;
sensor_msgs::LaserScan scanWrtRobot_msg;

bool first_scan = true;


void callback_laser_scan(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    realLaserScan = *msg;
    int range_size = realLaserScan.ranges.size();
    
    scanWrtRobot_msg.angle_min = -realLaserScan.angle_max;
    scanWrtRobot_msg.angle_max = -realLaserScan.angle_min;
    scanWrtRobot_msg.angle_increment = realLaserScan.angle_increment;
    scanWrtRobot_msg.scan_time = realLaserScan.scan_time;
    scanWrtRobot_msg.range_min = realLaserScan.range_min;
    scanWrtRobot_msg.range_max = realLaserScan.range_max;
    
    // //Transform laser sensor
    for(int i = 0; i < range_size; i++)
    {
      scanWrtRobot_msg.ranges[i] = realLaserScan.ranges[range_size - 1 - i ];
      // std::cout << "ranges: " << realLaserScan.ranges[i]
      // 		<< "  i: " << i << "  (size -1 - i):  " << range_size -1 - i << std::endl;
    }
    
    if(first_scan)
        first_scan = false;
}


int main(int argc, char** argv)
{
    std::cout << "INITIALIZING LASER MANAGER BY EDD-2..." << std::endl;
    ros::init(argc, argv, "laser_manager");
    ros::NodeHandle n;
  
    std::cout << "LaserManager.->USING LASER IN REAL MODE !!!" << std::endl;
        ros::Subscriber subRealLaserScan = n.subscribe("/scan", 1, callback_laser_scan);
    ros::Publisher  pubScan          = n.advertise<sensor_msgs::LaserScan>("/scan_wrt_robot", 1);

    ros::Rate loop(60);

   
    scanWrtRobot_msg.header.frame_id = "base_link";
    scanWrtRobot_msg.ranges.resize(726);

    
    while(ros::ok())
    {
      
      if(!first_scan)
      {
      	pubScan.publish(scanWrtRobot_msg);
      }
      
      ros::spinOnce();
      loop.sleep();
    }
    return 0;
}
