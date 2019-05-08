/* Juno_Buttonless_DFU.c
 * Purpose: This is the file that implements buttonless DFU routine. 
 * Usage: BLE system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */

#include "Juno_Buttonless_DFU.h"

extern ble_nus_t m_nus; /**< Structure to identify the Nordic UART Service. */
extern ble_advertising_t m_advertising;

void advertising_config_get(ble_adv_modes_config_t * p_config)
{
    memset(p_config, 0, sizeof(ble_adv_modes_config_t));

    p_config->ble_adv_fast_enabled  = true;
    p_config->ble_adv_fast_interval = APP_ADV_INTERVAL;
    p_config->ble_adv_fast_timeout  = APP_ADV_DURATION;
}

void disconnect(uint16_t conn_handle, void * p_context)
{
    UNUSED_PARAMETER(p_context);

    ret_code_t err_code = sd_ble_gap_disconnect(conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_WARNING("Failed to disconnect connection. Connection handle: %d Error: %d", conn_handle, err_code);
    }
    else
    {
        NRF_LOG_DEBUG("Disconnected connection handle %d", conn_handle);
    }
}

void shutdown_dfu_routine(void)
{
    /*prepare to enter bootloader*/
    // Prevent device from advertising on disconnect.
    ble_adv_modes_config_t config;
    advertising_config_get(&config);
    config.ble_adv_on_disconnect_disabled = true;
    ble_advertising_modes_config_set(&m_advertising, &config);

    // Disconnect all other bonded devices that currently are connected.
    // This is required to receive a service changed indication
    // on bootup after a successful (or aborted) Device Firmware Update.
    uint32_t conn_count = ble_conn_state_for_each_connected(disconnect, NULL);
    /*Code from shutdown handler. */
    ble_dfu_buttonless_bootloader_start_prepare();
}
