/* UART.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
 
#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "Juno_Light_Central_Pin_Map.h"
#include "nrf_uart.h"
#include "app_uart.h"

#define UART_TX_BUF_SIZE                512                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                512                                         /**< UART RX buffer size. */

void UART_Init(void);
void uart_error_handle(app_uart_evt_t * p_event);
void UART_Send_Multiple(uint8_t * data, uint8_t len);

#endif
