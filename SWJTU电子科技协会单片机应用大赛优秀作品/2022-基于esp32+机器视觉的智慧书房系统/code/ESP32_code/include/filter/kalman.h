#ifndef __KALMAN_H__
#define __KALMAN_H__

typedef struct
{
    /*不用动*/
    double LastP;//上次估算协方差
    double Now_P;//当前估算协方差
    double out;//卡尔曼滤波器输出
    double Kg;//卡尔曼增益
	  double Q;
	  double R;
}Kalman_Typedef;

void Kalman_Init(Kalman_Typedef *klm, const double klm_Q, const double klm_R);
double KalmanFilter(Kalman_Typedef *klm, double input);

#endif