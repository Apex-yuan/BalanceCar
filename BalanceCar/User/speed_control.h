/**
  ******************************************************************************
  * @file    speed_control.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   速度控制相关的代码
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPEED_CONTROL_H
#define __SPEED_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "pid.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
extern float speedP;
extern float speedI;
extern float speedD;
extern float g_fCarSpeed;
extern float g_fSpeedControlOut;
extern float g_fSpeedControlIntegral;
extern PID_t speedPID;
/* Exported functions ------------------------------------------------------- */
void SpeedControl(void);


#ifdef __cplusplus
}
#endif

#endif /* __SPEED_CONTROL_H */

