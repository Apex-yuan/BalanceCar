/**
  ******************************************************************************
  * @file    angle_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   ƽ�⳵ֱ��������صĴ���
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "angle_control.h"
#include "virtual_oscilloscope.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float g_fCarAngle;
float g_fGyroscopeAngleSpeed;
float g_fAngleControlOut;
// short g_nGyro[3], g_nAccel[3];
// float g_fPitch, g_fRoll, g_fYaw;

float ANGLE_CONTROL_P = 64;//64;//80;//90
float ANGLE_CONTROL_D = 0.12;//0.12;//0.15;//0.2
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ֱ�������㷨�������ж�������ÿ5msִ��һ�Σ�
  * @param  None
  * @retval None
  */
void AngleControl(void)
{
  float fValue;

  /* ��ȡ���rad�ͽ��ٶ�rad/s */
	g_fCarAngle = imu_data.rpy[0]; //g_fRoll;
  g_fGyroscopeAngleSpeed = imu_data.gyro[0]; //(float)g_nGyro[0];
  
	fValue = (g_fCarAngle - CAR_ANGLE_SET) * ANGLE_CONTROL_P + 
           (g_fGyroscopeAngleSpeed - CAR_ANGLE_SPEED_SET) * ANGLE_CONTROL_D;
	g_fAngleControlOut = fValue;
  
  //����ʾ����
  g_fware[0] = g_fCarAngle;
  g_fware[1] = g_fGyroscopeAngleSpeed;
  g_fware[2] = g_fAngleControlOut;
}




