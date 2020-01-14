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
#include "config.h"
#include "angle_control.h"
#include "speed_control.h"
#include "direction_control.h"
#include "motor_control.h"
#include "protocol.h"
#include "virtual_oscilloscope.h"
#include "pid.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//#define CONTROL_PERIOD 5 //5ms
//#define SPEED_CONTROL_COUNT 20 //20*5=100ms
//#define DIRECTION_CONTROL_COUNT 2 //2*5=10ms
/* Private variables ---------------------------------------------------------*/
float g_n1MsEventCount = 0;
float g_nSpeedControlCount = 0;
uint32_t tTime[5];

float targetSpeed = 0;          // used to control the robot, drive and turn, unit: wheel revulutions per second
float turningSpeed = 0; 
float actualTargetSpeed = 0; 
float actualTurningSpeed = 0; 
float motorSpeed = 0; //
//float ddt;

/* 系统在刚初始化完成时，数据输出不是很稳定，因而需要一定的延时时间，等待数据稳定再让电机输出。
   这里主要是为了服务于初始状态下的弹射起步，若没有等待到数据稳定，弹射起步便无法正常触发。
*/
uint32_t g_ndelayDeparturecount = 0;
uint32_t g_ndelayDeparturetime = 2000;  //单位ms  这里的时间是指初始化完成后的延时时间
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  parameter init
  * @param  None
  * @retval None
  */
void parameter_init(void)
{
  /*angle pid*/
  pid_init(&anglePID,ANGLE_P,ANGLE_I,ANGLE_D,0,FROM_MEASURED_VALUE);
//  ddt = DT;
  angleP = ANGLE_P;
  angleI = ANGLE_I;
  angleD = ANGLE_D;
  /* speed pid */
  pid_init(&speedPID,SPEED_P, SPEED_I, SPEED_D, SPEED_I_LIMIT, FROM_ERROR);
  speedP = SPEED_P;
  speedI = SPEED_I;
  speedD = SPEED_D;
  /* turn pid */
  turnP = TURN_P;
  turnI = TURN_I;
  turnD = TURN_D;
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t tmp;
  parameter_init();
  bsp_init(); //初始化板级支持包
  tmp = millis();
  for(uint8_t i = 0; i < 5; ++i)
  {
    tTime[i] = tmp;
  }
  
  while(1)
  {
    /* 以20hz的频率闪烁LED0 */
    if((millis() - tTime[0] >= 1000/20) && (g_ndelayDeparturecount >= g_ndelayDeparturetime))
    {
      led_toggle(LED0);
      tTime[0] = millis();
    }
    /* 在车模启动之后,以10hz的频率向虚拟示波器发送数据 */
    if(millis() - tTime[1] >= 1000/20)
    {
      vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
      tTime[1] = millis();
    }
    /* 处理接收到的上位机数据 */
    protocol_process();
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
    g_ndelayDeparturecount++; //用于延迟发车计数
    
    if(g_n1MsEventCount >= CONTROL_PERIOD)
    {
      g_n1MsEventCount = 0;
      //GetMotorPulse();
    }
    else if(g_n1MsEventCount == 1)
    {
//      MPU_DMP_ReadData(&imu_data); //数据一定要及时读出否则会卡死（即该函数在mpu初始化完成后一定要频繁执行）
    }
    else if(g_n1MsEventCount == 2)
    {
      MPU_DMP_ReadData(&imu_data); //数据一定要及时读出否则会卡死（即该函数在mpu初始化完成后一定要频繁执行）
      SpeedControl();
      AngleControl();
      if(g_ndelayDeparturecount >= g_ndelayDeparturetime) //到达发车时间
      {
        MotorOutput();
        g_ndelayDeparturecount = g_ndelayDeparturetime + 1; //防止计数溢出
      }
    }

    
  }
}
