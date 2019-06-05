#ifndef MPU6050_BASE_DRIVER_H
#define MPU6050_BASE_DRIVER_H

#include "SPI0_IMU.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"

#define MPU6500_SS_CH                   6
#define MPU6500_READ                    0x80
#define MPU6500_WRITE                   0x00

#define MAX_PACKET_LENGTH               (12)
#define HWST_MAX_PACKET_LENGTH          (512)

#define INV_X_GYRO                      (0x40)
#define INV_Y_GYRO                      (0x20)
#define INV_Z_GYRO                      (0x10)
#define INV_XYZ_GYRO                    (INV_X_GYRO | INV_Y_GYRO | INV_Z_GYRO)
#define INV_XYZ_ACCEL                   (0x08)
#define INV_XYZ_COMPASS                 (0x01)

#define MPU_INT_STATUS_DATA_READY       (0x0001)
#define MPU_INT_STATUS_DMP              (0x0002)
#define MPU_INT_STATUS_PLL_READY        (0x0004)
#define MPU_INT_STATUS_I2C_MST          (0x0008)
#define MPU_INT_STATUS_FIFO_OVERFLOW    (0x0010)
#define MPU_INT_STATUS_ZMOT             (0x0020)
#define MPU_INT_STATUS_MOT              (0x0040)
#define MPU_INT_STATUS_FREE_FALL        (0x0080)
#define MPU_INT_STATUS_DMP_0            (0x0100)
#define MPU_INT_STATUS_DMP_1            (0x0200)
#define MPU_INT_STATUS_DMP_2            (0x0400)
#define MPU_INT_STATUS_DMP_3            (0x0800)
#define MPU_INT_STATUS_DMP_4            (0x1000)
#define MPU_INT_STATUS_DMP_5            (0x2000)

void set_int_enable(uint8_t enable);
void mpu_reg_dump(void);
int mpu_read_reg(uint8_t reg, uint8_t *data);
void mpu_reset(void);
void mpu_sleep(void);
void mpu_wakeup(void);
int mpu_init(void);
int mpu_lp_accel_mode(uint16_t rate);
int mpu_get_gyro_reg(short *data);
int mpu_get_accel_reg(short *data);
int mpu_get_temperature(long *data);
int mpu_read_6500_accel_bias(long *accel_bias);
int mpu_read_6500_gyro_bias(long *gyro_bias);
int mpu_set_gyro_bias_reg(long *gyro_bias);
int mpu_set_accel_bias_6500_reg(const long *accel_bias);
int mpu_reset_fifo(void);
int mpu_get_gyro_fsr(unsigned short *fsr);
int mpu_set_gyro_fsr(unsigned short fsr);
int mpu_get_accel_fsr(uint8_t *fsr);
int mpu_set_accel_fsr(uint8_t fsr);
int mpu_get_lpf(unsigned short *lpf);
int mpu_set_lpf(unsigned short lpf);
int mpu_get_sample_rate(unsigned short *rate);
int mpu_set_sample_rate(unsigned short rate);
int mpu_get_gyro_sens(float *sens);
int mpu_get_accel_sens(unsigned short *sens);
int mpu_get_fifo_config(uint8_t *sensors);
int mpu_configure_fifo(uint8_t sensors);
int mpu_get_power_state(uint8_t *power_on);
int mpu_set_sensors(uint8_t sensors);
int mpu_get_int_status(short *status);
int mpu_read_fifo(short *gyro, short *accel, unsigned long *timestamp, uint8_t *sensors, uint8_t *more);
int mpu_read_fifo_stream(unsigned short length, uint8_t *data, uint8_t *more);
int mpu_set_int_level(uint8_t active_low);
int mpu_set_int_latched(uint8_t enable);
int get_st_biases(long *gyro, long *accel, uint8_t hw_test);
int accel_6500_self_test(long *bias_regular, long *bias_st);
int gyro_6500_self_test(long *bias_regular, long *bias_st);
int get_st_6500_biases(long *gyro, long *accel, uint8_t hw_test, int debug);
int mpu_run_6500_self_test(long *gyro, long *accel);
int mpu_run_self_test(long *gyro, long *accel);
int mpu_write_mem(unsigned short mem_addr, unsigned short length, uint8_t *data);
int mpu_read_mem(unsigned short mem_addr, unsigned short length, uint8_t *data);
int mpu_load_firmware(unsigned short length, const uint8_t *firmware, unsigned short start_addr, unsigned short sample_rate);
int mpu_set_dmp_state(uint8_t enable);
int mpu_get_dmp_state(uint8_t *enabled);
int mpu_lp_motion_interrupt(unsigned short thresh, uint8_t time, uint8_t lpa_freq);
void IMU_Interrupt_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

#endif
