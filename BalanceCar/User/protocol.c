/**
  ******************************************************************************
  * @file    protocol.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   基于手机客户端控制的蓝牙协议及处理，目前只实现了协议手机APP的下发控制
  ******************************************************************************
  * @attention
  * 目前协议处理部分仅包含简单的控制指令处理，如需更多功能需继续增加功能
  * 目前只实现了指令的下发控制，未实现数据的上报
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
bool g_bStartBitFlag = 0; //接收到起始字符的标志位
static char cmdBuffer[BUFFER_SIZE][MAX_PACK_SIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen  = 0;
int serial_count = 0;

/*蓝牙控制相关变量定义*/
float g_fBTSpeedSet = 0.0;
float g_fBTDirectionSet = 0.0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
 
/**
  * @brief  协议解包处理
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
  * @brief  USART3串口中断服务函数
  * @param  none
  * @retval none
  */ 
void usart3_irq(void)
{
	uint8_t rec;
  
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		rec = USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
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

