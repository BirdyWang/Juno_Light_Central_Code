#ifndef GPIO_H
#define GPIO_H

#include "App_Test_PCB_Config.h"
#include "nrf_gpio.h"
#include "UART.h"
#include "nrf_drv_timer.h"
#include "Pullstring_Control.h"
#include "App_Test_PCB_Pin_Map.h"

void GPIO_Init(void);
void LED_On(uint32_t LED_Pin);
void LED_Off(uint32_t LED_Pin);
void LED_Toggle(uint32_t LED_Pin);

#endif
