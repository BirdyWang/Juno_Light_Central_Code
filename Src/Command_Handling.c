#include "Command_Handling.h"

extern ble_nus_t m_nus; /**< Structure to identify the Nordic UART Service. */
extern ble_advertising_t m_advertising;
extern uint8_t BLE_cmd[3];
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

