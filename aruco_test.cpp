#include "aruco_test.h"

using namespace std;
using namespace cv;
using namespace aruco;

void aruco_thread() {

	cv::VideoCapture* capture = 0;
	cv::Mat frame;
	int width = 640;
	int height = 480;
	int camera_id = 0;

	//使用第一個相機   
	capture = new cv::VideoCapture(camera_id);
	capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);


	while (true) {
		//放入數據
		*capture >> frame;
		Mat image = frame;

		resize(image, image, Size(image.cols / 2, image.rows / 2));
		Mat imageCopy;
		image.copyTo(imageCopy);
		//載入Aruco用的dictionary ， 會根據這個去偵測在dictionary內有紀錄的Marker
		Ptr<aruco::Dictionary> dictionary = getPredefinedDictionary(cv::aruco::DICT_6X6_250);

		std::vector<int> ids;     //Marker 的id
		std::vector<std::vector<cv::Point2f> > corners;
		cv::aruco::detectMarkers(image, dictionary, corners, ids);  //這個是偵測Aruco的API
		
		//以下為相機的校正 ， 應該先校正再做量測
		cout << "num " << ids.size() << endl;
		Mat cameraMatrix = (Mat_<double>(3, 3) << 576.88606, 0, 326.23928, 0, 578.34444, 247.88512, 0, 0, 1);
		//double dist[4]={0.06108,-0.11259,-0.00508,0.00425};
		//vector<double>  distCoeffs(dist,dist+4);
		Mat distCoeffs = (Mat_<double>(1, 4) << 0.06108, -0.11259, -0.00508, 0.00425);

		
		if (ids.size() > 0)
		{
			cout << "num " << ids.size() << endl;
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
			vector< Vec3d > rvecs, tvecs;
			//獲得檢測出的pose向量 tves :為平移向量  rves :為旋轉向量
			cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
			
			//繪製Marker的邊緣  ，方便觀察
			for (int i = 0; i<ids.size(); i++)
			{
				cout << "t " << tvecs[i] << endl;
				cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
			}

		}
		//顯示opencv視窗
		cv::imshow("out", imageCopy);
		cvWaitKey(30);
		//每個30ms執行一次
		std::this_thread::sleep_for(std::chrono::microseconds(30));

	}

}

int main() {

	std::thread first(aruco_thread);
	std::cout << "test" << std::endl;
	
	first.join();
	system("pause");


	return 0;
}