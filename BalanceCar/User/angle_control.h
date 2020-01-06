/**
  ******************************************************************************
  * @file    angle_control.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   平衡车直立控制
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ANGLE_CONTROL_H
#define __ANGLE_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "mpu6050.h"
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//#define CAR_ANGLE_SET   0
//#define CAR_ANGLE_SPEED_SET  0

/* Exported variables ------------------------------------------------------- */
extern float g_fCarAngle;
extern float g_fGyroscopeAngleSpeed;
extern float g_fAngleControlOut;
extern bool g_bFallFlag;
/* Exported functions ------------------------------------------------------- */
void AngleControl(void);


#ifdef __cplusplus
}
#endif

#endif /* __ANGLE_CONTROL_H */


