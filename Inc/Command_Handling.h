/* Command_Handling.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#ifndef COMMAND_HANDLING_H
#define COMMAND_HANDLING_H

#include "app_uart.h"
#include "PWM.h"
#include "Juno_BLE.h"
#include "Juno_Buttonless_DFU.h"
#include "GPIO.h"
#include "nrf_power.h"
#include "nrf_bootloader_info.h"
#include "ble_dfu.h"

void command_handling_ble(const uint8_t * cmd);

#endif
