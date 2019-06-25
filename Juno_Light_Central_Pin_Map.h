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
#define IMU_MOSI        16
#define IMU_MISO        14
#define IMU_SCK         18
#define IMU_SS          20
#define IMU_INT         12
    
/**For LED Drivers**/
#define LED_MOSI        9
#define LED_MISO        10
#define LED_SCK         24
#define LED_LE          11
#define LED_PWCLK       23

/**For Touch Input**/
#define TOUCH_IN        27

/**For Battery Charging Status**/
#define PLUG_STAT		28
#define SW_STAT			29
#define BATT_CHRG_STAT  30
#define VBAT_MON        31

/**For UART Debugging**/
#define UART_TX         0
#define UART_RX         2
#define UART_RTS        4
#define UART_CTS        5
#define UART_HWFC       false

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC,            \
                                 .rc_ctiv       = 16,                                \
                                 .rc_temp_ctiv  = 2,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_500_PPM}

#endif // JUNO_LIGHT_CENTRAL_PIN_MAP_H
