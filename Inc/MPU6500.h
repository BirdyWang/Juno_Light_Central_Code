#ifndef MPU6500_H
#define MPU6500_H

#include "MPU6500_DMP_Motion_Driver.h"
#include "UART.h"

#define MPU6500_READ             0x80
#define MPU6500_WRITE            0x00

#define MPU6500_ACCEL_XOUT_H     0x3B
#define MPU6500_ACCEL_XOUT_L     0x3C
#define MPU6500_ACCEL_YOUT_H     0x3D
#define MPU6500_ACCEL_YOUT_L     0x3E
#define MPU6500_ACCEL_ZOUT_H     0x3F
#define MPU6500_ACCEL_ZOUT_L     0x40
#define MPU6500_TEMP_OUT_H       0x41
#define MPU6500_TEMP_OUT_L       0x42
#define MPU6500_GYRO_XOUT_H      0x43
#define MPU6500_GYRO_XOUT_L      0x44
#define MPU6500_GYRO_YOUT_H      0x45
#define MPU6500_GYRO_YOUT_L      0x46
#define MPU6500_GYRO_ZOUT_H      0x47
#define MPU6500_GYRO_ZOUT_L      0x48
#define MPU6500_WHO_AM_I         0x75

uint8_t MPU6500_Connection_Test(void);
uint8_t MPU6500_Setup(void);
void MPU6500_Enable_DMP(void);
void MPU6500_Disable_DMP(void);
void MPU6500_Enable_Tap_Feature(void);
void MPU6500_Enable_Full_Features(void);
void getDMP_Data(void);
static void MPU6500_tap_cb(uint8_t direction, uint8_t count);
static void MPU6500_orient_cb(uint8_t orientation);
static inline uint16_t inv_row_2_scale(const signed char *row);
static inline uint16_t inv_orientation_matrix_to_scalar(const signed char *mtx);
void send_packet(char packet_type, void *data);
#endif
