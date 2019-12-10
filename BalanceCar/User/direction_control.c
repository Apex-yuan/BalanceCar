/**
  ******************************************************************************
  * @file    direction_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   方向控制相关代码
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "direction_control.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float g_fDirectionControlOut = 0;
float g_fDirectionControlNew, g_fDirectionControlOld;
uint8_t g_nDirectionControlPeriod;
uint16_t g_nDirectionControlCount;
float g_fDirectionSet;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void DirectionControl(void)
{
//  float fValue; 
  g_fDirectionControlNew = g_fDirectionSet;
  
  g_fDirectionControlOld = g_fDirectionControlNew;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void DirectionControlOutput(void)
{
  float fValue;
  
  fValue = g_fDirectionControlNew - g_fDirectionControlOld;
  g_fDirectionControlOut = fValue * (g_nDirectionControlPeriod + 1) / 
                           DIRECTION_CONTROL_PERIOD + g_fDirectionControlOld; 
}


