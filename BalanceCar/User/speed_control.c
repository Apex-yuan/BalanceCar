/**
  ******************************************************************************
  * @file    speed_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   速度控制相关的代码
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float g_fCarSpeed;
float g_fActualTargetSpeed = 0.0;
float g_fTargetSpeed = 0;
float g_fSpeedControlOutOld;
float g_fSpeedControlIntegral;
float g_fSpeedControlOut;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  计算调用周期内电机的平均速度
  * @param  None
  * @retval None
  */
static float getCarSpeed(void)
{
  int32_t nLeftEncoderPulse, nRightEncoderPulse;
  float fSpeed;
  
  nLeftEncoderPulse = encoder_getPulse(LEFT_ENCODER);
  nRightEncoderPulse = encoder_getPulse(RIGHT_ENCODER);
  encoder_resetPulse();
  fSpeed = (nLeftEncoderPulse + nRightEncoderPulse) / 2.0;
  return (fSpeed * CAR_SPEED_CONSTANT); //速度单位转化为：转/秒
}


/**
  * @brief  速度控制函数
  * @param  None
  * @retval None
  */
void SpeedControl(void)
{
  float fDelta;
  float fP,fI;
  
  g_fTargetSpeed = g_fBTSpeedSet; //获取目标速度
  g_fActualTargetSpeed += constrain(g_fTargetSpeed - g_fActualTargetSpeed, -SPEED_ACCELERATION, SPEED_ACCELERATION); //速度平滑输出
  
  g_fCarSpeed = getCarSpeed(); //获取车模速度：转/秒
   
  fDelta = g_fActualTargetSpeed - g_fCarSpeed;
  
  fP = fDelta * SPEED_P;
  fI = fDelta * SPEED_I;
  
  g_fSpeedControlIntegral += fI;
  
  /* 积分限幅 */
  g_fSpeedControlIntegral = constrain(g_fSpeedControlIntegral, -SPEED_I_LIMIT, SPEED_I_LIMIT);
  
  g_fSpeedControlOutOld = g_fSpeedControlOut;
  g_fSpeedControlOut = fP + g_fSpeedControlIntegral;
  
  /* 虚拟示波器 */
  g_fware[3] = g_fCarSpeed * 100;
  g_fware[4] = g_fSpeedControlOut;

}


