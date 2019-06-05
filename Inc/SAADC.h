/* SAADC.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_saadc.h"

#define BATTERY_CONV_RATIO  7.2f/1024.0f      

void SAADC_Init(void);
void saadc_callback(nrf_drv_saadc_evt_t const * p_event);
nrf_saadc_value_t SAADC_Convert(void);

#endif
