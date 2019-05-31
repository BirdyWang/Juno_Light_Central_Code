/* GPIO.c
 * Purpose: This is the file that handles GPIO inputs from touch ring and battery charger
 * Usage: User Input system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#include "GPIO.h"

volatile uint8_t imuNewGyroFlag = 0;
volatile uint8_t powerOnFlag = 0;
volatile uint8_t batteryChargingFlag = 0;

void GPIO_Init(void) 
{
    /* Initialize the GPIO interrupt. */
    ret_code_t err_code;
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    
    /* Configure the TOUCH_IN pin interrupt to sense toggle. */
    nrf_drv_gpiote_in_config_t touchInConfig = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    touchInConfig.pull = NRF_GPIO_PIN_NOPULL;
    err_code = nrf_drv_gpiote_in_init(TOUCH_IN, &touchInConfig, Touch_In_handler);
    APP_ERROR_CHECK(err_code);
    
    /* Configure the IMU_DATA_READY pin to sense low to high. */
    nrf_drv_gpiote_in_config_t imuIntConfig = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true); 
    imuIntConfig.pull = NRF_GPIO_PIN_NOPULL;
	err_code = nrf_drv_gpiote_in_init(IMU_INT, &imuIntConfig, IMU_Interrupt_Handler);
	APP_ERROR_CHECK(err_code);
    
    /* Configure the CHRG_STAT pin to sense toggle. */
    nrf_drv_gpiote_in_config_t chrgStatConfig = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    chrgStatConfig.pull = NRF_GPIO_PIN_NOPULL;
    err_code = nrf_drv_gpiote_in_init(BATT_CHRG_STAT, &chrgStatConfig, Battery_Charger_Interrupt_handler);
    APP_ERROR_CHECK(err_code);
    
    /* Enable GPIO interrupt. */
    nrf_drv_gpiote_in_event_enable(TOUCH_IN, true);
    nrf_drv_gpiote_in_event_enable(BATT_CHRG_STAT, true);
}

void Touch_In_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    uint32_t touchInLevel = nrf_gpio_pin_read(TOUCH_IN);
    if(touchInLevel == 1)
    {
        powerOnFlag = 1;
    }
    else
    {
        powerOnFlag = 0;
    }
}

void IMU_Interrupt_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    imuNewGyroFlag = 1;
}

void Battery_Charger_Interrupt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    uint32_t chrgInLevel = nrf_gpio_pin_read(TOUCH_IN);
    if(chrgInLevel == 1)
    {
        batteryChargingFlag = 1;
    }
    else
    {
        batteryChargingFlag = 0;
    }
}
