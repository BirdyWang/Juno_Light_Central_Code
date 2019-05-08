/* SPI0_IMU.h
 * Author: Bird
 * Creation Date: 5/8/2019 
 */
#ifndef SPI0_IMU_H
#define SPI0_IMU_H

#include "Juno_Light_Central_Pin_Map.h"
#include "nrf_spi_mngr.h"

#define TRANSACTION_QUEUE_SIZE  3
#define IMU_SPI_INSTANCE        0

void IMU_SPI_Init(void);
void IMU_SPI_Manager_Init(void);
void IMU_SPI_Master_Uninit(void);
void IMU_Start_Transaction(void * p_user_data);
void IMU_Stop_Transaction(ret_code_t result, void * p_user_data);
void IMU_Stop_Transaction_DMP(ret_code_t result, void * p_user_data);
uint32_t IMU_SPI_Transmit(uint8_t * Tx_data, uint8_t tx_data_length);
uint32_t IMU_SPI_Transmit_DMP_Write(uint8_t * Tx_reg, uint8_t * Tx_data, uint8_t tx_data_length);
uint32_t IMU_SPI_Receive(uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length);
uint32_t IMU_SPI_Receive_DMP_Read(uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length);

#endif
