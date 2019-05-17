/* LED1642GW.c
 * Purpose: This is the file that implements base driver for LED1642 LED driver IC
 * Usage: LED driver system
 * Author: Bird
 * Creation Date: 5/12/2019 
 */

#include "LED1642GW.h"

uint8_t spiTxBuffer[4];
uint8_t spiRxBuffer[2];
uint16_t txBuffer[SPI_BUFFER_SIZE];
uint16_t rxBuffer[SPI_BUFFER_SIZE];

void LED1642GW_Test_WaveFormGeneration(nrf_pwm_values_common_t duty_cycle)
{
    //PWM_play(duty_cycle);
    //LED_SPI_Transmit_16(0xFFFF);
    LED_SPI_Transmit_LE_16(duty_cycle, 0xFFFF);
}

void LED1642GW_Driver_Count(void)
{
    uint8_t driverCount = 0;
    spiTxBuffer[0] = 0x00;
    spiTxBuffer[1] = 0x00;
    for(int i = 0; i < DRIVER_NUM; i++)
    {
        LED_SPI_Transmit(spiTxBuffer, 2);
    }
    spiTxBuffer[0] = 23;
    spiTxBuffer[1] = 24;
    for(int i = 0; i < DRIVER_NUM+1; i++)
    {
        LED_SPI_Receive(spiTxBuffer, spiRxBuffer, 2, 2);
        spiTxBuffer[0] = 0xFF;
        spiTxBuffer[1] = 0xFF;
        if((spiRxBuffer[0] == 23) && (spiRxBuffer[1] == 24))
        {
            driverCount = i;
            __asm{NOP};
        }

    }
    if(driverCount == DRIVER_NUM)
    {
            __asm{NOP};
    }
}

void LED1642_LED_All_On(void)
{
    int i, j;
    int tempBrighnessBufferCount = 0;
    for(i = 0; i < DRIVER_NUM; i++)
    {
        txBuffer[i] = 0x0010;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Write_Configuration_Register(txBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(txBuffer[i]);
        }
    }
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        txBuffer[i] = 0xFFFF;
        if(tempBrighnessBufferCount == (DRIVER_NUM - 1))
        {
            LED1642GW_Data_Latch(txBuffer[i]); 
            tempBrighnessBufferCount = 0;
        }
        else
        {
            LED_SPI_Transmit_16(txBuffer[i]);
            tempBrighnessBufferCount++;
        }
    }
    for(i = 0; i < DRIVER_NUM; i++)
    {
        txBuffer[i] = 0xFFFF;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Global_Latch(txBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(txBuffer[i]);
        }
    }
    
    while(1)
    {
        txBuffer[0] = 0x0001;
        txBuffer[1] = 0x0002;
        txBuffer[2] = 0x0004;
        for(j = 0; j < CHANNEL_PER_DRIVER_NUM; j++)
        {
            for(i = 0; i < DRIVER_NUM; i++)
            {
                
                if(i == DRIVER_NUM - 1)
                {
                    LED1642GW_Switch(txBuffer[i]);
                    txBuffer[i] = txBuffer[i] << 1;
                }
                else
                {
                    LED_SPI_Transmit_16(txBuffer[i]);
                    txBuffer[i] = txBuffer[i] << 1;
                }
                nrf_delay_ms(2);
                
            }
            nrf_delay_ms(10);
        }
        
    }
    
    /*
    for(i = 0; i < DRIVER_NUM; i++)
    {
        txBuffer[i] = 0x8000;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Switch(txBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(txBuffer[i]);
        }
    }*/
    
}

void LED1642GW_Write_Configuration_Register(uint16_t tempData)
{
    LED_SPI_Transmit_LE_16(WRITE_CONFIGURATION_REGISTER, tempData);
}

void LED1642GW_Data_Latch(uint16_t tempData)
{ 
    LED_SPI_Transmit_LE_16(BRIGHTNESS_DATA_LATCH, tempData);
}

void LED1642GW_Global_Latch(uint16_t tempData)
{ 
    LED_SPI_Transmit_LE_16(BRIGHTNESS_GLOBAL_LATCH, tempData);
}

void LED1642GW_Switch(uint16_t tempData)
{ 
    LED_SPI_Transmit_LE_16(WRITE_SWITCH, tempData);
}
