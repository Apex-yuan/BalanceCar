/**
  ******************************************************************************
  * @file    pid.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-27
  * @brief   »ù´¡pidÊµÏÖ
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "pid.h"
#include "config.h"
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
void pid_set(PID_t *pid, float p, float i, float d, float iLimit)
{
  pid->_kp = p;
  pid->_ki = i;
  pid->_kd = d;
  pid->_iLimit = iLimit;
  pid->_integratedError = 0;
  pid->_lastError = 0;
}

float pid_update(PID_t *pid,float target, float current, float deltaTime)
{
  float error = (target - current) * deltaTime;
  float pPart = pid->_kp * error;
  
  pid->_integratedError += error;
  pid->_integratedError = constrain(pid->_integratedError, -(pid->_iLimit), pid->_iLimit);
  float iPart = pid->_ki * pid->_integratedError;
  
  float dPart = pid->_kd * (error - pid->_lastError);
  pid->_lastError = error;
  return (pPart + iPart + dPart);
}

void pid_reset(PID_t *pid)
{
  pid->_integratedError = 0;
  pid->_lastError = 0;
}





