/* GPIO.c
 * Purpose: This is the file that handles GPIO inputs from touch ring and battery charger
 * Usage: User Input system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#include "GPIO.h"

volatile uint8_t imu_new_gyro = 0;

void GPIO_Init(void) 
{
    
    ret_code_t err_code;
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    in_config.pull = NRF_GPIO_PIN_NOPULL;
    err_code = nrf_drv_gpiote_in_init(TOUCH_IN, &in_config, Touch_In_handler);
    APP_ERROR_CHECK(err_code);
    
    //nrf_gpio_cfg_input(TOUCH_IN, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BATT_CHRG_STAT, NRF_GPIO_PIN_NOPULL);
    
	err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_config_t imu_int_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true); 
    imu_int_config.pull = NRF_GPIO_PIN_NOPULL;
    
	err_code = nrf_drv_gpiote_in_init(IMU_INT, &imu_int_config, IMU_Interrupt_Handler);
	APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(TOUCH_IN, true);
}

void Touch_In_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    LED1642_LED_RGB_Train_Forward();
}

void IMU_Interrupt_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    imu_new_gyro = 1;
}
