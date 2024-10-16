/*
卡尔曼滤波器
整理By 梁昀轲 2018211769
*/
#include "filter/kalman.h"

double KalmanFilter(Kalman_Typedef *klm, double input)
{
    //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
    klm->Now_P = klm->LastP + klm->Q;
    //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
    klm->Kg = klm->Now_P / (klm->Now_P + klm->R);
    //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
    klm->out = klm->out + klm->Kg * (input -klm->out);//因为这一次的预测值就是上一次的输出值
    //更新协方差方程: 本次的系统协方差赋给 klm->LastP 为下一次运算准备。
    klm->LastP = (1-klm->Kg) * klm->Now_P;
	
	return (klm->out);
}

void Kalman_Init(Kalman_Typedef *klm, const double klm_Q, const double klm_R)//温度klm_Q=0.01 klm_R=0.25
{
	klm->LastP=0.02;		//上次估算协方差
	klm->Now_P=0;			//当前估算协方差
	klm->out=1;				//卡尔曼滤波器输出
	klm->Kg=0;				//卡尔曼增益
	klm->Q=klm_Q;			//Q:过程噪声协方差 Q参数调滤波后的曲线平滑程度，Q越小越平滑;
	klm->R=klm_R;			//R:观测噪声协方差 R参数调整滤波后的曲线与实测曲线的相近程度，R越小越接近(收敛越快)
}