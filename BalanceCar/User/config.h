/**
  ******************************************************************************
  * @file    config.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-06
  * @brief   �����ļ�
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* �꺯�� */
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>=(high)?(high):(amt)))
#define DEG2RAD(x)   (x * 0.01745329252) // *PI/180
#define RAD2DEG(x)   (x * 57.2957795131) // *180/PI   
   
#define OPTICAL_ENCODE_CONSTANT  (13*4) //�������ֱ���
#define REDUCTION_RATIO          30 //���ٱ�
#define SPEED_CONTROL_PERIOD  5  //�ٶȿ������ڣ�ms��
#define CAR_SPEED_CONSTANT  (1000.0/SPEED_CONTROL_PERIOD/OPTICAL_ENCODE_CONSTANT/REDUCTION_RATIO)  //��λת������ֵ�����ٶȵ�λת��Ϊ ת/�룬��Ӧ����ת�٣�
   

/* ���ٶ� */
#define SPEED_ACCELERATION 0.05

/*�ٶȿ���PID*/
#define SPEED_P 2.0//1.05 //35; //240;//400;
#define SPEED_I 0.13  //1.2;//2.0;
#define SPEED_I_LIMIT 25

/* �Ƕȿ���PID */
#define ANGLE_P -44//-55 //45;//67.5;
#define ANGLE_D -1.32//-1.65 //1.6;//3.0;
   
#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */


