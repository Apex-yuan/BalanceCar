/**
  ******************************************************************************
  * @file    lowPassFilter.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-15
  * @brief   µÍÍ¨ÂË²¨Æ÷ÊµÏÖ
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOW_PASS_FILTER_H
#define __LOW_PASS_FILTER_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void lowPassFilter(float * filteredData, float rawData, float gain);


#ifdef __cplusplus
}
#endif

#endif /* __LOW_PASS_FILTER_H */


