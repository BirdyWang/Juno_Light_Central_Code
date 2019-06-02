/* Command_Handling.c
 * Purpose: This is the file that handles commands from the BLE. 
 * Usage: BLE system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#include "Command_Handling.h"

extern ble_nus_t m_nus; /**< Structure to identify the Nordic UART Service. */
extern ble_advertising_t m_advertising;
extern uint8_t BLE_cmd[3];

/* For the new hand board configuration. The app will no longer be able to turn the hand on or off...
   So the power on function will not be viable anymore. 
   The ble handling will query battery voltage data on the motor driver through bldc driver, so the battery update routine is still viable. */
void command_handling_ble(const uint8_t * cmd)
{
    switch(cmd[0])
    {
        case 'O':
            shutdown_dfu_routine();
        default: 
            break;
        
    }
}

