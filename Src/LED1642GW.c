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
uint16_t ledConfigBuffer[DRIVER_NUM];
uint16_t ledArrayBuffer[CHANNEL_PER_DRIVER_NUM][DRIVER_NUM];


uint8_t LED1642GW_Init(void)
{
    uint8_t error_code;
    PWM_PWCLK_play(); //starts generating the brightness clock. 
    nrf_delay_ms(100);
    error_code = LED1642GW_Driver_Count();
    return error_code;
}

/* OMG I CAN SET THE RISE AND FALL TIME IN THE DRIVER!?? IM SO IN LOVE RN! */

/*
 * To Enter Low Power Mode(LPM), one needs to confgure out 0 to 15 to 0x0000.
 * configure CFG0...CFG5 = '111111'. 
 * configure CFG6 = '1'.
 * In this case, we also want to turn off the PWCLK.
 */
uint8_t LED1642GW_Enter_LPM(void)
{
    PWM_PWCLK_stop();
    int i;
    int tempBrighnessBufferCount = 0;
    /* Configure CFG0...CFG5 = '111111'. */
    for(i = 0; i < DRIVER_NUM; i++)
    {
        ledConfigBuffer[i] = 0x1400;   // 0b0000110001111111;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Write_Configuration_Register(ledConfigBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(ledConfigBuffer[i]);
        }
    }
    
    
    for(int i = 0; i < DRIVER_NUM; i++)
    {
        ledConfigBuffer[i] = 0x0000;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Switch(ledConfigBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(ledConfigBuffer[i]);
        }
        nrf_delay_ms(5);
    }
    
    return 1;
}

void LED1642GW_Test_WaveFormGeneration(nrf_pwm_values_common_t duty_cycle)
{
    LED_SPI_Transmit_LE_16(duty_cycle, 0xFFFF);
}

uint8_t LED1642GW_Driver_Count(void)
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
        return 1;
    }
    return 0;
}

void LED1642_LED_All_On(void)
{
    int i;
    int tempBrighnessBufferCount = 0;
    LED1642_Set_Brightness(0x0010);
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
    
    for(i = 0; i < DRIVER_NUM; i++)
    {
        txBuffer[i] = 0xFFFF;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Switch(txBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(txBuffer[i]);
        }
    }
}

void LED1642_LED_RGB_Train_Forward(void)
{
    int i, j;
    int tempBrighnessBufferCount = 0;
    LED1642_Set_Brightness(0x0010);
    for(i = 0; i < SPI_BUFFER_SIZE - DRIVER_NUM; i++)
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
            nrf_delay_ms(100);
        }
        
    }
}

