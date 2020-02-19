# laser_mannager

The aim of this project is provide some ROS packages to detect a legs person and tracking using laser sensor URG and turtlebot.

-------------------------------

Please make sure that some ROS packages are already installed:

- ros-kinetic-urg-node

If not, you can installed with the follow command: 

> sudo apt-get install ros-kinetic-urg-node

After that you can check the ID device of the URG sensor.

> rosrun urg_node getID /dev/ttyACM0

You should see something like that:

`Device at /dev/ttyACM0 has ID H0807228`

In order to not be confused with sensor USB devices set the specific ID for each device configure the system rules. You can find those in:

/etc/udev/rules.d/

You need create a new rule file, or copy the rule file provide on this repository with the next command:

> sudo cp toInstall/80-turtlebot.rules /etc/udev/rules.d/


after that you need restart the udev rules:

> sudo udevadm control --reload-rules && sudo service udev restart && sudo udevadm trigger


Please, unplug the laser sensor and plug-in again. if you type 

> ls /dev/sensor

you should see in terminal something like:

`hokuyo_H1107988`

Please make sure that change the ID_device into `human_follower/launch/human_follower.launch`

In the lines:

`<param name="serial_port" type="string" value="/dev/sensors/$ID_DEVICE" />`

For example:

`<param name="serial_port" type="string" value="/dev/sensors/hokuyo_H1107988" />`

-----------------------


# Human Follower

This work was developed for PUMAS team as a degree work for some National Autonomous University of Mexico. This work is based on leg_finder algorithms and some trackers.

The package contains two ROS nodes, `human_follower` and `leg_finder`, these two ROS node contain some publisher and subscribers very useful.  

### `leg_finder`
 - Publishers:
	 - legs_found
		 - topic_name: `/hri/leg_finder/legs_found`
		 - topic_type: `std_msgs::Bool`
	 - legs_pose
		 - topic_name:  `/hri/leg_finder/leg_poses`
		 - topic_type:  `geometry_msgs::Marker`
	 - legs_hypotesis
		 - topic_name: `/hri/visualization_marker`
		 - topic_type:  `visualization_msgs::Marker`
 - Subscriber:
	 - laser_scan
		 - topic_name:  `/hsrb/base_scan`
		 - topic_type:   `sensor_msgs::LaserScan`
	 - leg_finder_enable
		 - topic_name: `/hri/leg_finder/enable`
		 - topic_type: `std_msds::Bool`


### `human_follower`
 - Publishers:
	 - cmd_velocity
		 - topic_name: `/hsrb/command_velocity`
		 - topic_type: `geometry_msgs::Twist`
	 - head_pose
		 - topic_name: `/manipulation/manip_pln/hd_goto_angles`
		 - topic_type:  `std_msgs::Float32MultiArray`
 - Subscriber:
	 - start_follow 
		 - topic_name: `/hri/human_following/start_follow`
		 - topic_type:  `std_msgs::Bool`
	 - legs_position
		 - topic_name: `/hri/leg_finder/leg_poses`
		 - topic_type: 
	 - potetial_fields_force
		 - topic_name: `/navigation/obs_avoid/pot_fields/rejective_force`
		 - topic_type:

*Note: please note that the complete name of the topics can be different if you are using a launch file for running the `human_folower` and `leg_finder` nodes. 

## Use
A launch file can be used for launch the ROS nodes needed, it is located in `human_follower/launch/human_follower.launch`, once the two ROS nodes are running `leg_finder_node` will be waiting for the topic `/leg_finder/enable` when it occur  the leg_finder will be looking for human legs. 

> rostopic pub /leg_finder/enable std_msgs/Bool "data: true" 


**You should see a marker in Rviz.**
`/hri/visualization_marker`


The `/leg_finder/legs_found` topic will be published when human legs were found whit a `true` value, otherwise the topic will be published with a `false` value when the human legs are lost.

Once you have received the topic that legs were found, you need publish the topic `/human_following/start_follow` with a `True` value for enable the robot movement. 

> rostopic pub /human_following/start_follow std_msgs/Bool "data: true" 

For **stop** human tracking you need publish it once again with a `False` value.

> rostopic pub /human_following/start_follow std_msgs/Bool "data: false" 
