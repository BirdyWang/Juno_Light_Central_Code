/* PWM.h
 * Author: Bird
 * Creation Date: 5/12/2019 
 */
 
#ifndef LED1642GW_H
#define LED1642GW_H

#include "SPI1_LED.h"
#include "PWM.h"

#define DRIVER_NUM                                      3
#define CHANNEL_PER_DRIVER_NUM                          16
#define SPI_BUFFER_SIZE                                 CHANNEL_PER_DRIVER_NUM * DRIVER_NUM


#define DUMMY_DATA_LED1642GW                            56  
#define WRITE_SWITCH                                    172  // 1-2 rising edges
#define BRIGHTNESS_DATA_LATCH                           156  // 3-4 rising edges
#define BRIGHTNESS_GLOBAL_LATCH                         144  // 5-6 rising edges
#define WRITE_CONFIGURATION_REGISTER                    132  // 7 rising edges
#define READ_CONFIGURATION_REGISTER                     126  // 8 rising edges
#define START_OPEN_ERROR_DETECTION_MODE                 118  // 9 rising edges
#define START_SHORT_ERROR_DETECTION_MODE                110  // 10 rising edges
#define START_COMBINED_ERROR_DETECTION_MODE             102  // 11 rising edges
#define END_ERROR_DETECTON_MODE                         94  // 12 rising edges

void LED1642GW_Test_WaveFormGeneration(nrf_pwm_values_common_t duty_cycle);
void LED1642GW_Driver_Count(void);
void LED1642_LED_All_On(void);
void LED1642GW_Write_Configuration_Register(uint16_t tempData);
void LED1642GW_Data_Latch(uint16_t tempData);
void LED1642GW_Global_Latch(uint16_t tempData);
void LED1642GW_Switch(uint16_t tempData);

#endif
