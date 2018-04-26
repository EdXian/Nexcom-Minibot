#ifndef  _MOTION_TASK_H
#define  _MOTION_TASK_H
#include <NexMotion_Handler.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <vector>

#define  sleep_500ms 500


class robot {
	
public :
	enum Mode
	{
		None = 0,
		Buffer = 1,
		Blending_coner = 2,
		Blending_dev = 3
	};

	robot();
	~robot();
	 //常用功能
	bool launch(I32_T devType, I32_T  devIndex);//系統開啟
	bool terminate();                           //系統關閉
	
	bool gotohome();                            //手臂歸位

	bool PTP_Until_Task(Pos_T & target);        //點對點控制
	bool LINE_Until_Task(Pos_T & target);       //線運動控制
	
	bool PTP_Queue_Task(std::vector<Pos_T> & target);
	bool LINE_Queue_Task(std::vector<Pos_T> & target);
	bool PTP_Queue_Task(Pos_T & target);					//Buffer 模式下才能用
	bool LINE_Queue_Task(Pos_T & target);					//Buffer 模式下才能用



	//自訂議task
	int task_a(int x, int y);


	//setter
	bool set_mode(int mode);                    //模式選擇     0: None(Abort) 1: Buffer  2: blending
	bool set_velratio(float percetage);         //1%~100%

	//getter
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
	I32_T Group_State = 0;         //手臂狀態
	I32_T Group_Status = 0;        //系統狀態
	I32_T velocity_ratio = 0;	   //位置速度 
	Pos_T actual_pose;             //三度空間位置 
	I32_T buffer_size = 0;         //Motion queue 的剩餘長度


	int mode = 0;                  
	



};







#endif