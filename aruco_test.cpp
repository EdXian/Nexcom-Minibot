#include "aruco_test.h"
#include <chrono>
using namespace std;
using namespace cv;
using namespace aruco;

void aruco_thread() {

	cv::VideoCapture* capture = 0;
	cv::Mat frame;
	int width = 640;
	int height = 480;
	int camera_id = 1;

	//�ϥβĤ@�Ӭ۾�   
	capture = new cv::VideoCapture(camera_id);
	capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);
	int fps = capture->get(CAP_PROP_FPS);
	printf("Frame rate : %d \n", fps);

	while (true) {
		//��J�ƾ�
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		*capture >> frame;
		Mat image = frame;

		resize(image, image, Size(image.cols, image.rows));
		Mat imageCopy;
		image.copyTo(imageCopy);
		//���JAruco�Ϊ�dictionary �A �|�ھڳo�ӥh�����bdictionary����������Marker
		Ptr<aruco::Dictionary> dictionary = getPredefinedDictionary(cv::aruco::DICT_6X6_50);  //�ثe�u�ϥ�0~50��id

		std::vector<int> ids;     //Marker ��id
		std::vector<std::vector<cv::Point2f> > corners;
		cv::aruco::detectMarkers(image, dictionary, corners, ids);  //�o�ӬO����Aruco��API

																	//�H�U���۾����ե� �A ���ӥ��ե��A���q��
																	//cout << "num " << ids.size() << endl;
		Mat cameraMatrix = (Mat_<double>(3, 3) << 826.323941, 0.000000, 330.817699, 0, 827.488990, 261.420381, 0.000, 0.000, 1.0000);
		Mat distCoeffs = (Mat_<double>(1, 4) << 0.012823, -0.139212, 0.000502, -0.000537);


		if (ids.size() > 0)
		{
			//cout << "num " << ids.size() << endl;
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
			vector< Vec3d > rvecs, tvecs;
			//��o�˴��X��pose�V�q tves :�������V�q  rves :������V�q
			cv::aruco::estimatePoseSingleMarkers(corners, 0.0485, cameraMatrix, distCoeffs, rvecs, tvecs);

			//ø�sMarker����t  �A��K�[��
			for (int i = 0; i<ids.size(); i++)
			{
				cout << "r " << rvecs[i] << endl;
				cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
			}

		}
		//���opencv����
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;
		cv::imshow("Aruco��������", imageCopy);
		
		int a;
		a = cvWaitKey(3); //����esc���U��������
		if (a == 27) { break; }


		std::this_thread::sleep_for(std::chrono::microseconds(20));//�C��30ms����@��

	}

}

int main() {

	std::thread first(aruco_thread);
	std::cout << "test" << std::endl;
	
	first.join();
	system("pause");


	return 0;
}