#include "GPIO.h"
uint32_t stm32BootLoaderPin[6] = {BOOT_CH1, BOOT_CH2, BOOT_CH3, BOOT_CH4, BOOT_CH5, BOOT_CH6};
void GPIO_Init(void) 
{
    /*
    nrf_gpio_cfg_output(UART_DC_SELECT);
    nrf_gpio_pin_write(UART_DC_SELECT, 1); //Default is UART
    __asm{NOP};
    nrf_gpio_pin_write(UART_DC_SELECT, 0); //Default is UART
    __asm{NOP};
    */
    nrf_gpio_cfg_output(LED_STAT);
    LED_Off(LED_STAT);
    nrf_gpio_cfg_output(ROT1);
    LED_Off(ROT1);
    nrf_gpio_cfg_output(ROT2);
    LED_Off(ROT2);
    for(int i = 0; i < 6; i++)
    {
        nrf_gpio_cfg_output(stm32BootLoaderPin[i]);
        nrf_gpio_pin_write(stm32BootLoaderPin[i], 0);
    }
    
    __asm{NOP};
}

void LED_On(uint32_t LED_Pin)
{
    nrf_gpio_pin_write(LED_Pin, 1);
}

void LED_Off(uint32_t LED_Pin)
{
    nrf_gpio_pin_write(LED_Pin, 0);
}

void LED_Toggle(uint32_t LED_Pin)
{
    nrf_gpio_pin_toggle(LED_Pin);
}
