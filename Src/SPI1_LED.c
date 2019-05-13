/* SPI1_LED.c
 * Purpose: This is the file that contains wrapper implementations for SPI1 instances.
 * Usage: LED System. 
 * Author: Bird
 * Creation Date: 5/8/2019 
 */

#include "SPI1_LED.h"

NRF_SPI_MNGR_DEF(ledSpiMngr, TRANSACTION_QUEUE_SIZE, LED_SPI_INSTANCE);
static nrf_spi_mngr_transfer_t ledSpitTransferData;
volatile uint8_t ledSpiCompleteFlag = 0;

void LED_SPI_Init(void)
{
    LED_SPI_Manager_Init();
}

void LED_SPI_Manager_Init(void)
{
    const nrf_drv_spi_config_t m_master1_config =
    {
        .sck_pin        = LED_SCK,
        .mosi_pin       = LED_MOSI,
        .miso_pin       = LED_MISO,
        .ss_pin         = NRF_DRV_SPI_PIN_NOT_USED,
        .irq_priority   = APP_IRQ_PRIORITY_MID,
        .orc            = 0xAA,
        .frequency      = NRF_DRV_SPI_FREQ_250K,
        .mode           = NRF_SPI_MODE_0,
        .bit_order      = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
    };
    nrf_spi_mngr_init(&ledSpiMngr, &m_master1_config);
}

void LED_SPI_Master_Uninit(void)
{
    nrf_spi_mngr_uninit(&ledSpiMngr);
    nrf_gpio_cfg_default(LED_SCK);
    nrf_gpio_cfg_default(LED_MOSI);
    nrf_gpio_cfg_default(LED_MISO);
}

void LED_Start_Transaction(void * p_user_data)
{
    //We are going to set the duty cycle of the LE pin and enable PWM generation.
    ledSpiCompleteFlag = 0;
}

void LED_Stop_Transaction(ret_code_t result, void * p_user_data)
{
    //We are going to stop the PWM generation
    ledSpiCompleteFlag = 1;
}

/*TODO: create a wrapper for 16bit data function*/

uint32_t LED_SPI_Transmit(uint8_t * Tx_data, uint8_t tx_data_length)
{
    ledSpiCompleteFlag = 0;
    
    ledSpitTransferData.p_tx_data = Tx_data;
    ledSpitTransferData.tx_length = tx_data_length;
    ledSpitTransferData.p_rx_data = NULL;
    ledSpitTransferData.rx_length = 0;
    static nrf_spi_mngr_transaction_t transaction = 
    {
        .begin_callback = LED_Start_Transaction,
        .end_callback = LED_Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &ledSpitTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&ledSpiMngr, &transaction);
    while(1)
    {
        if(ledSpiCompleteFlag == 1)
            return 1;
    }
}

uint32_t LED_SPI_Receive(uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length)
{
    ledSpiCompleteFlag = 0;
    
    ledSpitTransferData.p_tx_data = Tx_data;
    ledSpitTransferData.tx_length = tx_data_length;
    
    ledSpitTransferData.p_rx_data = Rx_data;
    ledSpitTransferData.rx_length = rx_data_length;
    
    static nrf_spi_mngr_transaction_t transaction = 
    {
        .begin_callback = LED_Start_Transaction,
        .end_callback = LED_Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &ledSpitTransferData,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&ledSpiMngr, &transaction);
    
    while(1)
    {
        if(ledSpiCompleteFlag == 1)
            return 1;
    }
}
