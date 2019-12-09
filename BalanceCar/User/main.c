#include "usart1.h"
#include "usart3.h"
#include "delay.h"
#include "encoder.h"
#include "motor.h"
#include "i2c.h"
#include "mpu6050.h"
#include "tim.h"

#include "angle_control.h"
#include "speed_control.h"
#include "direction_control.h"
#include "motor_control.h"
#include "protocol.h"

#define CONTROL_PERIOD 5 //5ms
#define SPEED_CONTROL_COUNT 20 //20*5=100ms
#define DIRECTION_CONTROL_COUNT 2 //2*5=10ms


float g_n1MsEventCount = 0;
float g_nSpeedControlCount = 0;



int main(void)
{
  delay_init();
  usart1_init(115200);
  usart3_init(9600);
  
  TIM1_TIMER_Init(999,71);
  MotorInit();
  encoder_init();
  while(MPU_DMP_Init());
  
  g_fAngleControlOut = 0;
  g_fCarAngle = 0;
  g_fGyroscopeAngleSpeed = 0;
  g_fLeftMotorOut = g_fRightMotorOut = 0;
  delay_ms(1000);
  //delay_ms(1000);
  //delay_ms(1000);
  delay_ms(1000);
  
  while(1)
  {
    //Send_Data(accel,gyro);
    //vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
    //delay_ms(10);

  // if(g_bNewLineFlag == 1)
  // {
  //   g_bNewLineFlag = 0;
  //   ProtocolCpyData();
  //   Protocol();
  // }
  protocol_process();

  }
}


//中断服务函数
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
      MPU_DMP_GetData(g_nGyro, g_nAccel, &g_fPitch, &g_fRoll, &g_fYaw);
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
