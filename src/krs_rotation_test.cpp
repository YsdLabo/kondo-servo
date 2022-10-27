//
// KRS ROTATION TEST
//

#include<ros/ros.h>
#include<signal.h>
#include"ics.h"

#define ID 1

ICSData ics_data;

void signal_handler(int signum)
{
	ics_pos(&ics_data, ID, 7500);
	ics_close(&ics_data);
	ROS_INFO("Close ICS");
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "krs_rotation_test_node");

	if( ics_init(&ics_data) < 0 ) {
		ROS_INFO("Could not init: %s", ics_data.error);
		exit(1);
	}
	ROS_INFO("Open ICS");

	signal(SIGINT, signal_handler);

	int zero = 7500;

	double speed = 0.3; // < 1 rps
	int rate = (int)(speed * 0.01 * 8000 * 360.0 / 270.0) + zero;
	if(rate > zero + 250) rate = zero + 250;
	else if(rate < zero - 250) rate = zero - 250;

	ics_pos(&ics_data, ID, rate);

	while(ros::ok());

	return 0;
}
