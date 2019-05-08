#ifndef SPI_H
#define SPI_H

#include "Juno_Light_Central_Pin_Map.h"
#include "nrf_spi_mngr.h"
#include "nrf_delay.h"

#define TRANSACTION_QUEUE_SIZE  3
#define SPI_INSTANCE            0
#define NUM_CHANNELS            6

void SPI_Init(void);
void SPI_Manager_Init(void);
void SPI_Master_Uninit(void);
void Start_Transaction(void * p_user_data);
void Stop_Transaction(ret_code_t result, void * p_user_data);
void Stop_Transaction2(ret_code_t result, void * p_user_data);
uint32_t SPI_Transmit(uint32_t Slave_Channel, uint8_t * Tx_data, uint8_t tx_data_length);
uint32_t SPI_Transmit2(uint32_t Slave_Channel, uint8_t * Tx_reg, uint8_t * Tx_data, uint8_t tx_data_length);
uint32_t SPI_Receive(uint32_t Slave_Channel, uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length);
uint32_t SPI_Receive2(uint32_t Slave_Channel, uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length);
#endif
