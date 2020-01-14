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

/* ϵͳ�ڸճ�ʼ�����ʱ������������Ǻ��ȶ��������Ҫһ������ʱʱ�䣬�ȴ������ȶ����õ�������
   ������Ҫ��Ϊ�˷����ڳ�ʼ״̬�µĵ����𲽣���û�еȴ��������ȶ��������𲽱��޷�����������
*/
uint32_t g_ndelayDeparturecount = 0;
uint32_t g_ndelayDeparturetime = 2000;  //��λms  �����ʱ����ָ��ʼ����ɺ����ʱʱ��
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
  bsp_init(); //��ʼ���弶֧�ְ�
  tmp = millis();
  for(uint8_t i = 0; i < 5; ++i)
  {
    tTime[i] = tmp;
  }
  
  while(1)
  {
    /* ��20hz��Ƶ����˸LED0 */
    if((millis() - tTime[0] >= 1000/20) && (g_ndelayDeparturecount >= g_ndelayDeparturetime))
    {
      led_toggle(LED0);
      tTime[0] = millis();
    }
    /* �ڳ�ģ����֮��,��10hz��Ƶ��������ʾ������������ */
    if(millis() - tTime[1] >= 1000/20)
    {
      vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
      tTime[1] = millis();
    }
    /* ������յ�����λ������ */
    protocol_process();
  }
}

/**
  * @brief  ��ʱ��1���жϷ�����
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void) 
{
  if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)
  {  
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    //�жϷ������
    g_n1MsEventCount ++;
    g_ndelayDeparturecount++; //�����ӳٷ�������
    
    if(g_n1MsEventCount >= CONTROL_PERIOD)
    {
      g_n1MsEventCount = 0;
      //GetMotorPulse();
    }
    else if(g_n1MsEventCount == 1)
    {
//      MPU_DMP_ReadData(&imu_data); //����һ��Ҫ��ʱ��������Ῠ�������ú�����mpu��ʼ����ɺ�һ��ҪƵ��ִ�У�
    }
    else if(g_n1MsEventCount == 2)
    {
      MPU_DMP_ReadData(&imu_data); //����һ��Ҫ��ʱ��������Ῠ�������ú�����mpu��ʼ����ɺ�һ��ҪƵ��ִ�У�
      SpeedControl();
      AngleControl();
      if(g_ndelayDeparturecount >= g_ndelayDeparturetime) //���﷢��ʱ��
      {
        MotorOutput();
        g_ndelayDeparturecount = g_ndelayDeparturetime + 1; //��ֹ�������
      }
    }

    
  }
}
