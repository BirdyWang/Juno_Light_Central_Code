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

#define BRIGHTNESS_INCREASE                             1
#define BRIGHTNESS_DECREASE                             2

#define DUMMY_DATA_LED1642GW                            56  
#define WRITE_SWITCH                                    172  // 1-2 rising edges
#define BRIGHTNESS_DATA_LATCH                           158  // 3-4 rising edges
#define BRIGHTNESS_GLOBAL_LATCH                         140  // 5-6 rising edges
#define WRITE_CONFIGURATION_REGISTER                    128  // 7 rising edges
#define READ_CONFIGURATION_REGISTER                     124  // 8 rising edges
#define START_OPEN_ERROR_DETECTION_MODE                 118  // 9 rising edges
#define START_SHORT_ERROR_DETECTION_MODE                110  // 10 rising edges
#define START_COMBINED_ERROR_DETECTION_MODE             102  // 11 rising edges
#define END_ERROR_DETECTON_MODE                         94  // 12 rising edges

#define CFG_AUTOSHUTOFF_60_RISETIME                     0x1400                                 

typedef struct rgb_led
{
    uint8_t r, g, b;
}rgb_led;

typedef struct rgb_led_channel
{
    uint8_t channel;
    rgb_led RGB_Color;
}rgb_led_channel;

enum LED_display_modes {
    SINGLE_COLOR_DISPLAY_BATTERY,
    SINGLE_COLOR_DISPLAY_YELLOW,
    SINGLE_COLOR_DISPLAY_RED,
    SINGLE_COLOR_DISPLAY_GREEN,
    SINGLE_COLOR_DISPALY_BLUE,
    SINGLE_COLOR_DISPLAY_ORANGE,
    MOTION_MAP_DISPLAY,
    COLOR_TRANSITION,
    OTA_UPDATE,
    COS_DISPLAY,
    SIN_PLUS_COS_DISPLAY
};

uint8_t LED1642GW_Init(void);
uint8_t LED1642GW_Enter_LPM(void);
uint8_t LED1642GW_Driver_Count(void);
void LED1642GW_Brightness_Control_PowerOn(uint8_t mode);
void LED1642GW_Brightness_Control_Charging(uint8_t mode);
void LED1642GW_Brightness_Set(uint8_t brightnessInput);
void LED1642_LED_All_On(void);
void LED1642_Switch_On(void);
void LED1642_Switch_Off(void);
void LED1642_LED_RGB_Train_Forward(void);
void LED1642_LED_Different_Color_Same_Time(void);
void LED1642GW_Brightness_Control_Test(void);
void LED1642GW_RGB_Translation_Individual_Channel(uint8_t channel, rgb_led RGB_Color);
void LED1642GW_RGB_Translation_Array(rgb_led * RGB_Color);
void LED1642_Set_Brightness(uint16_t brightness);
void LED1642GW_Write_Configuration_Register(uint16_t tempData);
void LED1642GW_Data_Latch(uint16_t tempData);
void LED1642GW_Global_Latch(uint16_t tempData);
void LED1642GW_Switch(uint16_t tempData);

#endif
