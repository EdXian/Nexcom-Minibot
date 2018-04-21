#include "motion_task.h"

robot::robot() {


}
robot::~robot() {


}

bool robot::launch(I32_T devType , I32_T  devIndex) {


	ret = NMC_DeviceOpenUp(devType, devIndex, &retDevID);

	if (ret != 0)
	{
		printf("ERROR! NMC_DeviceOpenUp: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		return false;
	}
	else {
			printf("\nDevice open up succeed, device ID: %d.\n", retDevID);
	}

	ret = NMC_DeviceGetState(retDevID, &retDevState);

	if (retDevState != NMC_DEVICE_STATE_OPERATION)
	{
		printf("ERROR! Device open up failed, device ID: %d.\n", retDevID);
		return false;
	}
	else {
		printf("Device ID %d: state is OPERATION.\n", retDevID);
	}

	ret = NMC_DeviceGetAxisCount(retDevID, &retSingleAxisCount);
		
	if (ret != 0)
	{
		printf("ERROR! NMC_DeviceGetAxisCount: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		return false;
	}
	else {
		printf("\nGet single axis count succeed, device has %d single axis.\n", retSingleAxisCount);
	}
	
	ret = NMC_DeviceGetGroupCount(retDevID, &retGroupCount);
	if (ret != 0)
	{
		printf("ERROR! NMC_DeviceGetGroupCount: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		return false;
	}
	else {
		printf("Get group count succeed, device has %d group.\n", retGroupCount);
	}
		
	for (i = 0; i < retGroupCount; i++)
	{
		ret = NMC_DeviceGetGroupAxisCount(retDevID, i, &retGroupAxisCount);
		if (ret != 0)
		{
			printf("ERROR! NMC_DeviceGetGroupAxisCount: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}
		else
			printf("Get group axis count succeed, group index %d has %d axis.\n", i, retGroupAxisCount);
	}

	printf("\nReady to reset all drives in device...\n", i);


	for (i = 0; i < retSingleAxisCount; i++)
	{
		ret = NMC_AxisResetDriveAlm(retDevID, i);
		if (ret != 0)
		{
			printf("ERROR! NMC_AxisResetDriveAlm(axis index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}
	}

	Sleep(sleep_500ms);

	for (i = 0; i < retSingleAxisCount; i++)
	{
		ret = NMC_AxisGetState(retDevID, i, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetState: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}

		if (retState != NMC_AXIS_STATE_DISABLE)
		{
			printf("ERROR! Single axis reset failed.(axis index %d)\n", i);
			return false;
		}
		else {
			printf("Single axis reset succeed.(axis index %d)\n", i);
		}
			
	}

	Sleep(sleep_500ms);

	for (i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupResetDriveAlmAll(retDevID, i);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupResetDriveAlmAll: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}
	}

	Sleep(sleep_500ms);

	for (i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupGetState(retDevID, i, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetState(group index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}

		if (retState != NMC_GROUP_STATE_DISABLE)
		{
			printf("ERROR! Group reset failed.(group index %d)\n", i);
			return false;
		}
		else {
			printf("Group reset succeed.(group index %d)\n", i);
		}
			
	}

	Sleep(sleep_500ms);

	for (i = 0; i < retGroupCount; i++)
	{
		ret = NMC_DeviceEnableAll(retDevID);
		if (ret != 0)
		{
			printf("ERROR! NMC_DeviceEnableAll: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}
		else
			printf("\nReady to enable all single axes and groups...\n");
	}

	Sleep(sleep_500ms);

	for (i = 0; i < retSingleAxisCount; i++)
	{
		ret = NMC_AxisGetState(retDevID, i, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_AxisGetState(axis index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}

		if (retState != NMC_AXIS_STATE_STAND_STILL)
		{
			printf("ERROR! Axis enable failed.(axis index %d)\n", i);
			return false;
		}
		else
			printf("Axis enable succeed.(axis index %d)\n", i);
	}

	
	for (i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupGetState(retDevID, i, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetState(group index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}

		if (retState != NMC_GROUP_STATE_STAND_STILL)
		{
			printf("ERROR! Group enable failed.(group index %d)\n", i);
			return false;
		}
		else {
			printf("Group enable succeed.(group index %d)\n", i);
		}
			
	}

	Sleep(sleep_500ms);

	return true;
}


bool robot::terminate() {

	for (i = 0; i < retGroupCount; i++)
	{
		ret = NMC_DeviceDisableAll(retDevID);
		if (ret != 0)
		{
			printf("ERROR! NMC_DeviceDisableAll: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}
		else {
			printf("\nReady to disable all single axes and groups...\n");
		}
			
	}

	
	Sleep(sleep_500ms);

	for (i = 0; i < retSingleAxisCount; i++)
	{
		ret = NMC_AxisGetState(retDevID, i, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_AxisGetState(axis index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}

		if (retState != NMC_AXIS_STATE_DISABLE)
		{
			printf("ERROR! Axis disable failed.(axis index %d)\n", i);
			return false;
		}
		else
			printf("Axis disable succeed.(axis index %d)\n", i);
	}

	//check group state
	for (i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupGetState(retDevID, 0, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetState(group index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			return false;
		}

		if (retState != NMC_GROUP_STATE_DISABLE)
		{
			printf("ERROR! Group disable failed.(group index %d)\n", i);
			return false;
		}
		else
			printf("Group disable succeed.(group index %d)\n", i);
	}

	ret = NMC_DeviceShutdown(retDevID);

	if (ret != 0) {
		printf("ERROR! NMC_DeviceShutdown: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
	}
	else {
		printf("\nDevice shutdown succeed.\n");
	}
	return true;
}


bool robot::gotohome() {

	I32_T Group_Axis_Control_Mask = 0;
	Group_Axis_Control_Mask = NMC_GROUP_AXIS_MASK_X | NMC_GROUP_AXIS_MASK_Y | NMC_GROUP_AXIS_MASK_Z |
		NMC_GROUP_AXIS_MASK_A | NMC_GROUP_AXIS_MASK_B | NMC_GROUP_AXIS_MASK_C;

	ret = NMC_GroupAxesHomeDrive(this->retDevID, this->devIndex , Group_Axis_Control_Mask);

	if (ret != ERR_NEXMOTION_SUCCESS) {

		printf("PTP_Until_Task error code :  %d\n", ret);
		return false;
	}

	do {
		ret = NMC_GroupGetState(this->retDevID, this->devIndex, &Group_State);
	} while (Group_State == NMC_GROUP_STATE_HOMING);

	Sleep(sleep_500ms);

	ret = NMC_GroupGetActualPosPcs(this->retDevID, this->devIndex, &actual_pose);
	printf("robot is set to home\n");
	printf("x = %f \n", actual_pose.pos[0]);
	printf("y = %f \n", actual_pose.pos[1]);
	printf("z = %f \n", actual_pose.pos[2]);
	printf("a = %f \n", actual_pose.pos[3]);
	printf("b = %f \n", actual_pose.pos[4]);
	printf("c = %f \n", actual_pose.pos[5]);

	return true;
}

bool robot::setvelratio(float percetage) {
	if ((percetage > 100.0) || (percetage < 0.0))
	{
		printf("SetVelRatio invalid parameter\n ");
		return false;
	}
	else {
		ret = NMC_GroupSetVelRatio(this->retDevID, this->devIndex, percetage);
	}
	
	if (ret != ERR_NEXMOTION_SUCCESS) {
		printf("SetVelRatio failed error code \n", ret);
	}

	return true;
}

bool robot::PTP_Until_Task(Pos_T & target) {

	I32_T Group_Axis_Control_Mask = 0;
	Group_Axis_Control_Mask = NMC_GROUP_AXIS_MASK_X | NMC_GROUP_AXIS_MASK_Y | NMC_GROUP_AXIS_MASK_Z |
								NMC_GROUP_AXIS_MASK_A | NMC_GROUP_AXIS_MASK_B | NMC_GROUP_AXIS_MASK_C;

	ret = NMC_GroupGetState(this->retDevID , this->devIndex, & Group_State);

	printf(" Group_State  =  %d\n", Group_State);

	ret = NMC_GroupPtpCartAll(this->retDevID, this->devIndex, Group_Axis_Control_Mask, &target);
	if (ret != ERR_NEXMOTION_SUCCESS) {

		printf("PTP_Until_Task error code :  %d\n", ret);
		return false;
	}

	do{
		ret = NMC_GroupGetState(this->retDevID, this->devIndex, &Group_State);

	} while (Group_State != NMC_GROUP_STATE_STAND_STILL);
	
	ret = NMC_GroupGetActualPosPcs(this->retDevID, this->devIndex, &actual_pose);
	printf("PTP task result\n");
	printf("x = %f \n", actual_pose.pos[0]);
	printf("y = %f \n", actual_pose.pos[1]);
	printf("z = %f \n", actual_pose.pos[2]);
	printf("a = %f \n", actual_pose.pos[3]);
	printf("b = %f \n", actual_pose.pos[4]);
	printf("c = %f \n", actual_pose.pos[5]);


	return true;
}

bool robot::LINE_Until_Task(Pos_T & target) {
	I32_T Group_Axis_Control_Mask = 0;
	Group_Axis_Control_Mask = NMC_GROUP_AXIS_MASK_X | NMC_GROUP_AXIS_MASK_Y | NMC_GROUP_AXIS_MASK_Z |
		NMC_GROUP_AXIS_MASK_A | NMC_GROUP_AXIS_MASK_B | NMC_GROUP_AXIS_MASK_C;

	NMC_GroupLine(this->retDevID, this->devIndex, Group_Axis_Control_Mask , &target,NULL);
	do {
		ret = NMC_GroupGetState(this->retDevID, this->devIndex, &Group_State);
	} while (Group_State != NMC_GROUP_STATE_STAND_STILL);

	ret = NMC_GroupGetActualPosPcs(this->retDevID, this->devIndex, &actual_pose);
	printf("PTP task result\n");
	printf("x = %f \n", actual_pose.pos[0]);
	printf("y = %f \n", actual_pose.pos[1]);
	printf("z = %f \n", actual_pose.pos[2]);
	printf("a = %f \n", actual_pose.pos[3]);
	printf("b = %f \n", actual_pose.pos[4]);
	printf("c = %f \n", actual_pose.pos[5]);
	return true;
}




int robot::task_a(int x, int y) {


	return x + y;
	
}





