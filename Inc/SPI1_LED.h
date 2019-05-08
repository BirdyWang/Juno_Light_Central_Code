/* SPI1_LED.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#ifndef SPI1_LED_H
#define SPI1_LED_H

#include "Juno_Light_Central_Pin_Map.h"
#include "nrf_spi_mngr.h"
#include "nrf_delay.h"

#define TRANSACTION_QUEUE_SIZE  3
#define LED_SPI_INSTANCE        1

void LED_SPI_Init(void);
void LED_SPI_Manager_Init(void);
void LED_SPI_Master_Uninit(void);
void LED_Start_Transaction(void * p_user_data);
void LED_Stop_Transaction(ret_code_t result, void * p_user_data);
uint32_t LED_SPI_Transmit(uint8_t * Tx_data, uint8_t tx_data_length);
uint32_t LED_SPI_Receive(uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length);

#endif
