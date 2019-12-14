/**
  ******************************************************************************
  * @file    motor_control.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   电机控制输出
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "motor.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//motor setting
#define LEFT_MOTOR_OUT_DEAD_ZONE   0//25//20//35
#define RIGHT_MOTOR_OUT_DEAD_ZONE  0//25//20//30
#define MAX_MOTOR_OUT        1000
#define MIN_MOTOR_OUT       -1000
   
//宏函数
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>=(high)?(high):(amt)))

/* Exported variables ------------------------------------------------------- */
extern float g_fLeftMotorOut;
extern float g_fRightMotorOut;
/* Exported functions ------------------------------------------------------- */
void MotorOutput(void);


#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_CONTROL_H */

