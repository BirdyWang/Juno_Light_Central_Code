/* GPIO.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#ifndef GPIO_H
#define GPIO_H

#include "Juno_Light_Central_Pin_Map.h"
#include "Juno_Light_Central_Config.h"
#include "nrf_gpio.h"
#include "UART.h"
#include "nrf_drv_timer.h"

void GPIO_Init(void);

#endif
