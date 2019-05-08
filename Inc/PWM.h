#ifndef PWM_H
#define PWM_H

#include "nrf_drv_pwm.h"
#include "Juno_Light_Central_Pin_Map.h"

#define PWM_PIN1    29
#define PWM_PIN2    31

#define TOP         1000
#define STEP_COUNT  25
    
typedef struct rgb_pwm 
{
    nrf_pwm_values_common_t r, g, b;
}rgb_pwm;    
void PWM_init(void);
void PWM_play(rgb_pwm duty_cycle);

#endif
