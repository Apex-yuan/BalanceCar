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
#include "motorControl.h"
#include "protocol.h"
#include "virtual_oscilloscope.h"
#include "pid.h"
#include "lowPassFilter.h"
#include <math.h>
#include "odometry.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

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
PID_t speedPID;
PID_t anglePID;
PID_t turnPID;
uint8_t carState = STOPPING;

Odometry_t odom;

float g_fAngleControlOut;
float g_fSpeedControlOut;
float g_fDirectionControlOut;

float g_fCarAngle;
float g_fCarLinearSpeed;

float g_fTargetLinearSpeed;
float g_fActualTargetLinearSpeed;


int16_t leftEncoderTick,rightEncoderTick;

float g_fTargetAngularSpeed;
float g_fActualTargetAngularSpeed;
float g_fCarAngularSpeed;

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

  /* speed pid */
  pid_init(&speedPID,SPEED_P, SPEED_I, SPEED_D, SPEED_I_LIMIT, FROM_ERROR);

  /* turn pid */
  pid_init(&turnPID, TURN_P, TURN_D, TURN_I,0,FROM_ERROR);
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
      
      /*update data*/
      MPU_DMP_ReadData(&imu_data); //数据一定要及时读出否则会卡死（即该函数在mpu初始化完成后一定要频繁执行）
      g_fCarAngle = RAD2DEG(imu_data.rpy[0]); //车模倾角

      /* 急停 */
      if((carState == RUNNING) && ((g_fCarAngle < -TURN_OFF_ANGLE) || (g_fCarAngle > TURN_OFF_ANGLE)))
      {
        //1.关闭电机输出
        //2.复位PID参数
        //3.将carState清零，为后续运行做准备
        carState = STOPPING;
        motor_setPwm(LEFT_MOTOR, 0);
        motor_setPwm(RIGHT_MOTOR, 0);
        
//        g_fAngleControlOut = 0;
//        g_fSpeedControlOut = 0;
//        g_fDirectionControlOut = 0;
        pid_reset(&anglePID);
        pid_reset(&speedPID);
        pid_reset(&turnPID);
        
      }
      
      /* 达到启动条件 */
      if(carState == STOPPING && (g_fCarAngle > -TURN_ON_ANGLE && g_fCarAngle < TURN_ON_ANGLE))
      {
        //1.将carState置位，标记为当前的车模状态
        carState = RUNNING;
      }
      
      /* 正常输出 */
      if(carState == RUNNING) 
      {
        
        //2.进行直立控制和方向控制计算
        //3.控制电机输出
        
        leftEncoderTick = encoder_getPulse(LEFT_ENCODER);
        rightEncoderTick = encoder_getPulse(RIGHT_ENCODER);
        encoder_resetPulse(); //
        odometryCalc(leftEncoderTick, rightEncoderTick, &odom, DT);

        g_fCarLinearSpeed = odom.linearSpeed; ///0.21363;
        g_fTargetLinearSpeed = -g_fBTSpeedSet; //获取目标速度
        g_fActualTargetLinearSpeed += constrain(g_fTargetLinearSpeed - g_fActualTargetLinearSpeed, -LINEAR_ACCELERATION, LINEAR_ACCELERATION); //线速度平滑输出
        g_fSpeedControlOut = pid_update(&speedPID,g_fActualTargetLinearSpeed,g_fCarLinearSpeed,DT);
        g_fAngleControlOut = pid_update(&anglePID,g_fSpeedControlOut,g_fCarAngle,DT);
        
        g_fCarAngularSpeed = odom.angularSpeed;
        g_fTargetAngularSpeed = g_fBTDirectionSet; //获取目标速度
        g_fActualTargetAngularSpeed += constrain(g_fTargetAngularSpeed - g_fActualTargetAngularSpeed, -ANGULAR_ACCELERATION, ANGULAR_ACCELERATION); //角速度平滑输出
        g_fDirectionControlOut = pid_update(&turnPID,g_fActualTargetAngularSpeed,g_fCarAngularSpeed,DT);

        if(g_ndelayDeparturecount >= g_ndelayDeparturetime) //到达发车时间
        {
          motorOutput(g_fAngleControlOut+g_fDirectionControlOut,g_fAngleControlOut-g_fDirectionControlOut);
          g_ndelayDeparturecount = g_ndelayDeparturetime + 1; //防止计数溢出
        }
      }
          
    }
  }
}
