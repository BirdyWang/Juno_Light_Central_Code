#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_saadc.h"

void SAADC_Init(void);
void saadc_callback(nrf_drv_saadc_evt_t const * p_event);
nrf_saadc_value_t SAADC_Convert(uint8_t channel);

#endif
