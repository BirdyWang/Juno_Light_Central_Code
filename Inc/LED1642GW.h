/* PWM.h
 * Author: Bird
 * Creation Date: 5/12/2019 
 */
 
#ifndef LED1642GW_H
#define LED1642GW_H

#include "SPI1_LED.h"
#include "PWM.h"

#define DUMMY_DATA_LED1642GW                            1000  
#define WRITE_SWITCH                                    900
#define BRIGHTNESS_DATA_LATCH                           800
#define BRIGHTNESS_GLOBAL_LATCH                         700
#define WRITE_CONFIGURATION_REGISTER                    600
#define READ_CONFIGURATION_REGISTER                     500
#define START_OPEN_ERROR_DETECTION_MODE                 400
#define START_SHORT_ERROR_DETECTION_MODE                300
#define START_COMBINED_ERROR_DETECTION_MODE             200
#define END_ERROR_DETECTON_MODE                         100


#endif
