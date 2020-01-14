/**
  ******************************************************************************
  * @file    protocol.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   �����ֻ��ͻ��˿��Ƶ�����Э�鼰����Ŀǰֻʵ����Э���ֻ�APP���·�����
  ******************************************************************************
  * @attention
  * ĿǰЭ�鴦���ֽ������򵥵Ŀ���ָ���������๦����������ӹ���
  * Ŀǰֻʵ����ָ����·����ƣ�δʵ�����ݵ��ϱ�
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "protocol.h"
#include "angle_control.h"
#include "speed_control.h"
#include "direction_control.h"
#include <string.h>
#include <stdlib.h>
#include "serial.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BUFFER_SIZE 4
#define MAX_PACK_SIZE 80
#define MAX_CMD_SIZE 128
/* Private variables ---------------------------------------------------------*/
bool g_bStartBitFlag = 0; //���յ���ʼ�ַ��ı�־λ
char cmdBuffer[BUFFER_SIZE][MAX_CMD_SIZE];
 int bufindr = 0;
 int bufindw = 0;
 int buflen  = 0;
int serial_count = 0;
static char serial_char;
static bool comment_mode = false;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

/*����������ر�������*/
float g_fBTSpeedSet = 0.0;
float g_fBTDirectionSet = 0.0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
 
/**
  * @brief  ��ȡ����
  * @param  none
  * @retval none
  */
void getCommand(void)
{
  while(serial_available() > 0 && buflen < BUFFER_SIZE)
  {
    serial_char = serial_read(); //�ӻ������ж�ȡ�ַ�
    if(serial_char == '\n' ||
       serial_char == '\r' ||
       (serial_char == ':' && comment_mode == false) ||
       serial_count >= (MAX_CMD_SIZE - 1))
    {
      if(!serial_count)
      {
        comment_mode = false;
        return;
      }
      cmdBuffer[bufindw][serial_count] = '\0'; //terminate string
      if(!comment_mode)
      {
        comment_mode = false;
        bufindw = (bufindw + 1) % BUFFER_SIZE;
        buflen += 1;
      }
      serial_count = 0; //reset buffer
    }
    else
    {
      if(serial_char == ';') comment_mode = true;
      if(!comment_mode) cmdBuffer[bufindw][serial_count++] = serial_char;
    }
  }
}
 
float code_value(void)  //��strchr_pointerָ�����ַ����е����ְ�ʮ������ת��Ϊ��ֵ
{
  return (strtod(&cmdBuffer[bufindr][strchr_pointer - cmdBuffer[bufindr] + 1], NULL));
}

long code_value_long(void)  //��strchr_pointerָ�����ַ����е����ְ�ʮ������ת��Ϊ˫������ֵ
{
  return (strtol(&cmdBuffer[bufindr][strchr_pointer - cmdBuffer[bufindr] + 1], NULL, 10));
}

bool code_seen(char code)  //����ַ���code�����ҵ�������ֵΪtrue
{
  strchr_pointer = strchr(cmdBuffer[bufindr], code);
  return (strchr_pointer != NULL);  //Return True if a character was found
}

void processCommand(void)
{
  if(code_seen('C')) //��ȡ���ַ�C
  {
    switch((int)code_value()) //��ȡ�ַ�C�������
    {
      case 0:
      case 1: //�����˶�
      {
        if(code_seen('X'))
        {
          g_fBTSpeedSet = code_value(); //��X�����������Ϊ�ٶȵ��趨ֵ
        }
        break;
      }
      case 2: //���ýǶ�PID
      {
        if(code_seen('P')) anglePID._kp = code_value();
        if(code_seen('I')) anglePID._ki = code_value();
        if(code_seen('D')) anglePID._kd = code_value();
        break;
      }
      case 3: //�����ٶ�PID
      {
        if(code_seen('P')) speedPID._kp = code_value();
        if(code_seen('I')) speedPID._ki = code_value();
        if(code_seen('D')) speedPID._kd = code_value();
        break;
      }
      case 4: //����ת��PID
      {
        if(code_seen('P')) turnP = code_value();
        if(code_seen('I')) turnI = code_value();
        if(code_seen('D')) turnD = code_value();
        break;
      }
      case 5: //��ѯPIDֵ
      {
        LOG_INFO("angle pid: p=%.2f i=%.2f d=%.2f\n", angleP, angleI, angleD);
        LOG_INFO("speed pid: p=%.2f i=%.2f d=%.2f\n", speedP, speedI, speedD);
        LOG_INFO("turn pid: p=%.2f i=%.2f d=%.2f\n", turnP, turnI, turnD);
        break;
      }
    }
  }
  else //δ֪����
  {
    LOG_INFO("unknowned command:\"%s\"\n",cmdBuffer[bufindr]);
  }
}


/**
  * @brief  Э��������
  * @param  none
  * @retval none
  */ 
void protocol_process(void)
{
  if(buflen < (BUFFER_SIZE-1)) { 
			 getCommand();
  }
  if(buflen)
  {
    processCommand();
//    usart3_printf("4555662133s\n");
//    usart3_printf("%s",cmdBuffer[bufindr]);
    buflen = (buflen-1);
    bufindr = (bufindr + 1)%BUFFER_SIZE;
  }
}

//void protocol_process(void)
//{
//  if(buflen)
//  {
//    switch(cmdBuffer[bufindr][1])
//    {
//      case '0':
//        g_fBTSpeedSet = 0;
//        g_fBTDirectionSet = 0; 
//        break;
//      case '1':
//        g_fBTSpeedSet = 1.0;
//        g_fBTDirectionSet = 0;
//        break;
//      case '2':   
//        g_fBTSpeedSet = (-1.0);
//        g_fBTDirectionSet = 0;
//        break;
//      case '3':
//        g_fBTSpeedSet = 0;
//        g_fBTDirectionSet = 1.0;
//        break;
//      case '4':
//        g_fBTSpeedSet = 0;
//        g_fBTDirectionSet = (-1.0);
//        break;
//      default:
//        g_fBTSpeedSet = 0;
//        g_fBTDirectionSet = 0;
//      break;
//    }
//    buflen -= 1;
//    bufindr = (bufindr + 1) % BUFFER_SIZE;
//  }
//}

///**
//  * @brief  USART3�����жϷ�����
//  * @param  none
//  * @retval none
//  */ 
//void usart3_irq(void)
//{
//	uint8_t rec;
//  
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
//	{
//		rec = USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
//    if(rec == '$')
//    {
//      g_bStartBitFlag = 1;
//      serial_count = 0;
//    }
//    if(g_bStartBitFlag == 1)
//    {
//      cmdBuffer[bufindw][serial_count++] = rec;
//    }
//    if(g_bStartBitFlag == 1 && rec == '#')
//    {
//      g_bStartBitFlag = 0;
//      bufindw = (bufindw + 1) % BUFFER_SIZE;
//      buflen += 1;
//    }
//    if(serial_count >= 80)
//    {
//      g_bStartBitFlag = 0;
//      serial_count = 0;
//    }
//  } 
//} 

