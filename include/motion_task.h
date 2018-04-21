#ifndef  _MOTION_TASK_H
#define _MOTION_TASK_H
#include <NexMotion_Handler.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define  sleep_500ms 500


class robot {
	enum Mode
	{
		None = 0,
		Buffer = 1,
		Blending_coner = 2,
		Blending_dev =3
	};
public :
	robot();
	~robot();
	bool launch(I32_T devType, I32_T  devIndex);
	bool terminate();
	bool PTP_Until_Task(Pos_T & target);
	bool set_velratio(float percetage);
	bool gotohome();
	bool LINE_Until_Task(Pos_T & target);
	bool set_mode(int mode);


	int task_a(int x, int y);

	Pos_T  get_ptp_pose(void);
	Pos_T  get_acs_pose(void);
	I32_T  get_buffer_size(void);
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
	I32_T velocity_ratio = 0;
	Pos_T actual_pose;
	I32_T buffer_size = 0;


	int mode = 0;
	



};







#endif