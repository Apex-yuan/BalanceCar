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
void pid_init(PID_t *pid, float p, float i, float d, float iLimit,DerivativeSource_t dSource)
{
  pid->_kp = p;
  pid->_ki = i;
  pid->_kd = d;
  pid->_iLimit = iLimit;
  pid->_dSource = dSource;
  pid->_errorSum = 0;
  pid->_lastError = 0;
  pid->_lastInput = 0;
}

float pid_update(PID_t *pid,float setpoint, float measuredValue, float dt)
{
  float error = setpoint - measuredValue;
  float dPart;
  
  /*P*/
  float pPart = pid->_kp * error;
  
  /*I*/
  pid->_errorSum += error;
  pid->_errorSum = constrain(pid->_errorSum, -(pid->_iLimit), pid->_iLimit);
  float iPart = pid->_ki * pid->_errorSum * dt;
  
  /*D*/
  switch(pid->_dSource)
  {
    case FROM_ERROR :
      dPart = pid->_kd * (error - pid->_lastError) / dt;
      break;
    case FROM_MEASURED_VALUE:
      dPart = pid->_kd * (measuredValue - pid->_lastInput) / dt;
      break;
  }
 
  pid->_lastError = error;
  pid->_lastInput = measuredValue;
  
  return (pPart + iPart + dPart);
}

void pid_reset(PID_t *pid)
{
  pid->_errorSum = 0;
  pid->_lastError = 0;
}





