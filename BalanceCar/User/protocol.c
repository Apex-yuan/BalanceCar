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
bool g_bStartBitFlag = 0; //接收到起始字符的标志位
char cmdBuffer[BUFFER_SIZE][MAX_CMD_SIZE];
 int bufindr = 0;
 int bufindw = 0;
 int buflen  = 0;
int serial_count = 0;
static char serial_char;
static bool comment_mode = false;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

/*蓝牙控制相关变量定义*/
float g_fBTSpeedSet = 0.0;
float g_fBTDirectionSet = 0.0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
 
/**
  * @brief  获取命令
  * @param  none
  * @retval none
  */
void getCommand(void)
{
  while(serial_available() > 0 && buflen < BUFFER_SIZE)
  {
    serial_char = serial_read(); //从缓冲区中读取字符
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
 
float code_value(void)  //将strchr_pointer指针后的字符串中的数字按十进制数转化为数值
{
  return (strtod(&cmdBuffer[bufindr][strchr_pointer - cmdBuffer[bufindr] + 1], NULL));
}

long code_value_long(void)  //将strchr_pointer指针后的字符串中的数字按十进制数转化为双精度数值
{
  return (strtol(&cmdBuffer[bufindr][strchr_pointer - cmdBuffer[bufindr] + 1], NULL, 10));
}

bool code_seen(char code)  //如果字符“code”被找到，返回值为true
{
  strchr_pointer = strchr(cmdBuffer[bufindr], code);
  return (strchr_pointer != NULL);  //Return True if a character was found
}

void processCommand(void)
{
  if(code_seen('C')) //获取到字符C
  {
    switch((int)code_value()) //读取字符C后的数字
    {
      case 0:
      case 1: //设置运动
      {
        if(code_seen('X'))
        {
          g_fBTSpeedSet = code_value(); //将X后面的数据作为速度的设定值
        }
        break;
      }
      case 2: //设置角度PID
      {
        if(code_seen('P')) anglePID._kp = code_value();
        if(code_seen('I')) anglePID._ki = code_value();
        if(code_seen('D')) anglePID._kd = code_value();
        break;
      }
      case 3: //设置速度PID
      {
        if(code_seen('P')) speedPID._kp = code_value();
        if(code_seen('I')) speedPID._ki = code_value();
        if(code_seen('D')) speedPID._kd = code_value();
        break;
      }
      case 4: //设置转向PID
      {
        if(code_seen('P')) turnP = code_value();
        if(code_seen('I')) turnI = code_value();
        if(code_seen('D')) turnD = code_value();
        break;
      }
      case 5: //查询PID值
      {
        LOG_INFO("angle pid: p=%.2f i=%.2f d=%.2f\n", angleP, angleI, angleD);
        LOG_INFO("speed pid: p=%.2f i=%.2f d=%.2f\n", speedP, speedI, speedD);
        LOG_INFO("turn pid: p=%.2f i=%.2f d=%.2f\n", turnP, turnI, turnD);
        break;
      }
    }
  }
  else //未知命令
  {
    LOG_INFO("unknowned command:\"%s\"\n",cmdBuffer[bufindr]);
  }
}


/**
  * @brief  协议解包处理
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
//  * @brief  USART3串口中断服务函数
//  * @param  none
//  * @retval none
//  */ 
//void usart3_irq(void)
//{
//	uint8_t rec;
//  
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		rec = USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
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

