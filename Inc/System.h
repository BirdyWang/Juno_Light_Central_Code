#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include "nrf_drv_clock.h"
#include "nrf_drv_timer.h"
#include "app_timer.h"
#include "nrf_delay.h"
#include "Juno_Light_Central_Pin_Map.h"
#include "Juno_Light_Central_Config.h"


#define APP_TIMER_PRESCALER               0                                            /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE           4

void timers_init(void);
void sleep(void);
void Timer2_Init(void);
void timer_ms_event_handler(nrf_timer_event_t event_type, void* p_context);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);

#endif
