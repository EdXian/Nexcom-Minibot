// Nexmo.cpp: 定義應用程式的進入點。
//
#include "Nexmo.h"
#include <chrono>
#include "kalmanFilter.h"

using namespace std;
using namespace cv;
using namespace aruco;

vector< Vec3d > tvecs;
bool Camera_shouldwork = true;
bool camera_isopen = true;


void aruco_thread() {

	//create three kalmanfilter
	MatrixXf A(1, 1); A << 1;
	MatrixXf H(1, 1); H << 1;
	MatrixXf Q(1, 1); Q << 0;
	MatrixXf R(1, 1); R << 0.1;
	VectorXf X0(1); X0 << 0;
	MatrixXf P0(1, 1); P0 << 1;

	kalmanFilter filter_x(1, 0);
	kalmanFilter filter_y(1, 0);
	kalmanFilter filter_z(1, 0);

	filter_x.setFixed(A, H, Q, R);	
	filter_x.setInitial(X0, P0);

	filter_y.setFixed(A, H, Q, R);
	filter_y.setInitial(X0, P0);

	filter_z.setFixed(A, H, Q, R);
	filter_z.setInitial(X0, P0);
	VectorXf Z_x(1) , Z_y(1), Z_z(1);

	cv::VideoCapture* capture ;
	cv::Mat frame;
	int width =640;
	int height = 480;
	int camera_id = 0;
	
	//使用第一個相機   
	capture = new cv::VideoCapture(camera_id);
	capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);
	int fps = capture->get(CAP_PROP_FPS);
	printf("Frame rate : %d \n", fps);


	for (;;) {
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		if (Camera_shouldwork==false) {
			break;
		}
		if (capture->isOpened() == false) {
			camera_isopen = false;
			break;
		}
		//放入數據
		*capture >> frame;
		Mat image = frame;

		resize(image, image, Size(image.cols , image.rows ));
		Mat imageCopy;
		image.copyTo(imageCopy);
		//載入Aruco用的dictionary ， 會根據這個去偵測在dictionary內有紀錄的Marker
		Ptr<aruco::Dictionary> dictionary = getPredefinedDictionary(cv::aruco::DICT_6X6_250);

		std::vector<int> ids;     //Marker 的id
		std::vector<std::vector<cv::Point2f> > corners;
		cv::aruco::detectMarkers(image, dictionary, corners, ids);  //這個是偵測Aruco的API

																	//以下為相機的校正 ， 應該先校正再做量測
		//cout << "num " << ids.size() << endl;
		Mat cameraMatrix = (Mat_<double>(3, 3) << 826.323941, 0.000000, 330.817699, 0, 827.488990, 261.420381, 0.000, 0.000, 1.0000);
		Mat distCoeffs = (Mat_<double>(1, 4) << 0.012823, -0.139212, 0.000502, -0.000537);
		
		if (ids.size() > 0)
		{
			//cout << "num " << ids.size() << endl;
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
			vector< Vec3d > rvecs ;
			
			//獲得檢測出的pose向量 tves :為平移向量  rves :為旋轉向量
			filter_x.predict(); 
			filter_y.predict(); 
			filter_z.predict(); 

			cv::aruco::estimatePoseSingleMarkers(corners, 0.0485, cameraMatrix, distCoeffs, rvecs, tvecs);
			Vec3d translate = tvecs[0];
			Z_x << translate.val[0];
			Z_y << translate.val[1];
			Z_z << translate.val[2];

			filter_x.correct(Z_x); 
			filter_y.correct(Z_y);
			filter_z.correct(Z_z);
			std::cout <<"x :" <<filter_x.X 
				<< "ｙ :" << filter_y.X
				<< "ｚ :" << filter_z.X
				<< std::endl;
			
			//繪製Marker的邊緣  ，方便觀察
			for (int i = 0; i<ids.size(); i++)
			{
				//cout << "t " << tvecs[i] << endl;

		//		cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
			}
		}
		//顯示opencv視窗
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;
		cv::imshow("Aruco偵測視窗", imageCopy);
		cvWaitKey(3); //等待esc按下關閉式窗
		std::this_thread::sleep_for(std::chrono::microseconds(33));//每個15ms執行一次

	}

}

int main(int argc, char* argv[])
{
	
	robot robot1;
	Pos_T pose;
	Pos_T target_pose;
	std::thread camera_thread(aruco_thread);
	Sleep(2000);
	if (camera_isopen == false) {
		printf("Camera is not found\n");
		camera_thread.join();
	}
	robot1.launch(DEVICE_SIMULATOR, 0 );
	cout << "Hello Robot!" << endl;
#ifdef gohome
	robot1.gotohome();
#endif // gohome


	robot1.set_velratio(100.0);
	robot1.set_mode(robot::Mode::Blending_coner);
	target_pose.pos[0] = 250;
	target_pose.pos[1] = 0;
	target_pose.pos[2] = 500;
	target_pose.pos[3] = 0;
	target_pose.pos[4] = 0;
	target_pose.pos[5] = 180;
	robot1.PTP_Until_Task(target_pose);
	/*
	
	*/
	target_pose.pos[3] = 0;
	target_pose.pos[4] = 0;
	target_pose.pos[5] = 180;
	
	//target_pose.pos[0] =     //x

	while (1) {
		
	    Vec3d u= tvecs[0];
		
		pose = robot1.get_ptp_pose();
		
		target_pose.pos[0] = pose.pos[0]- u.val[1]*1000;
			target_pose.pos[1] = pose.pos[1]- u.val[0]*1000;
		target_pose.pos[2] = pose.pos[2] - (u.val[2]-0.40)*1000;
		if (robot1.get_buffer_size() > 10) {
			robot1.PTP_Queue_Task(target_pose);
		}
		
		printf("x = %f\n", u.val[0]);
		printf("y = %f\n", u.val[1]);
		printf("z = %f\n", u.val[2]);
		Sleep(10);
	}
	
	/*
	pose.pos[0] = 250;
	pose.pos[1] = 0;
	pose.pos[2] = 500;
	pose.pos[3] = 0;
	pose.pos[4] = 0;
	pose.pos[5] = 180;


	robot1.PTP_Until_Task(pose);


	pose.pos[0] = 280;
	pose.pos[1] = 0;
	pose.pos[2] = 530;
	pose.pos[3] = 0;
	pose.pos[4] = 0;
	pose.pos[5] = 180;

	robot1.LINE_Until_Task(pose);
	*/
	


	Sleep(10000);

	if (Camera_shouldwork) {
		Camera_shouldwork = false;
		camera_thread.join();//一定要等待thread結束，才能關閉主程式
	}
	
	

	robot1.terminate();
	
	system("pause");
	return 0;

}