void LED1642_LED_Different_Color_Same_Time(void)
{
    int i, j;
    int tempBrighnessBufferCount = 0;
    int tempRepeatCounter = 0;
    LED1642_Set_Brightness(0x007F);
    
    while(1)
    {   
        tempRepeatCounter = tempRepeatCounter + 0x00FF;
        if(tempRepeatCounter > 0xF000)
        {
            tempRepeatCounter = 0x1000;
        }
        for(i = 0; i < SPI_BUFFER_SIZE; i++)
        {
            txBuffer[i] = tempRepeatCounter;
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
            txBuffer[i] = tempRepeatCounter;
            if(i == DRIVER_NUM - 1)
            {
                LED1642GW_Global_Latch(txBuffer[i]);
            }
            else
            {
                LED_SPI_Transmit_16(txBuffer[i]);
            }
        }
        txBuffer[0] = 0x62D5;
        txBuffer[1] = 0x62D5;
        txBuffer[2] = 0x62D5;
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
}

void LED1642GW_Brightness_Control_Test(void)
{
    int i;
    int tempBrighnessBufferCount = 0;
    /* Setting the brightness. */
    LED1642_Set_Brightness(0x0010);
    while(1)
    {
        for(int j = 1; j < 200; j++)
        {
            for(i = 0; i < SPI_BUFFER_SIZE - DRIVER_NUM; i++)
            {
                if(j < 100)
                {
                    txBuffer[i] = j*600;
                }
                else
                {
                    txBuffer[i] = (200 - j)*600;
                }
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
                if(j < 100)
                {
                    txBuffer[i] = j*600;
                }
                else
                {
                    txBuffer[i] = (200 - j)*600;
                }
                if(i == DRIVER_NUM - 1)
                {
                    LED1642GW_Global_Latch(txBuffer[i]);
                }
                else
                {
                    LED_SPI_Transmit_16(txBuffer[i]);
                }
            }
            txBuffer[0] = 0xFFFF;
            txBuffer[1] = 0xFFFF;
            txBuffer[2] = 0xFFFF;
            for(i = 0; i < DRIVER_NUM; i++)
            {
                if(i == DRIVER_NUM - 1)
                {
                    LED1642GW_Switch(txBuffer[i]);
                }
                else
                {
                    LED_SPI_Transmit_16(txBuffer[i]);
                }
                nrf_delay_ms(2);
            }
        }
    }
}

void LED1642GW_RGB_Translation_Individual_Channel(uint8_t channel, rgb_led RGB_Color)
{
    int i;
    int tempBrighnessBufferCount = 0;
    /* Setting the brightness. */
    LED1642_Set_Brightness(0x0001);
    for(i = 0; i < SPI_BUFFER_SIZE - DRIVER_NUM; i++)
    {
        if(i == ((14 - channel) * 3))
        {
            txBuffer[i] = RGB_Color.b * 256;
        }
        else if(i == ((14 - channel) * 3 + 1))
        {
            txBuffer[i] = RGB_Color.g * 256;
        }
        else if(i == ((14 - channel) * 3 + 2))
        {
            txBuffer[i] = RGB_Color.r * 256;
        }
        else
        {
             txBuffer[i] = 0x0000;
        }
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
    __asm{NOP};
    for(i = 0; i < DRIVER_NUM; i++)
    {
        if(channel == 15)
        {
            if(i == 0)
            {
                txBuffer[45 + i] = RGB_Color.b * 256;
            }
            else if(i == 1)
            {
                txBuffer[45 + i] = RGB_Color.g * 256;
            }
            else if(i == 2)
            {
                txBuffer[45 + i] = RGB_Color.r * 256;
            }
        }
        else
        {
            txBuffer[45 + i] = 0x0000;
        }
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Global_Latch(txBuffer[45 + i]);
        }
        else
        {
            LED_SPI_Transmit_16(txBuffer[45 + i]);
        }
    }
    __asm{NOP};
    for(int i = 0; i < DRIVER_NUM; i++)
    {
        ledConfigBuffer[i] = 0xFFFF;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Switch(ledConfigBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(ledConfigBuffer[i]);
        }
        nrf_delay_ms(5);
    }
}

/*RGB_Color will be 16*/
void LED1642GW_RGB_Translation_Array(rgb_led * RGB_Color)
{
    int i, j;
    /* Setting the brightness. */
    LED1642_Set_Brightness(0x0001);
    
    for(i = 0; i < CHANNEL_PER_DRIVER_NUM - 1; i++)
    {
        ledArrayBuffer[15-i][0] = RGB_Color[i].b * 256;
        ledArrayBuffer[15-i][1] = RGB_Color[i].g * 256;
        ledArrayBuffer[15-i][2] = RGB_Color[i].r * 256;
    }
    ledArrayBuffer[0][0] = RGB_Color[15].b * 256;
    ledArrayBuffer[0][1] = RGB_Color[15].g * 256;
    ledArrayBuffer[0][2] = RGB_Color[15].r * 256;
    
    for(i = 0; i < CHANNEL_PER_DRIVER_NUM - 1; i++)
    {
        for(j = 0; j < DRIVER_NUM; j++)
        {
            if(j == 2)
            {
                LED1642GW_Data_Latch(ledArrayBuffer[i][j]);
            }
            else
            {
                LED_SPI_Transmit_16(ledArrayBuffer[i][j]);
            }
        }
    }
    __asm{NOP};
    for(i = 0; i < DRIVER_NUM; i++)
    {
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Global_Latch(ledArrayBuffer[15][i]);
        }
        else
        {
            LED_SPI_Transmit_16(ledArrayBuffer[15][i]);
        }
    }
    __asm{NOP};
    for(int i = 0; i < DRIVER_NUM; i++)
    {
        ledConfigBuffer[i] = 0xFFFF;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Switch(ledConfigBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(ledConfigBuffer[i]);
        }
        //nrf_delay_ms(2);
    }
}

void LED1642_Set_Brightness(uint16_t brightness)
{
    for(int i = 0; i < DRIVER_NUM; i++)
    {
        ledConfigBuffer[i] = brightness;
        if(i == DRIVER_NUM - 1)
        {
            LED1642GW_Write_Configuration_Register(ledConfigBuffer[i]);
        }
        else
        {
            LED_SPI_Transmit_16(ledConfigBuffer[i]);
        }
    }
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
