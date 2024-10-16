#include "servo.h"

void servo_setAngle(uint16_t Angle)
{
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,((Angle+45)*80)/3);
}



