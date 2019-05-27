/* GPIO.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#ifndef GPIO_H
#define GPIO_H

#include "Juno_Light_Central_Pin_Map.h"
#include "Juno_Light_Central_Config.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "LED1642GW.h"
#include 

void GPIO_Init(void);
void Touch_In_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void IMU_Interrupt_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

#endif
