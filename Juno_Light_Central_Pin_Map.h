/* Juno_Light_Central_Pin_Map.h
 * Comment: This is the header file that contains pin configuration for the Juno Light Central Control Circuit V1R1
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#ifndef JUNO_LIGHT_CENTRAL_PIN_MAP_H
#define JUNO_LIGHT_CENTRAL_PIN_MAP_H

#include "nrf_gpio.h"
    
/****************JUNO DEFINED*******************/    
/**For IMU**/
#define IMU_MOSI        1
#define IMU_MISO        2
#define IMU_SCK         3
#define IMU_SS          4
#define IMU_INT         5
    
/**For LED Drivers**/
#define LED_MOSI        6
#define LED_MISO        7
#define LED_SCK         8
#define LED_LE          9
#define LED_PWCLK       10

/**For Touch Input**/
#define TOUCH_IN        11

/**For Battery Charging Status**/
#define BATT_CHRG_STAT  12

/**For UART Debugging**/
#define UART_TX         13
#define UART_RX         14
#define UART_RTS        15
#define UART_CTS        15
#define UART_HWFC       false

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC,            \
                                 .rc_ctiv       = 16,                                \
                                 .rc_temp_ctiv  = 2,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_500_PPM}

#endif // JUNO_LIGHT_CENTRAL_PIN_MAP_H
