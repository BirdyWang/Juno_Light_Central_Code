#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_uart.h"
#include "app_uart.h"
#include "Juno_Light_Central_Pin_Map.h"

#define IMU_RX          19         
#define IMU_TX          15
#define IMU_CTS         17
#define IMU_RTS         13
#define IMU_HWFC        false

#define UART_TX_BUF_SIZE                32                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                32                                         /**< UART RX buffer size. */

void UART_Init(void);
void uart_error_handle(app_uart_evt_t * p_event);
void UART_Send_Multiple(uint8_t * data, uint8_t len);
