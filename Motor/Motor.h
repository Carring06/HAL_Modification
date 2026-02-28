#ifndef MOTOR_H_
#define MOTOR_H_

#include ""stm32f1xx_hal.h" "
#include "tim.h"

void Motor_PWMStart(void);
void MotorA2_SetSpeed(int8_t Speed);
void MotorA1_SetSpeed(int8_t Speed);

#endif /* MOTOR_H_ */
