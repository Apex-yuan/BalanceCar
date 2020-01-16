/**
  ******************************************************************************
  * @file    speed_control.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   �ٶȿ�����صĴ���
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
//typedef struct
//{
//  int16_t counter; //������������
//  float speed; //��λ��ת/��
//}EncoderMotor_t;
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
extern float g_fActualTargetSpeed;
extern float g_fTargetSpeed;
/* Exported functions ------------------------------------------------------- */
void SpeedControl(void);


#ifdef __cplusplus
}
#endif

#endif /* __SPEED_CONTROL_H */

