// Nexmo.cpp: 定義應用程式的進入點。
//
#include "Nexmo.h"


using namespace std;

int main(int argc, char* argv[])
{
	robot robot1;
	Pos_T pose;
	robot1.launch(DEVICE_SIMULATOR , 0 );

	cout << "Hello CMake." << endl;

	robot1.gotohome();
	
	robot1.set_velratio(80.0);

	/*
	
	*/
	pose.pos[0] = 200;
	pose.pos[1] = 0;
	pose.pos[2] = 500;
	pose.pos[3] = 0;
	pose.pos[4] = 0;
	pose.pos[5] = 180;


	robot1.PTP_Until_Task(pose);

	pose.pos[0] = 220;
	pose.pos[1] = 0;
	pose.pos[2] = 530;
	pose.pos[3] = 0;
	pose.pos[4] = 0;
	pose.pos[5] = 180;

	robot1.LINE_Until_Task(pose);



	robot1.set_mode(robot::Mode::Blending_coner);
	

	printf("-----------------------PTP_Queue_Task demo --------------\n");
	pose = robot1.get_ptp_pose();
	for (unsigned int i = 0; i < 3; i++) {
		pose.pos[i] += 10;
	}
	robot1.PTP_Queue_Task(pose);
	pose = robot1.get_ptp_pose();
	for (unsigned int i = 0; i < 6; i++) {
		
			printf("pose[%d] =   %f\n",i, pose.pos[i]);
	}

	Sleep(10);

	pose = robot1.get_ptp_pose();
	for (unsigned int i = 0; i < 3; i++) {
		pose.pos[i] += 10;
	}

	robot1.PTP_Queue_Task(pose);

	pose = robot1.get_ptp_pose();

	for (unsigned int i = 0; i < 6; i++) {

		printf("pose[%d] =   %f\n", i, pose.pos[i]);
	}

	Sleep(5000);
	pose = robot1.get_ptp_pose();

	for (unsigned int i = 0; i < 6; i++) {

		printf("pose[%d] =   %f\n", i, pose.pos[i]);
	}



	robot1.terminate();
	
	system("pause");
	return 0;

}
