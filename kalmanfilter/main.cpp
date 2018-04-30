
#include <iostream>
#include "KalmanFilter.h"

using namespace std;

int main(int argc, char const *argv[])
{

  /* Set Matrix and Vector for Kalman Filter: */
  MatrixXf A(1, 1); A << 1;
  MatrixXf H(1, 1); H << 1;
  MatrixXf Q(1, 1); Q << 0;
  MatrixXf R(1, 1); R << 0.1;
  VectorXf X0(1); X0 << 0;
  MatrixXf P0(1, 1); P0 << 1;


  KalmanFilter filter1(1, 0);

  filter1.setFixed(A, H, Q, R);
  filter1.setInitial(X0, P0);

  
  VectorXf Z(1);
  float mesaure[10] = {0.39, 0.50, 0.48, 0.29, 1, 0.32, 0.34, 0.48, 0.41, 0.45};

  /* This loop simulate the measure/prediction process */
  for (int i = 0; i < 10; ++i)
  {
    filter1.predict(); //Predict phase
    Z << mesaure[i];
    filter1.correct( Z ); //Correction phase
	std::cout << Z ;
	cout <<"\t" << filter1.X << endl;
	std::cout << "-------------" << std::endl;
  }
  system("pause");
	return 0;
}
