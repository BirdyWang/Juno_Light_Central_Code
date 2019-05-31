/* UART.c
 * Purpose: This is the file that performs ADC conversions. 
 *          It will potentially be used if touch ring digital in does not work as expected.
 * Usage: Touch input system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#include "SAADC.h"

/* The reference voltage is 0.6V, the gain is 6x. */
void SAADC_Init(void)
{
    ret_code_t err_code;
    const nrf_saadc_channel_config_t vbatMonConfig =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_saadc_channel_init(0, &vbatMonConfig);
    APP_ERROR_CHECK(err_code);
}

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
}

nrf_saadc_value_t SAADC_Convert(void)
{
    nrf_saadc_value_t adc_result;
    nrf_drv_saadc_sample_convert(0, &adc_result);
    return adc_result;
}
