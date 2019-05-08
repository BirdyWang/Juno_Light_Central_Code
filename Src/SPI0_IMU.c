/* SPI0_IMU.c
 * Purpose: This is the file that contains wrapper implementations for SPI0 instances.
 * Usage: IMU System. 
 * Author: Bird
 * Creation Date: 5/8/2019 
 */

#include "SPI0_IMU.h"

NRF_SPI_MNGR_DEF(imuSpiMngr, TRANSACTION_QUEUE_SIZE, IMU_SPI_INSTANCE);
static nrf_spi_mngr_transfer_t imuSpiTransferData;
volatile uint8_t imuSpiCompleteFlag = 0;
uint8_t imuSpiRx[20];

void IMU_SPI_Init(void)
{
    IMU_SPI_Manager_Init();
    nrf_gpio_cfg_output(IMU_SS);
    nrf_gpio_pin_write(IMU_SS, 1);
}
    
void IMU_SPI_Manager_Init(void)
{
    const nrf_drv_spi_config_t m_master0_config =
    {
        .sck_pin        = IMU_SCK,
        .mosi_pin       = IMU_MOSI,
        .miso_pin       = IMU_MISO,
        .ss_pin         = NRF_DRV_SPI_PIN_NOT_USED,
        .irq_priority   = APP_IRQ_PRIORITY_MID,
        .orc            = 0xAA,
        .frequency      = NRF_DRV_SPI_FREQ_250K,
        .mode           = NRF_DRV_SPI_MODE_3,
        .bit_order      = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
    };
    nrf_spi_mngr_init(&imuSpiMngr, &m_master0_config);
}

void IMU_SPI_Master_Uninit(void)
{
    nrf_spi_mngr_uninit(&imuSpiMngr);
    nrf_gpio_cfg_default(IMU_SCK);
    nrf_gpio_cfg_default(IMU_MOSI);
    nrf_gpio_cfg_default(IMU_MISO);
    nrf_gpio_cfg_default(IMU_SS);
}

void IMU_Start_Transaction(void * p_user_data)
{
    nrf_gpio_pin_write(IMU_SS, 0);
    imuSpiCompleteFlag = 0;
}

void IMU_Stop_Transaction(ret_code_t result, void * p_user_data)
{
    nrf_gpio_pin_write(IMU_SS, 1);
    imuSpiCompleteFlag = 1;
}

void IMU_Stop_Transaction_DMP(ret_code_t result, void * p_user_data)
{
    imuSpiCompleteFlag = 1;
}

uint32_t IMU_SPI_Transmit(uint8_t * Tx_data, uint8_t tx_data_length)
{
    imuSpiCompleteFlag = 0;
    
    imuSpiTransferData.p_tx_data = Tx_data;
    imuSpiTransferData.tx_length = tx_data_length;
    imuSpiTransferData.p_rx_data = NULL;
    imuSpiTransferData.rx_length = 0;
    static nrf_spi_mngr_transaction_t transaction = 
    {
        .begin_callback = IMU_Start_Transaction,
        .end_callback = IMU_Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &imuSpiTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&imuSpiMngr, &transaction);
    while(1)
    {
        if(imuSpiCompleteFlag == 1)
            return 1;
    }
}

uint32_t IMU_SPI_Transmit_DMP_Write(uint8_t * Tx_reg, uint8_t * Tx_data, uint8_t tx_data_length)
{
    imuSpiCompleteFlag = 0;
    
    imuSpiTransferData.p_tx_data = Tx_reg;
    imuSpiTransferData.tx_length = 1;
    imuSpiTransferData.p_rx_data = NULL;
    imuSpiTransferData.rx_length = 0;
    nrf_spi_mngr_transaction_t transaction1 = 
    {
        .begin_callback = IMU_Start_Transaction,
        .end_callback = IMU_Stop_Transaction_DMP,
        .p_user_data = NULL,
        .p_transfers = &imuSpiTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };
    nrf_spi_mngr_schedule(&imuSpiMngr, &transaction1);
    while(1)
    {
        if(imuSpiCompleteFlag == 1)
            break;
    }
    
    imuSpiCompleteFlag = 0;
    imuSpiTransferData.p_tx_data = Tx_data;
    imuSpiTransferData.tx_length = tx_data_length;
    imuSpiTransferData.p_rx_data = imuSpiRx;
    imuSpiTransferData.rx_length = tx_data_length;
    nrf_spi_mngr_transaction_t transaction2 = {
        .begin_callback = NULL,
        .end_callback = IMU_Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &imuSpiTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&imuSpiMngr, &transaction2);
    while(1)
    {
        if(imuSpiCompleteFlag == 1)
            return 1;
    }
}

uint32_t IMU_SPI_Receive(uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length)
{
    imuSpiCompleteFlag = 0;
    
    imuSpiTransferData.p_tx_data = Tx_data;
    imuSpiTransferData.tx_length = tx_data_length;
    
    imuSpiTransferData.p_rx_data = Rx_data;
    imuSpiTransferData.rx_length = rx_data_length;
    
    static nrf_spi_mngr_transaction_t transaction = 
    {
        .begin_callback = IMU_Start_Transaction,
        .end_callback = IMU_Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &imuSpiTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&imuSpiMngr, &transaction);
    
    while(1)
    {
        if(imuSpiCompleteFlag == 1)
            return 1;
    }
}
uint32_t IMU_SPI_Receive_DMP_Read(uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length)
{
    imuSpiCompleteFlag = 0;
    
    imuSpiTransferData.p_tx_data = Tx_data;
    imuSpiTransferData.tx_length = tx_data_length;
    imuSpiTransferData.p_rx_data = NULL;
    imuSpiTransferData.rx_length = 0;
    nrf_spi_mngr_transaction_t transaction1 = 
    {
        .begin_callback = IMU_Start_Transaction,
        .end_callback = IMU_Stop_Transaction_DMP,
        .p_user_data = NULL,
        .p_transfers = &imuSpiTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };
    nrf_spi_mngr_schedule(&imuSpiMngr, &transaction1);
    while(1)
    {
        if(imuSpiCompleteFlag == 1)
            break;
    }
    imuSpiCompleteFlag = 0;
    imuSpiTransferData.p_tx_data = NULL;
    imuSpiTransferData.tx_length = rx_data_length;
    imuSpiTransferData.p_rx_data = Rx_data;
    imuSpiTransferData.rx_length = rx_data_length;
    
    nrf_spi_mngr_transaction_t transaction2 = {
        .begin_callback = NULL,
        .end_callback = IMU_Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &imuSpiTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&imuSpiMngr, &transaction2);
    while(1)
    {
        if(imuSpiCompleteFlag == 1)
            return 1;
    }
}
