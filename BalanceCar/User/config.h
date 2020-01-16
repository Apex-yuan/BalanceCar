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

#define STOPPING 0
#define RUNNING  1
   
#define CONTROL_PERIOD 5 //5ms
#define DT (float)(CONTROL_PERIOD/1000.0)  //��λ����
#define DT_TO_MINUTES  60
   
#define WHEEL_RADIUS   0.034 //0.033 //meter
#define WHEEL_SEPARATION 0.218 //0.160  //meter

//#define DirectionConst			0.31193 		//wheelDiameter / shaftLength
//#define Encoder_to_mm			0.13694122		//wheelDiameter * PI / 1560

//������תΪ��Ӧ���ȵĳ������ӣ�rad = tick * TICK2RAD
//����ת��һȦ����������13�������������� * 4��һ���������岶׽������ * 30�����ٱȣ� = 1560
//���������Ӧ�Ļ��ȼ� TICK2RAD = 1 / 1560 * (2*PI)
#define TICK2RAD 0.004027683  //    // 0.087890625[deg] * 3.14159265359 / 180 = 0.001533981 

/* �꺯�� */
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>=(high)?(high):(amt)))
#define between(amt,low,high) ((amt)<(low)?(0):((amt)>=(high)?(0):(1)))
#define DEG2RAD(x)   (x * 0.01745329252) // *PI/180
#define RAD2DEG(x)   (x * 57.2957795131) // *180/PI   

#define TURN_ON_ANGLE 5 //��������ֱ���ĽǶ�
#define TURN_OFF_ANGLE 50 //�����Ƕ�
   
//#define OPTICAL_ENCODE_CONSTANT  (13*4) //�������ֱ���
//#define REDUCTION_RATIO          30 //���ٱ�
//#define ENCODER_TICK_PER_ROTATE  (OPTICAL_ENCODE_CONSTANT * REDUCTION_RATIO)
//#define SPEED_CONTROL_PERIOD  5  //�ٶȿ������ڣ�ms��
//#define CAR_SPEED_CONSTANT  (1000.0/SPEED_CONTROL_PERIOD/OPTICAL_ENCODE_CONSTANT/REDUCTION_RATIO)  //��λת������ֵ�����ٶȵ�λת��Ϊ ת/�룬��Ӧ����ת�٣�
   
/* �ٶȵ�ͨ�˲����� */
#define LINEAR_SPEED_LPF_GAIN 0.2
#define ANGULAR_SPEED_LPF_GAIN 0.2

/* ���ٶ� */
#define LINEAR_ACCELERATION 0.05
#define ANGULAR_ACCELERATION 0.1

/*�ٶȿ���PID*/
#define SPEED_P 25
#define SPEED_I 100
#define SPEED_D 0
#define SPEED_I_LIMIT 1000//30

/* �Ƕȿ���PID */
#define ANGLE_P -44  //-55
#define ANGLE_I 0
#define ANGLE_D 1.32 //-1.65 
   
/* ת�����PID */
#define TURN_P 30
#define TURN_I 0
#define TURN_D 0
   
#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */


