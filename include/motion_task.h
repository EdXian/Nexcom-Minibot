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
	 //�`�Υ\��
	bool launch(I32_T devType, I32_T  devIndex);//�t�ζ}��
	bool terminate();                           //�t������
	
	bool gotohome();                            //���u�k��

	bool PTP_Until_Task(Pos_T & target);        //�I���I����
	bool LINE_Until_Task(Pos_T & target);       //�u�B�ʱ���
	
	bool PTP_Queue_Task(std::vector<Pos_T> & target);
	bool LINE_Queue_Task(std::vector<Pos_T> & target);
	bool PTP_Queue_Task(Pos_T & target);					//Buffer �Ҧ��U�~���
	bool LINE_Queue_Task(Pos_T & target);					//Buffer �Ҧ��U�~���



	//�ۭqĳtask
	int task_a(int x, int y);


	//setter
	bool set_mode(int mode);                    //�Ҧ����     0: None(Abort) 1: Buffer  2: blending
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
	I32_T Group_State = 0;         //���u���A
	I32_T Group_Status = 0;        //�t�Ϊ��A
	I32_T velocity_ratio = 0;	   //��m�t�� 
	Pos_T actual_pose;             //�T�תŶ���m 
	I32_T buffer_size = 0;         //Motion queue ���Ѿl����


	int mode = 0;                  
	



};







#endif