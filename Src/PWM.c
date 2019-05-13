/* UART.c
 * Purpose: This is the file that controls the LE pins to the LED driver for latching
 * Usage: LED system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#include "PWM.h"

static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
nrf_pwm_sequence_t pwm_seq;

void PWM_init(void)
{
    uint32_t                   err_code;
    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            LED_LE, // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,
            NRF_DRV_PWM_PIN_NOT_USED, 
            NRF_DRV_PWM_PIN_NOT_USED
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_250kHz, //I am running SPI at 250KHz so this should match
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = TOP,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    err_code = nrf_drv_pwm_init(&m_pwm0, &config0, NULL);
    APP_ERROR_CHECK(err_code);
}

/* PWM freq: 250Hz.
   PWM 100% duty cycle: 0
   PWM 50% duty cycle: 500
   PWM 0% duty cycle: 1000 */
void PWM_play(nrf_pwm_values_common_t duty_cycle)
{
    static nrf_pwm_values_individual_t pwm_seq_values;
    pwm_seq_values.channel_0 = duty_cycle;
    
    pwm_seq.values.p_individual = &pwm_seq_values;
    pwm_seq.length = NRF_PWM_VALUES_LENGTH(pwm_seq_values);
    pwm_seq.repeats = 1;
    pwm_seq.end_delay = 0;
    
    nrf_drv_pwm_simple_playback(&m_pwm0, &pwm_seq, 1, NRF_DRV_PWM_FLAG_STOP);
}
