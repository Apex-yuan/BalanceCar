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
/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static void startTask(void *parameter);
static void ledTask(void *parameter);
// static void speedControlTask(void *parameter);
// static void directionControlTask(void *parameter);
static void controlTask(void *parameter);
// static void usart3RxTask(void *parameter);
static void imuTask(void *parameter);
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
static TaskHandle_t usart3RxTaskHandle = NULL;
static TaskHandle_t imuTaskHandle = NULL;

/*队列句柄*/
QueueHandle_t usart3RxQueue;
QueueHandle_t imuDataQueue;

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
  
  bsp_init(); //初始化板级支持包

  /* 创建启动任务 */
  xReturn = xTaskCreate((TaskFunction_t)startTask,
                        (const char *)"startTask",
                        (uint16_t)512,
                        (void *)NULL,
                        (UBaseType_t)1,
                        (TaskHandle_t *)&startTaskHandle);
  if(xReturn != pdFALSE)
  {
    vTaskStartScheduler(); //开启任务调度
  }
  while(1); //程序不会执行到此处
}

/**
  * @brief  启动任务，用于创建其他调度任务
  * @param  parameter
  * @retval None
  */
static void startTask(void* parameter)
{
  BaseType_t xReturn = pdPASS;
  
  taskENTER_CRITICAL();  //进入临界区

  /* led 任务 */
  xReturn = xTaskCreate((TaskFunction_t)ledTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )0,
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
                        (uint16_t      )512,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&controlTaskHandle);
  if(pdPASS == xReturn)
    printf("controlTask创建成功！\r\n");
  
  /* usart3RxTask */
  xReturn = xTaskCreate((TaskFunction_t)usart3RxTask,
                        (const char*   )"uart3RxTask",
                        (uint16_t      )512,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&usart3RxTaskHandle);
  if(pdPASS == xReturn)
    printf("usart3RxTask创建成功！\r\n");
  
  /* imuTask */
  xReturn = xTaskCreate((TaskFunction_t)imuTask,
                        (const char*   )"imuTask",
                        (uint16_t      )512,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&imuTaskHandle);
  if(pdPASS == xReturn)
    printf("imuTask创建成功！\r\n");
  
  vTaskDelete(startTaskHandle); //删除启动任务
  taskEXIT_CRITICAL(); //退出临界区
}

/* ledTask 指示程序在正常运行*/
static void ledTask(void *parameter)
{
  while(1)
  { 
    led_on(LED0);
    vTaskDelay(500);
    led_off(LED0);
    vTaskDelay(500);
  }
}

/* 控制输出任务，周期：5ms */
static void controlTask(void *parameter)
{ 
  static portTickType xLastWakeTime;
  const portTickType xFrequency = 5;
  
  while(1)
  {
    vTaskDelayUntil(&xLastWakeTime, xFrequency); //等待下一个周期
    /* 以下为周期性执行的代码 */
    g_nSpeedControlPeriod += 5;
    if(g_nSpeedControlPeriod >= 100)
    {
      g_nSpeedControlPeriod = 0;
    }
    SpeedControlOutput();
    
    g_nDirectionControlPeriod +=5;
    if(g_nDirectionControlPeriod >= 10)
    {
      g_nDirectionControlPeriod = 0;
    }
    DirectionControlOutput();
    
    GetMotorPulse();
    AngleControl();
    /* 用于延迟弹射发车（需要一定的时间等待车模数据稳定） */
    if(g_ndelayDeparturecount >= g_ndelayDeparturetime)
    {
      MotorOutput();
    }
    else
    {
      g_ndelayDeparturecount += 5;
    }
    
  }
}



/* imuTask */
void imuTask(void * parameter)
{
  // BaseType_t xReturn;
  static portTickType xLastWakeTime;
  const portTickType xPeriod = 10; //单位：系统节拍tick
  // IMU_Data imuData;
  
  imuDataQueue = xQueueCreate((UBaseType_t)1, (UBaseType_t)sizeof(IMU_Data));
  
  xLastWakeTime = xTaskGetTickCount(); //获取当前系统时间
  
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, xPeriod); //等待下一个周期
    /* 以下为需要周期执行的代码 */
    MPU_DMP_ReadData(&imu_data);
//    MPU_DMP_ReadData(&imuData);
//    xReturn = xQueueOverwrite(imuDataQueue, &imuData);//xQueueSend(imuDataQueue, &imuData, 0); //将imu消息发送到消息队列中
//    usart3_printf("%d\r\n",xReturn);
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
    /* 中断服务内容 */
  }
}
