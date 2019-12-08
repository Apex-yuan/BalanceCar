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
#include "speed_control.h"
#include "direction_control.h"
#include <string.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BUFFER_SIZE 4
#define MAX_PACK_SIZE 80
/* Private variables ---------------------------------------------------------*/
bool g_bStartBitFlag = 0; //���յ���ʼ�ַ��ı�־λ
static char cmdBuffer[BUFFER_SIZE][MAX_PACK_SIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen  = 0;
int serial_count = 0;

/*����������ر�������*/
float g_fBTSpeedSet = 0.0;
float g_fBTDirectionSet = 0.0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
 
/**
  * @brief  Э��������
  * @param  none
  * @retval none
  */ 
void protocol_process(void)
{
  if(buflen)
  {
    switch(cmdBuffer[bufindr][1])
    {
      case '0':
        g_fBTSpeedSet = 0;
        g_fBTDirectionSet = 0; 
        break;
      case '1':
        g_fBTSpeedSet = 1.0;
        g_fBTDirectionSet = 0;
        break;
      case '2':   
        g_fBTSpeedSet = (-1.0);
        g_fBTDirectionSet = 0;
        break;
      case '3':
        g_fBTSpeedSet = 0;
        g_fBTDirectionSet = 1.0;
        break;
      case '4':
        g_fBTSpeedSet = 0;
        g_fBTDirectionSet = (-1.0);
        break;
      default:
        g_fBTSpeedSet = 0;
        g_fBTDirectionSet = 0;
      break;
    }
    buflen -= 1;
    bufindr = (bufindr + 1) % BUFFER_SIZE;
  }
}

/**
  * @brief  USART3�����жϷ�����
  * @param  none
  * @retval none
  */ 
void usart3_irq(void)
{
	uint8_t rec;
  
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		rec = USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
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

