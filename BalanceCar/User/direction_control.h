/**
  ******************************************************************************
  * @file    direction_control.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   方向控制相关代码
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIRECTION_CONTROL_H
#define __DIRECTION_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define DIRECTION_CONTROL_PERIOD 10

/* Exported variables ------------------------------------------------------- */
extern float g_fDirectionControlOut;
extern float g_fDirectionControlNew, g_fDirectionControlOld;
extern uint8_t g_nDirectionControlPeriod;
extern uint16_t g_nDirectionControlCount;

extern float g_fDirectionSet;
/* Exported functions ------------------------------------------------------- */
void DirectionControl(void);
void DirectionControlOutput(void);


#ifdef __cplusplus
}
#endif

#endif /* __DIRECTION_CONTROL_H */


