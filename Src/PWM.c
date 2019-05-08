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
            LED_PWM1, // channel 0
            LED_PWM2, // channel 1
            LED_PWM3, // channel 3
            NRF_DRV_PWM_PIN_NOT_USED
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_500kHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = TOP,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    err_code = nrf_drv_pwm_init(&m_pwm0, &config0, NULL);
    APP_ERROR_CHECK(err_code);
}

/* PWM freq: 500Hz.
   PWM 100% duty cycle: 0
   PWM 50% duty cycle: 500
   PWM 0% duty cycle: 1000 */

void PWM_play(rgb_pwm duty_cycle)
{
    static nrf_pwm_values_individual_t pwm_seq_values;
    pwm_seq_values.channel_0 = duty_cycle.r;
    pwm_seq_values.channel_1 = duty_cycle.g;
    pwm_seq_values.channel_2 = duty_cycle.b;
    
    pwm_seq.values.p_individual = &pwm_seq_values;
    pwm_seq.length = NRF_PWM_VALUES_LENGTH(pwm_seq_values);
    pwm_seq.repeats = 10;
    pwm_seq.end_delay = 0;
    
    nrf_drv_pwm_simple_playback(&m_pwm0, &pwm_seq, 10,
                                NRF_DRV_PWM_FLAG_STOP);
   
}
