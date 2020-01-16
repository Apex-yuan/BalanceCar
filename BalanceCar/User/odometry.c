/**
  ******************************************************************************
  * @file    odometry.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-15
  * @brief   ¿Ô≥Ãº∆
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "odometry.h"
#include "config.h"
#include "lowPassFilter.h"
#include <math.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void odometryCalc(float encoder1, float encoder2, Odometry_t *odom, float dt)
{
	int overflow = 0;								//the global direction need to be internally changed by this number. 
  static float linearSpeed = 0;
  static float angularSpeed = 0;
	
	odom->encoder1Sum += encoder1;
	odom->encoder2Sum += encoder2;
	
	encoder1 = encoder1 * TICK2RAD;			
	encoder2 = encoder2 * TICK2RAD;			
  
	odom->positionChange = WHEEL_RADIUS * (encoder1 + encoder2) / 2;		
	odom->directionChange = WHEEL_RADIUS * (encoder1 - encoder2) / WHEEL_SEPARATION;	

  lowPassFilter(&linearSpeed, odom->positionChange, LINEAR_SPEED_LPF_GAIN);
  lowPassFilter(&angularSpeed, odom->directionChange, ANGULAR_SPEED_LPF_GAIN);

  odom->linearSpeed = linearSpeed / dt;
  odom->angularSpeed = angularSpeed / dt;
	
	odom->direction += odom->directionChange;			
	odom->position.y += odom->positionChange * cos(odom->direction);
	odom->position.x += odom->positionChange * sin(odom->direction);
	
	
	//restrict globalPosition.direction between -180 and 180 to get rid of possible overflow
	if (odom->direction < -180)
	{
		overflow = 360;
		odom->direction = odom->direction+overflow;
	}
	else if (odom->direction > 180)
	{
		overflow = -360;
		odom->direction = odom->direction+overflow;
	}	
}





