#ifndef PSYONIC_BUTTONLESS_DFU_H
#define PSYONIC_BUTTONLESS_DFU_H

#include "Psyonic_BLE.h"
#include <stdbool.h>

#include "nrf_dfu_ble_svci_bond_sharing.h"
#include "nrf_svci_async_function.h"
#include "nrf_svci_async_handler.h"

#include "app_error.h"
#include "ble.h"

#include "app_timer.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "ble_hci.h"
#include "ble_dfu.h"
#include "fds.h"
#include "nrf_power.h"
#include "nrf_bootloader_info.h"

#define SEC_PARAM_BOND                  1                                           /**< Perform bonding. */
#define SEC_PARAM_MITM                  0                                           /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                  0                                           /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS              0                                           /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                        /**< No I/O capabilities. */
#define SEC_PARAM_OOB                   0                                           /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE          7                                           /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE          16                                          /**< Maximum encryption key size. */

void buttonless_dfu_init(void);
bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event);
void buttonless_dfu_sdh_state_observer(nrf_sdh_state_evt_t state, void * p_context);
void disconnect(uint16_t conn_handle, void * p_context);
void ble_dfu_evt_handler(ble_dfu_buttonless_evt_type_t event);
void pm_evt_handler(pm_evt_t const * p_evt);
void advertising_config_get(ble_adv_modes_config_t * p_config);
void disconnect(uint16_t conn_handle, void * p_context);
void dfu_service_init(void);
void peer_manager_init(void);
void delete_bonds(void);
    
#endif
