/* PWM.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#ifndef PWM_H
#define PWM_H

#include "Juno_Light_Central_Pin_Map.h"
#include "nrf_drv_pwm.h"

#define TOP         256
    
void PWM_init(void);
void PWM_play(nrf_pwm_values_common_t duty_cycle);
void PWM1_init(void);
void PWM1_play(void);

#endif
