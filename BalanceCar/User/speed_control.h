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
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define OPTICAL_ENCODE_CONSTANT  (13*4) //�������ֱ���
#define REDUCTION_RATIO          30 //���ٱ�
#define SPEED_CONTROL_PERIOD  100  //�ٶȿ������ڣ�ms��
#define CAR_SPEED_CONSTANT  (1000.0/SPEED_CONTROL_PERIOD/OPTICAL_ENCODE_CONSTANT/REDUCTION_RATIO)  //��λת������ֵ�����ٶȵ�λת��Ϊ ת/�룬��Ӧ����ת�٣�

#define CAR_SPEED_SET 0
#define MOTOR_OUT_DEAD_VAL 0

/* Exported variables ------------------------------------------------------- */
extern float g_fCarSpeed;
extern int g_nLeftMotorPulseSigma, g_nRightMotorPulseSigma;
extern float g_fSpeedControlOutOld, g_fSpeedControlOutNew;
extern float g_fSpeedControlIntegral;
extern float g_fSpeedControlOut;
extern uint16_t g_nSpeedControlPeriod;
/* Exported functions ------------------------------------------------------- */
void GetMotorPulse(void);
void SpeedControl(void);
void SpeedControlOutput(void);


#ifdef __cplusplus
}
#endif

#endif /* __SPEED_CONTROL_H */

