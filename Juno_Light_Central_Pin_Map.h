/**
 * Copyright (c) 2014 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#ifndef APP_TEST_PCB_PIN_MAP_H
#define APP_TEST_PCB_PIN_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"
    
/****************PSYONIC DEFINED*******************/    
#define SPI_MISO        10
#define SPI_MOSI        9
#define SPI_CLK         11

#define SS_CH1          12          
#define SS_CH2          29
#define SS_CH3          28
#define SS_CH4          27
#define SS_CH5          26
#define SS_CH6          25
#define SS_CH7          2

#define IMU_INT         4

#define LED_PWM1        22
#define LED_PWM2        23
#define LED_PWM3        24

#define LED_STAT        16

#define BOOT_RX         18
#define BOOT_TX         14

#define BOOT_CTS        17
#define BOOT_RTS        19
#define BOOT_HWFC       false
    

    
#define BOOT_CH1        0
#define BOOT_CH2        1
#define BOOT_CH3        3
#define BOOT_CH4        5
#define BOOT_CH5        6
#define BOOT_CH6        7

#define ROT1            31
#define ROT2            30

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC,            \
                                 .rc_ctiv       = 16,                                \
                                 .rc_temp_ctiv  = 2,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_500_PPM}

#ifdef __cplusplus
}
#endif

#endif // APP_TEST_PCB_PIN_MAP_H
