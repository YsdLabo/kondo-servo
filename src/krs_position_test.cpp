//
// KRS POSITION TEST
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
	ros::init(argc, argv, "krs_position_test_node");

	if( ics_init(&ics_data) < 0 ) {
		ROS_INFO("Could not init: %s", ics_data.error);
		exit(1);
	}
	ROS_INFO("Open ICS");

	signal(SIGINT, signal_handler);

	int sp = ics_get_speed(&ics_data, ID);
	printf("set speed : %d\n", sp);

	ics_set_speed(&ics_data, ID, 127);
	sp = ics_get_speed(&ics_data, ID);
	printf("set speed : %d\n", sp);

	while(ros::ok())
	{
		int zero = 7500;
		int start = zero + (int)(-90.0 / 270.0 * 8000);
		int end = zero + (int)(90.0 / 270.0 * 8000);
		int rate = (end - start) / 100;

		for(int i=start;i<=end;i+=rate) {
			ics_pos(&ics_data, ID, i);
			usleep(10000);
		}

		for(int i=end;i>=start;i-=rate) {
			ics_pos(&ics_data, ID, i);
			usleep(10000);
		}
	}


	return 0;
}
