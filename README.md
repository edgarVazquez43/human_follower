# laser_mannager

This project is for work with laser sensor URG and followme task for turtlebot

-------------------------------

Need to be installed ros urg node:

sudo apt-get install ros-kinetic-urg-node

$ rosrun urg_node getID /dev/ttyACM0

Device at /dev/ttyACM0 has ID H0807228

In order to not be confudsed with sensor USB devices set the specific ID for each device configure the system rules. You can find those in:

/etc/udev/rules.d/

You need create a new rule file, or copy the rule file provide on this repository with the next command:

sudo cp ToInstall/USB/80-turtlebot.rules /etc/udev/rules.d/


after that you need restart the udev rules:

sudo udevadm control --reload-rules && sudo service udev restart && sudo udevadm trigger

