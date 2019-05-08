#include "SPI_Command_Handling.h"

uint8_t spi_tx_data[NUM_SPI_BYTES];
uint8_t spi_rx_data[NUM_SPI_BYTES];
extern volatile uint8_t spi_complete;
void Channel_Write(int channel, uint8_t cmd, int32_t tx_data)
{
    spi_tx_data[0] = cmd;
    Command_Parse(tx_data, &spi_tx_data[1]);
    SPI_Transmit(channel, spi_tx_data, NUM_SPI_BYTES);
    while(1)
    {
        if(spi_complete == 1)
            break;
    }
    return;
}

void Channel_Read(int channel, uint8_t cmd, int32_t tx_data, int32_t * rx_data)
{
    spi_tx_data[0] = cmd;
    Command_Parse(tx_data, &spi_tx_data[1]);
    SPI_Receive(channel, spi_tx_data, spi_rx_data, NUM_SPI_BYTES, NUM_SPI_BYTES);
    while(1)
    {
        if(spi_complete == 1)
            break;
    }
    *rx_data = (spi_rx_data[1]<<24) | (spi_rx_data[2] << 16) | (spi_rx_data[3] << 8) | spi_rx_data[4];
    return;
}

void Command_Parse(int32_t cmd_in, uint8_t * cmd_out)
{
    cmd_out[0] = (cmd_in & 0xFF000000) >> 24;
    cmd_out[1] = (cmd_in & 0x00FF0000) >> 16;
	cmd_out[2] = (cmd_in & 0x0000FF00) >> 8;
	cmd_out[3] = (cmd_in & 0x000000FF);
    return;
}


