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

#define STOPPING 0
#define RUNNING  1
   
#define CONTROL_PERIOD 5 //5ms
#define DT (float)(CONTROL_PERIOD/1000.0)  //单位：秒
#define DT_TO_MINUTES  60
   
#define WHEEL_RADIUS   0.034 //0.033 //meter
#define WHEEL_SEPARATION 0.218 //0.160  //meter

//#define DirectionConst			0.31193 		//wheelDiameter / shaftLength
//#define Encoder_to_mm			0.13694122		//wheelDiameter * PI / 1560

//脉冲数转为对应弧度的乘数因子：rad = tick * TICK2RAD
//轮子转动一圈的脉冲数：13（编码器线数） * 4（一个周期脉冲捕捉次数） * 30（减速比） = 1560
//单个脉冲对应的弧度即 TICK2RAD = 1 / 1560 * (2*PI)
#define TICK2RAD 0.004027683  //    // 0.087890625[deg] * 3.14159265359 / 180 = 0.001533981 

/* 宏函数 */
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>=(high)?(high):(amt)))
#define between(amt,low,high) ((amt)<(low)?(0):((amt)>=(high)?(0):(1)))
#define DEG2RAD(x)   (x * 0.01745329252) // *PI/180
#define RAD2DEG(x)   (x * 57.2957795131) // *180/PI   

#define TURN_ON_ANGLE 5 //跌倒后开启直立的角度
#define TURN_OFF_ANGLE 50 //跌倒角度
   
//#define OPTICAL_ENCODE_CONSTANT  (13*4) //编码器分辨率
//#define REDUCTION_RATIO          30 //减速比
//#define ENCODER_TICK_PER_ROTATE  (OPTICAL_ENCODE_CONSTANT * REDUCTION_RATIO)
//#define SPEED_CONTROL_PERIOD  5  //速度控制周期（ms）
//#define CAR_SPEED_CONSTANT  (1000.0/SPEED_CONTROL_PERIOD/OPTICAL_ENCODE_CONSTANT/REDUCTION_RATIO)  //单位转化比例值（将速度单位转化为 转/秒，对应轮子转速）
   
/* 速度低通滤波增益 */
#define LINEAR_SPEED_LPF_GAIN 0.2
#define ANGULAR_SPEED_LPF_GAIN 0.2

/* 加速度 */
#define LINEAR_ACCELERATION 0.05
#define ANGULAR_ACCELERATION 0.1

/*速度控制PID*/
#define SPEED_P 25
#define SPEED_I 100
#define SPEED_D 0
#define SPEED_I_LIMIT 1000//30

/* 角度控制PID */
#define ANGLE_P -44  //-55
#define ANGLE_I 0
#define ANGLE_D 1.32 //-1.65 
   
/* 转向控制PID */
#define TURN_P 30
#define TURN_I 0
#define TURN_D 0
   
#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */


