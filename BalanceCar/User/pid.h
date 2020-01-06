/**
  ******************************************************************************
  * @file    pid.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-27
  * @brief   »ù´¡pidÊµÏÖ
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
typedef struct
{
  float _kp;
  float _ki;
  float _kd;
  float _iLimit;
  float _integratedError;
  float _lastError;
}PID_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void pid_set(PID_t *pid, float p, float i, float d, float iLimit);
float pid_update(PID_t *pid,float target, float current, float deltaTime);
void pid_reset(PID_t *pid);


#ifdef __cplusplus
}
#endif

#endif /* __PID_H */


