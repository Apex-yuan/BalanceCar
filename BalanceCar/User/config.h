/**
  ******************************************************************************
  * @file    config.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-06
  * @brief   配置文件
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

#define CONTROL_PERIOD 5 //5ms
#define DT (float)(CONTROL_PERIOD/1000.0)
   
/* 宏函数 */
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>=(high)?(high):(amt)))
#define DEG2RAD(x)   (x * 0.01745329252) // *PI/180
#define RAD2DEG(x)   (x * 57.2957795131) // *180/PI   
   
#define OPTICAL_ENCODE_CONSTANT  (13*4) //编码器分辨率
#define REDUCTION_RATIO          30 //减速比
#define SPEED_CONTROL_PERIOD  5  //速度控制周期（ms）
#define CAR_SPEED_CONSTANT  (1000.0/SPEED_CONTROL_PERIOD/OPTICAL_ENCODE_CONSTANT/REDUCTION_RATIO)  //单位转化比例值（将速度单位转化为 转/秒，对应轮子转速）
   

/* 加速度 */
#define SPEED_ACCELERATION 0.05

/*速度控制PID*/
#define SPEED_P 2.0//1.05 //35; //240;//400;
#define SPEED_I 0.13  //1.2;//2.0;
#define SPEED_D 0
#define SPEED_I_LIMIT 30

/* 角度控制PID */
#define ANGLE_P 44//-44//-55 //45;//67.5;
#define ANGLE_I 0
#define ANGLE_D -1.32//-1.65 //1.6;//3.0;
   
/* 转向控制PID */
#define TURN_P 0
#define TURN_I 0
#define TURN_D 0
   
#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */


