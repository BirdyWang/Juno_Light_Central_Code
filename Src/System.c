/* System.c
 * Purpose: This is the file that implements system tick timer as well as sleep logic.
 * Usage: Timer and sleep system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */

#include "System.h"

const nrf_drv_timer_t Timer_ms_Tick = NRF_DRV_TIMER_INSTANCE(2);
uint32_t timerTicks;

/**@brief Function for initializing the timer module.
 */
void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

void sleep(void)
{
    //__SEV();
    /* Wait for event */
    __WFE();
    /* Wait for event */
    __WFE();
    //__WFI();
}

void Timer2_Init(void)
{
    uint32_t timer_ticks;
    uint32_t err_code = NRF_SUCCESS;
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    
    //nrf_gpio_cfg_output(CALI_TEST_PIN);
    
    timer_cfg.interrupt_priority = 3;
    err_code = nrf_drv_timer_init(&Timer_ms_Tick, &timer_cfg, timer_ms_event_handler);
    APP_ERROR_CHECK(err_code);
    timer_ticks = nrf_drv_timer_ms_to_ticks(&Timer_ms_Tick, 1);
    nrf_drv_timer_extended_compare(
         &Timer_ms_Tick, NRF_TIMER_CC_CHANNEL2, timer_ticks, NRF_TIMER_SHORT_COMPARE2_CLEAR_MASK, true);
    timerTicks = 0;
    nrf_drv_timer_enable(&Timer_ms_Tick);
}

void timer_ms_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    timerTicks++;
    //nrf_gpio_pin_toggle(CALI_TEST_PIN);
#if (EN_CALIBRATE_TIMER == 1)
    nrf_gpio_pin_toggle(CALI_TEST_PIN);
#endif
}

uint32_t HAL_GetTick(void)
{
    return timerTicks;
}

void HAL_Delay(uint32_t Delay)
{
    nrf_delay_ms(Delay);
}
