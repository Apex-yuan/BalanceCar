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

/* ������ */
static TaskHandle_t startTaskHandle = NULL;
static TaskHandle_t ledTaskHandle = NULL;
static TaskHandle_t speedControlTaskHandle = NULL;
static TaskHandle_t directionControlTaskHandle = NULL;
static TaskHandle_t controlTaskHandle = NULL;
static TaskHandle_t usart3RxTaskHandle = NULL;
static TaskHandle_t imuTaskHandle = NULL;

/*���о��*/
QueueHandle_t usart3RxQueue;
QueueHandle_t imuDataQueue;

/* ϵͳ�ڸճ�ʼ�����ʱ������������Ǻ��ȶ��������Ҫһ������ʱʱ�䣬�ȴ������ȶ����õ�������
   ������Ҫ��Ϊ�˷����ڳ�ʼ״̬�µĵ����𲽣���û�еȴ��������ȶ��������𲽱��޷�����������
*/
uint32_t g_ndelayDeparturecount = 0;
uint32_t g_ndelayDeparturetime = 2000;  //��λms  �����ʱ����ָ��ʼ����ɺ����ʱʱ��
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
  
  bsp_init(); //��ʼ���弶֧�ְ�

  /* ������������ */
  xReturn = xTaskCreate((TaskFunction_t)startTask,
                        (const char *)"startTask",
                        (uint16_t)512,
                        (void *)NULL,
                        (UBaseType_t)1,
                        (TaskHandle_t *)&startTaskHandle);
  if(xReturn != pdFALSE)
  {
    vTaskStartScheduler(); //�����������
  }
  while(1); //���򲻻�ִ�е��˴�
}

/**
  * @brief  �����������ڴ���������������
  * @param  parameter
  * @retval None
  */
static void startTask(void* parameter)
{
  BaseType_t xReturn = pdPASS;
  
  taskENTER_CRITICAL();  //�����ٽ���

  /* led ���� */
  xReturn = xTaskCreate((TaskFunction_t)ledTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )0,
                        (TaskHandle_t*  )&ledTaskHandle);
  if(pdPASS == xReturn)
    printf("ledTask�����ɹ���\r\n");
  
  /* speedControlTask  */
  xReturn = xTaskCreate((TaskFunction_t)speedControlTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&speedControlTaskHandle);
  if(pdPASS == xReturn)
    printf("speedControlTask�����ɹ���\r\n");
  
  /* directionControlTask */
  xReturn = xTaskCreate((TaskFunction_t)directionControlTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&directionControlTaskHandle);
  if(pdPASS == xReturn)
    printf("directionControlTask�����ɹ���\r\n");
  
  /* controlTask */
  xReturn = xTaskCreate((TaskFunction_t)controlTask,
                        (const char*   )"controlTask",
                        (uint16_t      )512,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&controlTaskHandle);
  if(pdPASS == xReturn)
    printf("controlTask�����ɹ���\r\n");
  
  /* usart3RxTask */
  xReturn = xTaskCreate((TaskFunction_t)usart3RxTask,
                        (const char*   )"uart3RxTask",
                        (uint16_t      )512,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&usart3RxTaskHandle);
  if(pdPASS == xReturn)
    printf("usart3RxTask�����ɹ���\r\n");
  
  /* imuTask */
  xReturn = xTaskCreate((TaskFunction_t)imuTask,
                        (const char*   )"imuTask",
                        (uint16_t      )512,
                        (void*         )NULL,
                        (UBaseType_t   )3,
                        (TaskHandle_t*  )&imuTaskHandle);
  if(pdPASS == xReturn)
    printf("imuTask�����ɹ���\r\n");
  
  vTaskDelete(startTaskHandle); //ɾ����������
  taskEXIT_CRITICAL(); //�˳��ٽ���
}

/* ledTask ָʾ��������������*/
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

/* ��������������ڣ�5ms */
static void controlTask(void *parameter)
{ 
  static portTickType xLastWakeTime;
  const portTickType xFrequency = 5;
  
  while(1)
  {
    vTaskDelayUntil(&xLastWakeTime, xFrequency); //�ȴ���һ������
    /* ����Ϊ������ִ�еĴ��� */
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
    /* �����ӳٵ��䷢������Ҫһ����ʱ��ȴ���ģ�����ȶ��� */
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
  const portTickType xPeriod = 10; //��λ��ϵͳ����tick
  // IMU_Data imuData;
  
  imuDataQueue = xQueueCreate((UBaseType_t)1, (UBaseType_t)sizeof(IMU_Data));
  
  xLastWakeTime = xTaskGetTickCount(); //��ȡ��ǰϵͳʱ��
  
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, xPeriod); //�ȴ���һ������
    /* ����Ϊ��Ҫ����ִ�еĴ��� */
    MPU_DMP_ReadData(&imu_data);
//    MPU_DMP_ReadData(&imuData);
//    xReturn = xQueueOverwrite(imuDataQueue, &imuData);//xQueueSend(imuDataQueue, &imuData, 0); //��imu��Ϣ���͵���Ϣ������
//    usart3_printf("%d\r\n",xReturn);
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
    /* �жϷ������� */
  }
}
