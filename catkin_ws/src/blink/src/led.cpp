#include <wiringPi.h>
#include "ros/ros.h"
#include "std_msgs/String.h"

bool state = LOW;

void chatterCallback(const std_msgs::String::ConstPtr& msg) {
	ROS_INFO("I heard: [%s]", msg->data.c_str());
	digitalWrite(2, !state);
	state = !state;
}

int main(int argc, char** argv) {
	wiringPiSetup();
	pinMode(2, OUTPUT);
	ros::init(argc, argv, "led");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
	ros::spin();
	return 0;
}

