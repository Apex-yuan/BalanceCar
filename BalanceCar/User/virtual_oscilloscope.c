/**
  ******************************************************************************
  * @file    virtual_oscilloscope.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   ����ʾ����������ɽ������ʾ�����Ŀͻ���
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
float g_fware[CHANNEL_NUM]; //�����������飬�����ʾ8��ͨ��

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   ����ָ��len���ֽڳ��ȵ����飨����NULLҲ�ᷢ�ͣ�
  * @param   buff    ������ʼ��ַ
  * @param   len     �������ݵĳ���
  * @retval  None
  * @example uart_putBuff (UART3,"1234567", 3); //ʵ�ʷ�����3���ֽ�'1','2','3'
  */
static void serial_putBuff (uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        usart3_sendByte(*buff); //��Ҫ�ײ�ʵ�ֵĺ���������һ���ֽڵ�����
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
* Abstract Description:   ����ADֵ������ʾ����
*------------------------Revision History----------------------------------------------------------------
*  NO    Version      Date       Revised By    Description
*  1      V1.1      2016.6.6      xiaoyuan     �淶������
*********************************************************************************************************/
void vcan_sendware(uint8_t *wareaddr, uint32_t waresize)
{
    #define CMD_WARE     3   //3��������ʾ����,1����ͷ��2CCD
  
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //yy_����ͷ���ڵ��� ʹ�õ�����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //yy_����ͷ���ڵ��� ʹ�õ�����
    
    serial_putBuff(cmdf,sizeof(cmdf));
    serial_putBuff(wareaddr,waresize);
    serial_putBuff(cmdr,sizeof(cmdr));
}




