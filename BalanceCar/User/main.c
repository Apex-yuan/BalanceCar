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
static void speedControlTask(void *parameter);
static void directionControlTask(void *parameter);
static void controlTask(void *parameter);
static void usart3RxTask(void *parameter);
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
//int main(void)
//{
//  uint32_t tmp;
//  bsp_init(); //��ʼ���弶֧�ְ�
//  tmp = millis();
//  for(uint8_t i = 0; i < 5; ++i)
//  {
//    tTime[i] = tmp;
//  }
//  
//  while(1)
//  {
//    /* ��20hz��Ƶ����˸LED0 */
//    if((millis() - tTime[0] >= 1000/20) && (g_ndelayDeparturecount >= g_ndelayDeparturetime))
//    {
//      led_toggle(LED0);
//      tTime[0] = millis();
//    }
//    /* �ڳ�ģ����֮��,��10hz��Ƶ��������ʾ������������ */
//    if(millis() - tTime[1] >= 1000/20)
//    {
//      vcan_sendware((uint8_t *)g_fware,sizeof(g_fware));
//      tTime[1] = millis();
//    }
//    /* ������յ�����λ������ */
//    protocol_process();
//  }
//}

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
//  usart3_printf("sdfsdfsdfsdfs\n");

  xReturn = xTaskCreate((TaskFunction_t)startTask,
                        (const char *)"startTask",
                        (uint16_t)512,
                        (void *)NULL,
                        (UBaseType_t)1,
                        (TaskHandle_t *)&startTaskHandle);
  if(xReturn != pdFALSE)
  {
//    printf("�������񴴽��ɹ�\r\n");
    vTaskStartScheduler();
  }
  while(1);
}

static void startTask(void* parameter)
{
  BaseType_t xReturn = pdPASS;
  
  taskENTER_CRITICAL();  //�����ٽ���

  /* led ���� */
  xReturn = xTaskCreate((TaskFunction_t)ledTask,
                        (const char*   )"ledTask",
                        (uint16_t      )128,
                        (void*         )NULL,
                        (UBaseType_t   )2,
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
  
  
  vTaskDelete(startTaskHandle);
  taskEXIT_CRITICAL(); //�˳��ٽ���
}

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

/* �ٶȿ�������������ڣ�100ms */
static void speedControlTask(void *parameter)
{ 
  static portTickType xLastWakeTime;
  const portTickType xFrequency = 100;
  
  xLastWakeTime = xTaskGetTickCount(); //��ȡ��ǰ��ϵͳʱ��
  
  while(1)
  {
    vTaskDelayUntil(&xLastWakeTime, xFrequency); //�ȴ���һ������
    /* ����Ϊ������ִ�еĴ��� */
    SpeedControl();
  }
}

IMU_Data imuData;
/* �����������������ڣ�10ms */
static void directionControlTask(void *parameter)
{
  static portTickType xLastWakeTime;
  const portTickType xFrequency = 10;
  
  xLastWakeTime = xTaskGetTickCount(); //��ȡϵͳ��ǰʱ��
  
  while(1)
  {
    vTaskDelayUntil(&xLastWakeTime, xFrequency); //�ȴ���һ������
    /* ����Ϊ������ִ�еĴ��� */
    DirectionControl();
//    xQueuePeek(imuDataQueue, &imuData, 0);
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
    MotorOutput();
  }
}

/* usart3RxTask */
static void usart3RxTask(void *parameter)
{
  BaseType_t xReturn;
  uint8_t rec;
  usart3_printf("usart3RxTask Start\n");
  usart3RxQueue = xQueueCreate((UBaseType_t) 128,
							                 (UBaseType_t) sizeof(uint8_t));
  for(;;)
  {
    xReturn = xQueueReceive(usart3RxQueue, &rec, portMAX_DELAY);
//    usart3_printf("%c\n",rec);
    if(rec == '$')
    {
      g_bStartBitFlag = 1;
      serial_count = 0;
    }
    if(g_bStartBitFlag == 1)
    {
      cmdBuffer[bufindw][serial_count++] = rec;
    }
    if(g_bStartBitFlag == 1 && rec == '#')
    {
      g_bStartBitFlag = 0;
      bufindw = (bufindw + 1) % BUFFER_SIZE;
      buflen += 1;
    }
    if(serial_count >= 80)
    {
      g_bStartBitFlag = 0;
      serial_count = 0;
    }
  }
}

void imuTask(void * parameter)
{
  BaseType_t xReturn;
  static portTickType xLastWakeTime;
  const portTickType xFrequency = 10;
  IMU_Data imuData;
  
  imuDataQueue = xQueueCreate((UBaseType_t)1, (UBaseType_t)sizeof(IMU_Data));
  
  xLastWakeTime = xTaskGetTickCount(); //��ȡ��ǰϵͳʱ��
  
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, xFrequency); //�ȴ���һ������
    /* ����Ϊ��Ҫ����ִ�еĴ��� */
    MPU_DMP_ReadData(&imu_data);
//    MPU_DMP_ReadData(&imuData);
//    xReturn = xQueueOverwrite(imuDataQueue, &imuData);//xQueueSend(imuDataQueue, &imuData, 0); //��imu��Ϣ���͵���Ϣ������
//    usart3_printf("%d\r\n",xReturn);
  }
}

