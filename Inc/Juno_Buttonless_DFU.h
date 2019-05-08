/* Juno_Buttonless_DFU.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#ifndef JUNO_BUTTONLESS_DFU_H
#define JUNO_BUTTONLESS_DFU_H

#include "Juno_BLE.h"
#include "nrf_bootloader_info.h"
#include "ble_dfu.h"

void advertising_config_get(ble_adv_modes_config_t * p_config);
void disconnect(uint16_t conn_handle, void * p_context);
uint8_t Check_Cmd_Grasp_Inrange(uint8_t cmd);
void shutdown_dfu_routine(void);

#endif
