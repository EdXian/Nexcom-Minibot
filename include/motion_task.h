#ifndef  _MOTION_TASK_H
#define _MOTION_TASK_H
#include <NexMotion_Handler.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define  sleep_500ms 500


class robot {
	
public :
	robot();
	~robot();
	bool launch(I32_T devType, I32_T  devIndex);
	bool terminate();
	bool PTP_Until_Task(Pos_T & target);
	bool setvelratio(float percetage);
	bool gotohome();
	bool LINE_Until_Task(Pos_T & target);

	int task_a(int x, int y);



private : 
	//
	RTN_ERR ret = 0;
	I32_T   devIndex = 0;
	I32_T   retDevID = 0;
	I32_T   retSingleAxisCount = 0;
	I32_T   retGroupCount = 0;
	I32_T   retGroupAxisCount = 0;
	I32_T    i = 0;
	I32_T   retState = 0;
	I32_T   retDevState = 0;
	//
	I32_T Group_State = 0;
	I32_T Group_Status = 0;
	Pos_T actual_pose;




};







#endif