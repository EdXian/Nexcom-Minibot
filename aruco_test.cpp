#include "aruco_test.h"

using namespace std;
using namespace cv;
using namespace aruco;

void aruco_thread() {

	cv::VideoCapture* capture = 0;
	cv::Mat frame;
	int width = 640;
	int height = 480;
	capture = new cv::VideoCapture(0);
	capture->set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	while (true) {
		*capture >> frame;
		Mat image = frame;
		resize(image, image, Size(image.cols / 2, image.rows / 2));
		Mat imageCopy;
		image.copyTo(imageCopy);
		Ptr<aruco::Dictionary> dictionary = getPredefinedDictionary(cv::aruco::DICT_6X6_250);
		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f> > corners;
		cv::aruco::detectMarkers(image, dictionary, corners, ids);
		// if at least one marker detected
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
			cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
			// draw axis for each marker
			for (int i = 0; i<ids.size(); i++)
			{
				cout << "t " << tvecs[i] << endl;
				cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
			}
		}
		cv::imshow("out", imageCopy);
		cvWaitKey(30);

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