////ÿ10��ϵͳ����ִ��һ�� 
//void vTaskFunction( void * pvParameters ) 
//{     
//  static portTickType xLastWakeTime;     
//  const portTickType xFrequency = 10;      // ʹ�õ�ǰʱ���ʼ������xLastWakeTime     
//  xLastWakeTime = xTaskGetTickCount();      
//  for( ;; )     
//  {         //�ȴ���һ������         
//    vTaskDelayUntil( &xLastWakeTime,xFrequency );          
//    // ��Ҫ������ִ�д����������     
//  } 
//}


/**
  * @brief  ��ʱ��1���жϷ�����
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void) 
{
  if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)
  {
//    BaseType_t xReturn;
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//    //�жϷ������
//    g_n1MsEventCount ++;
//    g_ndelayDeparturecount++; //�����ӳٷ�������
//    
////    g_nSpeedControlPeriod ++;
////    SpeedControlOutput();
////    g_nDirectionControlPeriod ++; 
////    DirectionControlOutput();
//    
//    if(g_n1MsEventCount >= CONTROL_PERIOD)
//    {
//      g_n1MsEventCount = 0;
////      GetMotorPulse();
//    }
//    else if(g_n1MsEventCount == 1)
//    {
////      MPU_DMP_ReadData(&imu_data); //����һ��Ҫ��ʱ��������Ῠ�������ú�����mpu��ʼ����ɺ�һ��ҪƵ��ִ�У�
//    }
//    else if(g_n1MsEventCount == 2)
//    {
////      xReturn = xTaskNotifyGive(controlTaskHandle);  //�ô�ʹ������֪ͨ�ᵼ��С�����ߣ���֪��ԭ��
////      AngleControl();
////      if(g_ndelayDeparturecount >= g_ndelayDeparturetime) //���﷢��ʱ��
////      {
////        MotorOutput();
////        g_ndelayDeparturecount = g_ndelayDeparturetime + 1; //��ֹ�������
////      }
//    }
//    else if (g_n1MsEventCount == 3)
//    {
//      g_nSpeedControlCount ++;
//      if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT)
//      {
//        /* ��speedControlTask��������֪ͨ */
////      xReturn = xTaskNotifyGive(speedControlTaskHandle);
////        SpeedControl();
//        g_nSpeedControlCount = 0;
//        g_nSpeedControlPeriod = 0;
//      }
//    }
//    else if(g_n1MsEventCount == 4)
//    {
//      g_nDirectionControlCount ++;
//      if(g_nDirectionControlCount >= DIRECTION_CONTROL_COUNT)
//      {
//        /* ��directionControlTask��������֪ͨ */
////        xReturn = xTaskNotifyGive(directionControlTaskHandle);
////        DirectionControl();
//        g_nDirectionControlCount = 0;
//        g_nDirectionControlPeriod = 0;
//      }
//    }
    
  }
}
