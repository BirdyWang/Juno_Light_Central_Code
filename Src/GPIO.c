/* GPIO.c
 * Purpose: This is the file that handles GPIO inputs from touch ring and battery charger
 * Usage: User Input system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#include "GPIO.h"
void GPIO_Init(void) 
{
    nrf_gpio_cfg_input(TOUCH_IN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(BATT_CHRG_STAT, NRF_GPIO_PIN_NOPULL);
}

/*TODO: Setup GPIO interrupts and corresponding handlers for both input pins. */

