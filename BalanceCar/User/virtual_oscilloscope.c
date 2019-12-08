/**
  ******************************************************************************
  * @file    virtual_oscilloscope.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   虚拟示波器，基于山外虚拟示波器的客户端
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "virtual_oscilloscope.h"
#include "usart3.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float g_fware[CHANNEL_NUM]; //波形数据数组，最多显示8个通道

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   发送指定len个字节长度的数组（包括NULL也会发送）
  * @param   buff    数组起始地址
  * @param   len     发送数据的长度
  * @retval  None
  * @example uart_putBuff (UART3,"1234567", 3); //实际发送了3个字节'1','2','3'
  */
static void serial_putBuff (uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        usart3_sendByte(*buff); //需要底层实现的函数：发送一个字节的数据
        buff++;
    }
}

/********************************************************************************************************
* Function name:           vcan_sendware
* Author/Corporation:      vcan
* Create date:             2015-07-05
* input parameters:        none
* output parameters:       none
* Return value:            none   
* Abstract Description:   发送AD值到虚拟示波器
*------------------------Revision History----------------------------------------------------------------
*  NO    Version      Date       Revised By    Description
*  1      V1.1      2016.6.6      xiaoyuan     规范代码风格
*********************************************************************************************************/
void vcan_sendware(uint8_t *wareaddr, uint32_t waresize)
{
    #define CMD_WARE     3   //3代表虚拟示波器,1摄像头，2CCD
  
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //yy_摄像头串口调试 使用的命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //yy_摄像头串口调试 使用的命令
    
    serial_putBuff(cmdf,sizeof(cmdf));
    serial_putBuff(wareaddr,waresize);
    serial_putBuff(cmdr,sizeof(cmdr));
}




