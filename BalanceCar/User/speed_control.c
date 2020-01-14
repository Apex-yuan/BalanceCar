/**
  ******************************************************************************
  * @file    speed_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   �ٶȿ�����صĴ���
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "speed_control.h"
#include "encoder.h"
#include "virtual_oscilloscope.h"
#include "protocol.h"
#include "config.h"
#include "pid.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float speedP = 0;
float speedI = 0;
float speedD = 0;
PID_t speedPID;
float g_fCarSpeed;
float g_fActualTargetSpeed = 0.0;
float g_fTargetSpeed = 0;
float g_fSpeedControlOutOld;
float g_fSpeedControlIntegral;
float g_fSpeedControlOut;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ������������ڵ����ƽ���ٶ�
  * @param  None
  * @retval None
  */
//  int32_t nLeftEncoderPulse, nRightEncoderPulse;
static float getCarSpeed(void)
{
  int32_t nLeftEncoderPulse, nRightEncoderPulse;
  static float fSpeed;
  float tmp;
  
  nLeftEncoderPulse = encoder_getPulse(LEFT_ENCODER);
  nRightEncoderPulse = encoder_getPulse(RIGHT_ENCODER);
  encoder_resetPulse();
  tmp = (nLeftEncoderPulse + nRightEncoderPulse) / 2.0;
  fSpeed *= 0.8;
  fSpeed += tmp*0.2;
  return (fSpeed * CAR_SPEED_CONSTANT); //�ٶȵ�λת��Ϊ��ת/��
}


/**
  * @brief  �ٶȿ��ƺ���
  * @param  None
  * @retval None
  */
void SpeedControl(void)
{
  float fDelta;
  float fP,fI;
  
  g_fTargetSpeed = g_fBTSpeedSet; //��ȡĿ���ٶ�
  g_fActualTargetSpeed += constrain(g_fTargetSpeed - g_fActualTargetSpeed, -SPEED_ACCELERATION, SPEED_ACCELERATION); //�ٶ�ƽ�����
  
  g_fCarSpeed = getCarSpeed(); //��ȡ��ģ�ٶȣ�ת/��
   
  fDelta = g_fActualTargetSpeed - g_fCarSpeed;
//  
//  fP = fDelta * speedP;
//  fI = fDelta * speedI;
//  
//  g_fSpeedControlIntegral += fI;
//  
//  /* �����޷� */
////  g_fSpeedControlIntegral = constrain(g_fSpeedControlIntegral, -SPEED_I_LIMIT, SPEED_I_LIMIT);
//  
//  g_fSpeedControlOutOld = g_fSpeedControlOut;
//  g_fSpeedControlOut = fP + g_fSpeedControlIntegral;
  g_fSpeedControlOut = pid_update(&speedPID,g_fActualTargetSpeed,g_fCarSpeed,DT);
  
  /* ����ʾ���� */
  g_fware[3] = g_fCarSpeed * 100;
  g_fware[4] = g_fSpeedControlOut;

}


