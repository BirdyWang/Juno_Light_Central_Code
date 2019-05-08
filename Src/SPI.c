#include "SPI.h"
NRF_SPI_MNGR_DEF(spi_mngr, TRANSACTION_QUEUE_SIZE, SPI_INSTANCE);
static nrf_spi_mngr_transfer_t transfer_data;
volatile uint8_t spi_complete = 0;
uint32_t Slave_GPIO;
uint32_t SPI_Slave_Selects[7] = {SS_CH1, SS_CH2, SS_CH3, SS_CH4, SS_CH5, SS_CH6, SS_CH7};
uint8_t spi_rx[20];
void SPI_Init(void)
{
    SPI_Manager_Init();
    for(int i = 0; i < 7; i++)
    {
        nrf_gpio_cfg_output(SPI_Slave_Selects[i]);
        nrf_gpio_pin_write(SPI_Slave_Selects[i], 1);
    }
}
    
void SPI_Manager_Init(void)
{
    const nrf_drv_spi_config_t m_master0_config =
    {
        .sck_pin        = SPI_CLK,
        .mosi_pin       = SPI_MOSI,
        .miso_pin       = SPI_MISO,
        .ss_pin         = NRF_DRV_SPI_PIN_NOT_USED,
        .irq_priority   = APP_IRQ_PRIORITY_MID,
        .orc            = 0xAA,
        .frequency      = NRF_SPI_FREQ_250K,
        .mode           = NRF_DRV_SPI_MODE_3,
        .bit_order      = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
    };
    nrf_spi_mngr_init(&spi_mngr, &m_master0_config);
}

void SPI_Master_Uninit(void)
{
    nrf_spi_mngr_uninit(&spi_mngr);
    //nrf_gpio_cfg_default(SPI_CLK);
    //nrf_gpio_cfg_default(SPI_MOSI);
    //nrf_gpio_cfg_default(SPI_MISO);
    //nrf_gpio_cfg_default(SS_CH1);
}

void Start_Transaction(void * p_user_data)
{
    nrf_gpio_pin_write(Slave_GPIO, 0);
    spi_complete = 0;
}

void Stop_Transaction(ret_code_t result, void * p_user_data)
{
    nrf_gpio_pin_write(Slave_GPIO, 1);
    spi_complete = 1;
}

void Stop_Transaction2(ret_code_t result, void * p_user_data)
{
    //nrf_gpio_pin_write(Slave_GPIO, 1);
    spi_complete = 1;
}

uint32_t SPI_Transmit(uint32_t Slave_Channel, uint8_t * Tx_data, uint8_t tx_data_length)
{
    spi_complete = 0;
    Slave_GPIO = SPI_Slave_Selects[Slave_Channel];
    
    transfer_data.p_tx_data = Tx_data;
    transfer_data.tx_length = tx_data_length;
    transfer_data.p_rx_data = NULL;
    transfer_data.rx_length = 0;
    static nrf_spi_mngr_transaction_t transaction = 
    {
        .begin_callback = Start_Transaction,
        .end_callback = Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &transfer_data,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&spi_mngr, &transaction);
    while(1)
    {
        if(spi_complete == 1)
            return 1;
    }
}

uint32_t SPI_Transmit2(uint32_t Slave_Channel, uint8_t * Tx_reg, uint8_t * Tx_data, uint8_t tx_data_length)
{
    spi_complete = 0;
    Slave_GPIO = SPI_Slave_Selects[Slave_Channel];
    
    transfer_data.p_tx_data = Tx_reg;
    transfer_data.tx_length = 1;
    transfer_data.p_rx_data = NULL;
    transfer_data.rx_length = 0;
    nrf_spi_mngr_transaction_t transaction1 = 
    {
        .begin_callback = Start_Transaction,
        .end_callback = Stop_Transaction2,
        .p_user_data = NULL,
        .p_transfers = &transfer_data,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };
    nrf_spi_mngr_schedule(&spi_mngr, &transaction1);
    while(1)
    {
        if(spi_complete == 1)
            break;
    }
    
    spi_complete = 0;
    transfer_data.p_tx_data = Tx_data;
    transfer_data.tx_length = tx_data_length;
    transfer_data.p_rx_data = spi_rx;
    transfer_data.rx_length = tx_data_length;
    nrf_spi_mngr_transaction_t transaction2 = {
        .begin_callback = NULL,
        .end_callback = Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &transfer_data,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&spi_mngr, &transaction2);
    while(1)
    {
        if(spi_complete == 1)
            return 1;
    }
}

uint32_t SPI_Receive(uint32_t Slave_Channel, uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length)
{
    spi_complete = 0;
    Slave_GPIO = SPI_Slave_Selects[Slave_Channel];
    
    transfer_data.p_tx_data = Tx_data;
    transfer_data.tx_length = tx_data_length;
    
    transfer_data.p_rx_data = Rx_data;
    transfer_data.rx_length = rx_data_length;
    
    static nrf_spi_mngr_transaction_t transaction = 
    {
        .begin_callback = Start_Transaction,
        .end_callback = Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &transfer_data,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&spi_mngr, &transaction);
    
    while(1)
    {
        if(spi_complete == 1)
            return 1;
    }
}
uint32_t SPI_Receive2(uint32_t Slave_Channel, uint8_t * Tx_data, uint8_t * Rx_data, uint8_t tx_data_length, uint8_t rx_data_length)
{
    spi_complete = 0;
    Slave_GPIO = SPI_Slave_Selects[Slave_Channel];
    
    transfer_data.p_tx_data = Tx_data;
    transfer_data.tx_length = tx_data_length;
    transfer_data.p_rx_data = NULL;
    transfer_data.rx_length = 0;
    nrf_spi_mngr_transaction_t transaction1 = 
    {
        .begin_callback = Start_Transaction,
        .end_callback = Stop_Transaction2,
        .p_user_data = NULL,
        .p_transfers = &transfer_data,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };
    nrf_spi_mngr_schedule(&spi_mngr, &transaction1);
    while(1)
    {
        if(spi_complete == 1)
            break;
    }
    spi_complete = 0;
    transfer_data.p_tx_data = NULL;
    transfer_data.tx_length = rx_data_length;
    transfer_data.p_rx_data = Rx_data;
    transfer_data.rx_length = rx_data_length;
    
    nrf_spi_mngr_transaction_t transaction2 = {
        .begin_callback = NULL,
        .end_callback = Stop_Transaction,
        .p_user_data = NULL,
        .p_transfers = &transfer_data,
        .number_of_transfers = 1,
        .p_required_spi_cfg = NULL
    };

    nrf_spi_mngr_schedule(&spi_mngr, &transaction2);
    while(1)
    {
        if(spi_complete == 1)
            return 1;
    }
}
