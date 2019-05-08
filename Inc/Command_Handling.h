#ifndef COMMAND_HANDLING_H
#define COMMAND_HANDLING_H

#include "app_uart.h"
#include "PWM.h"
#include "Juno_BLE.h"
#include "GPIO.h"
#include "nrf_power.h"
#include "nrf_bootloader_info.h"
#include "ble_dfu.h"

#define HAND_SET_GRASP              0x1D
#define HAND_PRES_ENABLE            0x65
#define HAND_PRES_DISABLE           0x69
#define HAND_READ_PRESSURE          0x70

#define HAND_RE_CALIBRATE 		    0xCB

#define CHANGE_SELECTED_SENSOR		0x73

#define CONTACT_REFLEX_ENABLE		0x75
#define CONTACT_REFLEX_DISABLE		0x76

#define HAND_GRIP_STR_255           0xFF
#define HAND_GRASP_ASCII_BASE       0x30

void command_handling(uint8_t * cmd);
//void command_handling_uart(uint8_t * cmd);
void command_handling_ble(const uint8_t * cmd);
uint8_t Check_Cmd_Grasp_Inrange(uint8_t cmd);

#endif
