#include "SAADC.h"

void SAADC_Init(void)
{
    ret_code_t err_code;
    const nrf_saadc_channel_config_t channel_1_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);
    const nrf_saadc_channel_config_t channel_2_config = 
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_saadc_channel_init(1, &channel_1_config);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_saadc_channel_init(2, &channel_2_config);
    APP_ERROR_CHECK(err_code);
}

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
}

nrf_saadc_value_t SAADC_Convert(uint8_t channel)
{
    nrf_saadc_value_t adc_result;
    nrf_drv_saadc_sample_convert(channel, &adc_result);
    return adc_result;
}
