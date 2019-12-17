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
//#include "led.h"
/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static void startTask(void *parameter);
static void ledTask(void *parameter);
static void speedControlTask(void *parameter);
static void directionControlTask(void *parameter);
static void controlTask(void *parameter);
/* Private macro -------------------------------------------------------------*/
#define CONTROL_PERIOD 5 //5ms
#define SPEED_CONTROL_COUNT 20 //20*5=100ms
#define DIRECTION_CONTROL_COUNT 2 //2*5=10ms
/* Private variables ---------------------------------------------------------*/
float g_n1MsEventCount = 0;
float g_nSpeedControlCount = 0;
uint32_t tTime[5];

/* 任务句柄 */
static TaskHandle_t startTaskHandle = NULL;
static TaskHandle_t ledTaskHandle = NULL;
static TaskHandle_t speedControlTaskHandle = NULL;
static TaskHandle_t directionControlTaskHandle = NULL;
static TaskHandle_t controlTaskHandle = NULL;

/* 系统在刚初始化完成时，数据输出不是很稳定，因而需要一定的延时时间，等待数据稳定再让电机输出。
   这里主要是为了服务于初始状态下的弹射起步，若没有等待到数据稳定，弹射起步便无法正常触发。
*/
uint32_t g_ndelayDeparturecount = 0;
uint32_t g_ndelayDeparturetime = 2000;  //单位ms  这里的时间是指初始化完成后的延时时间
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  BaseType_t xReturn = pdTRUE;
  
  bsp_init();

//  while(1)
//  {
//    MPU_DMP_ReadData(&imu_data);
//    g_fware[3] = imu_data.gyro[0] * 10000;
//    vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
//    delay_ms(10);
//  }

//  xReturn = xTaskCreate((TaskFunction_t)startTask,
//                        (const char *)"startTask",
//                        (uint16_t)512,
//                        (void *)NULL,
//                        (UBaseType_t)1,
//                        (TaskHandle_t *)&startTaskHandle);
//  if(xReturn != pdFALSE)
//  {
////    printf("启动任务创建成功\r\n");
//    vTaskStartScheduler();
//  }
  while(1);
}

static void startTask(void* parameter)
{
  BaseType_t xReturn = pdPASS;
  
  taskENTER_CRITICAL();  //进入临界区

  /* led 任务 */
  xReturn = xTaskCreate((TaskFunction_t)ledTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )2,
                        (TaskHandle_t*  )&ledTaskHandle);
  if(pdPASS == xReturn)
    printf("ledTask创建成功！\r\n");
  
  /* speedControlTask  */
  xReturn = xTaskCreate((TaskFunction_t)speedControlTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&speedControlTaskHandle);
  if(pdPASS == xReturn)
    printf("speedControlTask创建成功！\r\n");
  
  /* directionControlTask */
  xReturn = xTaskCreate((TaskFunction_t)directionControlTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&directionControlTaskHandle);
  if(pdPASS == xReturn)
    printf("directionControlTask创建成功！\r\n");
  
  /* controlTask */
  xReturn = xTaskCreate((TaskFunction_t)controlTask,
                        (const char*   )"controlTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )4,
                        (TaskHandle_t*  )&controlTaskHandle);
  if(pdPASS == xReturn)
    printf("controlTask创建成功！\r\n");
  
  
  vTaskDelete(startTaskHandle);
  taskEXIT_CRITICAL(); //退出临界区
}

static void ledTask(void *parameter)
{
  
  
  while(1)
  {
//    MPU_DMP_ReadData(&imu_data);
    vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
//    MPU_DMP_ReadData(&imu_data);
    vTaskDelay(5);
//    printf("sdfsdf\n");
//    led_on(LED0);
//    
//    vTaskDelay(500);
//    led_off(LED0);
//    printf("sdfvdcx\n");
//    vTaskDelay(500);
  }
}

/* 速度控制输出任务，周期：100ms */
static void speedControlTask(void *parameter)
{
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // 获取任务通知
    SpeedControl();
  }
}

/* 方向控制输出任务，周期：10ms */
static void directionControlTask(void *parameter)
{
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // 获取任务通知
    DirectionControl();
  }
}

/* 控制输出任务，周期：5ms */
static void controlTask(void *parameter)
{ 
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // 获取任务通知
    
    GetMotorPulse();
//    AngleControl();
    MotorOutput();
  }
}


//int main(void)
//{
//  uint32_t tmp;
//  bsp_init(); //初始化板级支持包
//  tmp = millis();
//  for(uint8_t i = 0; i < 5; ++i)
//  {
//    tTime[i] = tmp;
//  }
//  
//  while(1)
//  {
//    /* 以20hz的频率闪烁LED0 */
//    if((millis() - tTime[0] >= 1000/20) && (g_ndelayDeparturecount >= g_ndelayDeparturetime))
//    {
//      led_toggle(LED0);
//      tTime[0] = millis();
//    }
//    /* 在车模启动之后,以10hz的频率向虚拟示波器发送数据 */
//    if(millis() - tTime[1] >= 1000/20)
//    {
//      vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
//      tTime[1] = millis();
//    }
//    /* 处理接收到的上位机数据 */
//    protocol_process();
//  }
//}

/**
  * @brief  定时器1的中断服务函数
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void) 
{
  if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)
  {  
    BaseType_t xReturn = pdPASS;
    
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    //中断服务程序：
    g_n1MsEventCount ++;
    g_ndelayDeparturecount++; //用于延迟发车计数
    
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
      MPU_DMP_ReadData(&imu_data); //数据一定要及时读出否则会卡死（即该函数在mpu初始化完成后一定要频繁执行）
    }
    else if(g_n1MsEventCount == 2)
    {
      AngleControl();
//      xReturn = xTaskNotifyGive(controlTaskHandle);
      MotorOutput();
//      AngleControl();
//      if(g_ndelayDeparturecount >= g_ndelayDeparturetime) //到达发车时间
//      {
//        MotorOutput();
//        g_ndelayDeparturecount = g_ndelayDeparturetime + 1; //防止计数溢出
//      }
    }
    else if (g_n1MsEventCount == 3)
    {
      g_nSpeedControlCount ++;
      if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT)
      {
        SpeedControl();
        /* 向speedControlTask发送任务通知 */
//      xReturn = xTaskNotifyGive(speedControlTaskHandle);
//      if(xReturn != pdFALSE)
//      {
//        //printf("任务通知2发送成功\r\n");
//      }
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
        /* 向directionControlTask发送任务通知 */
//        xReturn = xTaskNotifyGive(directionControlTaskHandle);
        g_nDirectionControlCount = 0;
        g_nDirectionControlPeriod = 0;
      }
    }
    
  }
}
