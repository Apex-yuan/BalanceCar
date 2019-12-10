/**
  ******************************************************************************
  * @file    main.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   main body
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "bsp.h"
#include "angle_control.h"
#include "speed_control.h"
#include "direction_control.h"
#include "motor_control.h"
#include "protocol.h"
#include "virtual_oscilloscope.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CONTROL_PERIOD 5 //5ms
#define SPEED_CONTROL_COUNT 20 //20*5=100ms
#define DIRECTION_CONTROL_COUNT 2 //2*5=10ms
/* Private variables ---------------------------------------------------------*/
float g_n1MsEventCount = 0;
float g_nSpeedControlCount = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  bsp_init();
  
  while(1)
  {
    vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
    delay_ms(10);

  protocol_process();
//     printf("accel_x:%.3f,accel_y:%.3f,accel_z:%.3f\n",imu_data.accel[0],imu_data.accel[1],imu_data.accel[2]);
//     printf("gyro_x:%.3f,gyro_y:%.3f,gyro_z:%.3f\n",imu_data.gyro[0],imu_data.gyro[1],imu_data.gyro[2]);
//     printf("roll:%.3f,pitch:%.3f,yow:%.3f\n\n",imu_data.rpy[0],imu_data.rpy[1],imu_data.rpy[2]);
// delay_ms(1000);
  }
}

/**
  * @brief  定时器1的中断服务函数
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void) 
{
  if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)
  {  
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    //中断服务程序：
    g_n1MsEventCount ++;
    
    g_nSpeedControlPeriod ++;
    SpeedControlOutput();
    g_nDirectionControlPeriod ++;
    DirectionControlOutput();
    
    if(g_n1MsEventCount >= CONTROL_PERIOD)
    {
      g_n1MsEventCount = 0;
      GetMotorPulse();
    }
    else if(g_n1MsEventCount == 1)
    {
      //MPU_DMP_GetData(g_nGyro, g_nAccel, &g_fPitch, &g_fRoll, &g_fYaw);
      MPU_DMP_ReadData(&imu_data);
    }
    else if(g_n1MsEventCount == 2)
    {
      AngleControl();
      MotorOutput();
    }
    else if (g_n1MsEventCount == 3)
    {
      g_nSpeedControlCount ++;
      if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT)
      {
        SpeedControl();
        g_nSpeedControlCount = 0;
        g_nSpeedControlPeriod = 0;
      }
    }
    else if(g_n1MsEventCount == 4)
    {
      g_nDirectionControlCount ++;
      if(g_nDirectionControlCount >= DIRECTION_CONTROL_COUNT)
      {
        DirectionControl();
        g_nDirectionControlCount = 0;
        g_nDirectionControlPeriod = 0;
      }
    }
    
  }
}
