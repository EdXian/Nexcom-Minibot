#include "aruco_test.h"
#include <chrono>
#include "kalmanFilter.h"
using namespace std;
using namespace cv;
using namespace aruco;

vector< Vec3d > tvecs;
bool Camera_shouldwork = true;
bool camera_isopen = true;

/*

*/
//void aruco_thread() {
//
//	cv::VideoCapture* capture;
//	cv::Mat frame;
//	int width = 640;
//	int height = 480;
//	int camera_id = 0;
//
//	//使用第一個相機   
//	capture = new cv::VideoCapture(camera_id);
//	capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
//	capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);
//	capture->set(CV_CAP_PROP_FPS, 60);
//	int fps = capture->get(CAP_PROP_FPS);
//	printf("Frame rate : %d \n", fps);
//	Ptr<aruco::DetectorParameters> detect_parameter = DetectorParameters::create();
//	/*detect_parameter->adaptiveThreshWinSizeStep = 1;
//	detect_parameter->adaptiveThreshWinSizeMax = 6;
//	detect_parameter->adaptiveThreshWinSizeMin = 4;
//
//	detect_parameter->maxMarkerPerimeterRate = 2.5;
//	detect_parameter->minMarkerPerimeterRate = 0.1;
//	detect_parameter->polygonalApproxAccuracyRate = 0.04;
//	std::cout << detect_parameter->minCornerDistanceRate << std::endl;
//	std::cout << detect_parameter->minMarkerDistanceRate << std::endl;
//	std::cout << detect_parameter->minDistanceToBorder << std::endl;
//	detect_parameter->minCornerDistanceRate = 0.05;
//	detect_parameter->minMarkerDistanceRate = 0.05;
//	detect_parameter->minDistanceToBorder = 3;
//	detect_parameter->markerBorderBits =1;
//
//
//	detect_parameter->minOtsuStdDev = 5.0;
//	detect_parameter->perspectiveRemoveIgnoredMarginPerCell = 0.13;
//	
//	detect_parameter->maxErroneousBitsInBorderRate = 0.35;
//	detect_parameter->errorCorrectionRate = 0.6;
//
//	detect_parameter->cornerRefinementMethod = CORNER_REFINE_SUBPIX;
//	detect_parameter->cornerRefinementWinSize = 5;
//
//	detect_parameter->cornerRefinementMaxIterations = 30;
//	detect_parameter->cornerRefinementMinAccuracy = 0.1;*/
//
//	std::cout << detect_parameter->adaptiveThreshWinSizeStep <<"  "
//		<< detect_parameter->adaptiveThreshWinSizeMax << "  "
//		<< detect_parameter->adaptiveThreshWinSizeMin << "  "
//		<< std::endl;
//
//
//	while (true) {
//		//放入數據
//		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
//		*capture >> frame;
//		Mat image = frame;
//
//		resize(image, image, Size(image.cols, image.rows));
//		Mat imageCopy;
//		image.copyTo(imageCopy);
//		//載入Aruco用的dictionary ， 會根據這個去偵測在dictionary內有紀錄的Marker
//		Ptr<aruco::Dictionary> dictionary = getPredefinedDictionary(cv::aruco::DICT_6X6_50);  //目前只使用0~50的id
//
//		std::vector<int> ids;     //Marker 的id
//		std::vector<std::vector<cv::Point2f> > corners;
//		//cv::aruco::detectMarkers(image, dictionary, corners, ids,detect_parameter);  //這個是偵測Aruco的API
//		cv::aruco::detectMarkers(image, dictionary, corners, ids);
//			//以下為相機的校正 ， 應該先校正再做量測																						//cout << "num " << ids.size() << endl;
//		Mat cameraMatrix = (Mat_<double>(3, 3) << 826.323941, 0.000000, 330.817699, 0, 827.488990, 261.420381, 0.000, 0.000, 1.0000);
//		Mat distCoeffs = (Mat_<double>(1, 4) << 0.012823, -0.139212, 0.000502, -0.000537);
//		
//		if (ids.size() > 0)
//		{
//			//cout << "num " << ids.size() << endl;
//			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
//			vector< Vec3d > rvecs, tvecs;
//			//獲得檢測出的pose向量 tves :為平移向量  rves :為旋轉向量
//			cv::aruco::estimatePoseSingleMarkers(corners, 0.0485, cameraMatrix, distCoeffs, rvecs, tvecs);
//
//			//繪製Marker的邊緣  ，方便觀察
//			for (int i = 0; i<ids.size(); i++)
//			{
//				cout << "r " << rvecs[i] << endl;
//				cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
//			}
//
//		}
//		//顯示opencv視窗
//		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
//		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;
//		cv::imshow("Aruco偵測視窗", imageCopy);
//		
//		int a;
//		a = cvWaitKey(3); //等待esc按下關閉式窗
//		if (a == 27) { break; }
//
//
//		std::this_thread::sleep_for(std::chrono::microseconds(20));//每個30ms執行一次
//
//	}
//
//}
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
	VectorXf Z_x(1), Z_y(1), Z_z(1);

	cv::VideoCapture* capture;
	cv::Mat frame;
	int width = 640;
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
		if (Camera_shouldwork == false) {
			break;
		}
		if (capture->isOpened() == false) {
			camera_isopen = false;
			break;
		}
		//放入數據
		*capture >> frame;
		Mat image = frame;

		resize(image, image, Size(image.cols, image.rows));
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
			vector< Vec3d > rvecs;

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
			std::cout << "x :" << filter_x.X
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











int main() {

	std::thread first(aruco_thread);
	std::cout << "test" << std::endl;
	
	first.join();
	//system("pause");


	return 0;
}