#include "Command_Handling.h"

extern ble_nus_t m_nus; /**< Structure to identify the Nordic UART Service. */
extern ble_advertising_t m_advertising;
extern uint8_t BLE_cmd[3];
extern unsigned char bootloader_file[6656];
float qd_set[6] = {0,0,0,0,0,0};
float qd[6] = {0,0,0,0,0,0};
uint32_t STM32_binfile_length;
uint32_t STM32_binfile_remain_length;
volatile uint8_t stm32BootloadingInProgress = 0; 

/* For the new hand board configuration. The app will no longer be able to turn the hand on or off...
   So the power on function will not be viable anymore. 
   The ble handling will query battery voltage data on the motor driver through bldc driver, so the battery update routine is still viable. */
void command_handling_ble(const uint8_t * cmd)
{
    if(stm32BootloadingInProgress == 0)
    {
        if(Check_Cmd_Grasp_Inrange(cmd[0]) == 1)
        {
            LED_Off(ROT1);
            LED_Off(ROT2);
            BLE_cmd[1] = cmd[0] - HAND_GRASP_ASCII_BASE;
            set_grasp(qd_set);
        }
        else
        {
            switch(cmd[0])
            {
                case 'A':
                    shutdown_dfu_routine();
                    break;
                case 'B':
                    //BLE_cmd[1] = MODE_SWITCH_OPEN_NO_THUMB_CMD;
                    //set_grasp(qd_set);
                    LED_Off(ROT1);
                    LED_Off(ROT2);
                    LED_Toggle(LED_STAT);
                    break;
                case 'H':
                    shutdown_dfu_routine();
                case 'O':
                    stm32BootloadingInProgress = 1;
                    Boot_Loader(0, (uint8_t *)bootloader_file, 7196);
                    stm32BootloadingInProgress = 0;
                    break;
                case 'a':
                    LED_On(ROT1);
                    LED_Off(ROT2);
                    break;
                case 'z':
                    LED_On(ROT2);
                    LED_Off(ROT1);
                    break;
                default: 
                    break;
                
            }
        }
    }
}

uint8_t Check_Cmd_Grasp_Inrange(uint8_t cmd)
{
    if((cmd >= 0x30 && cmd <=  0x3A))
    {
        return 1;
    }
    return 0;
}

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
    uint32_t err_code;
    err_code = ble_dfu_buttonless_bootloader_start_prepare();
    
}
