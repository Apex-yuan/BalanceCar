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

float ANGLE_CONTROL_P = 45;//36;//67.5;
float ANGLE_CONTROL_D = 2.0;//1.8;//3.0;

/*��ģ������־λ*/
bool g_bFallFlag = 0;
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
	g_fCarAngle = RAD2DEG(imu_data.rpy[0]); //g_fRoll;
  g_fGyroscopeAngleSpeed = RAD2DEG(imu_data.gyro[0]); //(float)g_nGyro[0];
  
	fValue = (g_fCarAngle - CAR_ANGLE_SET) * ANGLE_CONTROL_P + 
           (g_fGyroscopeAngleSpeed - CAR_ANGLE_SPEED_SET) * ANGLE_CONTROL_D;
	g_fAngleControlOut = fValue;
  
//  //�������
  if(g_fCarAngle > 50 || g_fCarAngle < (-50))
  {
    g_bFallFlag = 1;
  }
  // else
  // {
  //   g_bFallFlag = 0;
  // }

  //����ʾ����
  g_fware[0] = g_fCarAngle;
  g_fware[1] = g_fGyroscopeAngleSpeed;
  g_fware[2] = g_fAngleControlOut;
}




