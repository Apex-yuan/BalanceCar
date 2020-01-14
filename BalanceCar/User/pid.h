/**
  ******************************************************************************
  * @file    pid.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-27
  * @brief   基础pid实现
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  FROM_ERROR = 0,
  FROM_MEASURED_VALUE = 1
}DerivativeSource_t; //微分项的来源（偏差或测量值）
   
typedef struct
{
  float _kp;
  float _ki;
  float _kd;
  float _iLimit;
  float _errorSum;  //偏差积分
  float _lastError;
  float _lastInput;
  DerivativeSource_t _dSource;  //Proportional on Error
}PID_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void pid_init(PID_t *pid, float p, float i, float d, float iLimit,DerivativeSource_t dSource);
float pid_update(PID_t *pid,float setpoint, float measuredValue, float deltaTime);
void pid_reset(PID_t *pid);


#ifdef __cplusplus
}
#endif

#endif /* __PID_H */


