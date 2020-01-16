/**
  ******************************************************************************
  * @file    odometry.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-15
  * @brief   里程计
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ODOMETRY_H
#define __ODOMETRY_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	float x;
	float y;
}Position_t;

typedef struct
{
	Position_t position;
	float direction;
	float directionChange;	//方向该变量
  float positionChange;   //位置变化量
	float linearSpeed;
	float angularSpeed;
	uint32_t encoder1Sum;
	uint32_t encoder2Sum;
}Odometry_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void odometryCalc(float encoder1, float encoder2, Odometry_t *odom, float dt);


#ifdef __cplusplus
}
#endif

#endif /* __ODOMETRY_H */


