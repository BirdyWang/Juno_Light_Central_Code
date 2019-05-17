/* UART.c
 * Purpose: This is the file that controls the LE pins to the LED driver for latching
 * Usage: LED system.
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#include "PWM.h"

static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
static nrf_drv_pwm_t m_pwm1 = NRF_DRV_PWM_INSTANCE(1);
nrf_pwm_sequence_t pwm_seq;
nrf_pwm_sequence_t pwm_seq1;

/* We want the PWM generate to be at 4MHz / 16 = 250kHz. 
 * 16MHz / 64 = 250kHz. 
 * So the TOP value is 64. each step is 4. 
 * Only if this method works, theoretically this method will sync up the clock, but in reality due to delay between function calls,
 * The PWM generation happens well before SPI clock is generated. The delay between PWM starts and SPI finishes is 27.78us which 36kHz. 
 * So what needs to happen is that the PWM generation needs to slow down to 36kHz or some value close to it. */
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
        .base_clock   = NRF_PWM_CLK_8MHz, //I am running SPI at 250KHz so this should match
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

void PWM1_init(void)
{
    uint32_t                   err_code;
    nrf_drv_pwm_config_t const config1 =
    {
        .output_pins =
        {
            LED_PWCLK, // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,
            NRF_DRV_PWM_PIN_NOT_USED, 
            NRF_DRV_PWM_PIN_NOT_USED
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_16MHz, //I am running SPI at 250KHz so this should match
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 4,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    err_code = nrf_drv_pwm_init(&m_pwm1, &config1, NULL);
    APP_ERROR_CHECK(err_code);
}

void PWM1_play(void)
{
    static nrf_pwm_values_individual_t pwm_seq_values;
    pwm_seq_values.channel_0 = 2;
    
    pwm_seq1.values.p_individual = &pwm_seq_values;
    pwm_seq1.length = NRF_PWM_VALUES_LENGTH(pwm_seq_values);
    pwm_seq1.repeats = 10;
    pwm_seq1.end_delay = 0;
    
    nrf_drv_pwm_simple_playback(&m_pwm1, &pwm_seq1, 10, NRF_DRV_PWM_FLAG_LOOP);
}

