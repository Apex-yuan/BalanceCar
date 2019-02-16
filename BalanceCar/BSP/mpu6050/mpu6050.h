#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "stm32f10x.h"

#define q30  1073741824.0f

#define DEFAULT_MPU_HZ  (100)


int8_t MPU_I2C_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);
int8_t MPU_I2C_Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

uint8_t MPU_DMP_Init(void);
uint8_t MPU_DMP_GetData(short *gyro,short *accel ,float *pitch,float *roll,float *yaw);

#endif
