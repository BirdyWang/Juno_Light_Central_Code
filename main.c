#include "Juno_Light_Central_Config.h"
#include "Juno_Light_Central_Pin_Map.h"
#include "Juno_BLE.h"
#include "GPIO.h"
#include "SAADC.h"
#include "PWM.h"
#include "Command_Handling.h"
#include "MPU6500.h"
#include "LED1642GW.h"
#include "ble_dfu.h"
#include "Euler_Angle.h"
#define INJECT_I2C_COMMANDS

#define SLEEP_TIMEOUT_VALUE 2000

#define INTERRUPT_TEST      0
#define BLE_ENABLE          1
#define UART_DEBUGGING      0

extern uint8_t BLE_cmd[3];
extern volatile uint8_t imuNewGyroFlag;
extern volatile uint8_t powerOnFlag;
extern volatile uint8_t batteryChargingFlag;
extern volatile uint8_t ledDisplayMode;
extern volatile uint8_t setupCompleteFlag;
extern volatile uint8_t driveTrainBreakFlag;
extern volatile uint8_t swToUSBPwr;
extern short gyro[3];
extern short accel[3];
extern short sensors;
extern unsigned char more;
extern long quat[4];

uint8_t deviceInitFlag = 0;

rgb_led rgbSingleColorDisplay[16];
rgb_led rgbDmpMappingDisplay[16];
rgb_led rgbTrainForwardDisplay[16];
rgb_led rgbBatteryDisplay[16];
rgb_led rgbOTAUpdateDisplay[16];
    
enum packet_type_e {
    PACKET_TYPE_ACCEL,
    PACKET_TYPE_GYRO,
    PACKET_TYPE_QUAT,
    PACKET_TYPE_TAP,
    PACKET_TYPE_ANDROID_ORIENT,
    PACKET_TYPE_PEDO,
    PACKET_TYPE_MISC
};

uint32_t uart_activity_ts;

void HardFault_Handler(void)
{
    NVIC_SystemReset();
}

#if (EN_PWR_MANAGE == 1)
/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}
#endif

static void Enter_Low_Power_Mode(void)
{
    Timer2_Disable();
    mpu_sleep();
    LED1642GW_Enter_LPM();
}
/*
 * APP_ADV_TIMEOUT_IN_SECONDS is the reason why the NRF chip reboot itself.
 * When the timeout is triggered, on_adv_evt is called with BLE_ADV_EVT_IDLE case.
 * BLE_ADV_EVT_IDLE is handled through setting up indications and reset the chip. 
 * The current indication should avoid that and put the 
 **/
/**@brief Application main function.
 */
int main(void)
{
    __asm{NOP};
    /* Define private variables. */
    
    uint8_t channel;
    uint8_t channelHistory;
    rgb_led colorHistory;
    nrf_saadc_value_t batteryVoltageRaw;
    float batteryVoltageFloat;
    rgb_led color;
    Quaternion convertedQuat;
    EulerAngle eulerAngle;
    int i,j;
    uint8_t ledBrightnessOffset;
    volatile ret_code_t err_code;
    float halTime;
    //err_code = ble_dfu_buttonless_async_svci_init();
    //APP_ERROR_CHECK(err_code);
    
    /*Initialize peripherals*/
    timers_init();
    Timer2_Init();
    
    PWM_init();
    PWM_PWCLK_init();
    PWM_PWCLK_play();
#if (UART_DEBUGGING == 1)
    UART_Init();
#endif
    LED_SPI_Init();
    IMU_SPI_Init();
    SAADC_Init();
    
    LED1642GW_Init();
    LED1642GW_Driver_Count();
    
    for(i = 0; i < 16; i++)
    {
        rgbSingleColorDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
        rgbDmpMappingDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
        rgbTrainForwardDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
        rgbBatteryDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
        rgbOTAUpdateDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
    }
    LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);

#if (BLE_ENABLE == 1)    
    BLE_Init();
    //
#endif    
    GPIO_Init();
    
    mpu_reset();
    nrf_delay_ms(5000);
    /*Say when the battery is depleted and user to recharge the chip, It will wake up and display the light. */
    powerOnFlag = 1;
    while(1)
    {
        if((powerOnFlag == 1) || (swToUSBPwr == 1))
        {
            if(deviceInitFlag == 0)
            {
                for(i = 0; i < 16; i++)
                {
                    rgbSingleColorDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 255};
                }
                LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
                MPU6500_Setup(); // Right now it only has tapping interrupt enabled. 
                MPU6500_Enable_DMP();
                PWM_PWCLK_play();
                nrf_delay_ms(10);
                LED1642GW_Driver_Count();
                ledDisplayMode = SINGLE_COLOR_DISPLAY_BATTERY;
                Timer2_Enable();
                for(i = 0; i < 16; i++)
                {
                    rgbSingleColorDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
                }
                LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
                deviceInitFlag = 1;
            }
            else
            {
                switch(ledDisplayMode)
                {
                    case SINGLE_COLOR_DISPLAY_BATTERY:
                        batteryVoltageFloat = 0;
                        for(i = 0; i < 5; i++)
                        {
                            batteryVoltageRaw = SAADC_Convert();
                            batteryVoltageFloat += batteryVoltageRaw * BATTERY_CONV_RATIO;
                        }
                        batteryVoltageFloat = floor(batteryVoltageFloat*10)/50.0f;
                        for(i = 0; i < 16; i++)
                        {
                            if((batteryVoltageFloat >= 4.19f))
                            {
                                rgbBatteryDisplay[i] = (rgb_led){.r = 0, .g = 100, .b = 0};
                            }
                            else if(batteryVoltageFloat <= 4.15f)
                            {
                                ledBrightnessOffset = 125 * batteryVoltageFloat - 425;
                                rgbBatteryDisplay[i] = (rgb_led){.r = 255 - ledBrightnessOffset, .g = ledBrightnessOffset, .b = 0};
                            }
                        }
                        LED1642GW_RGB_Translation_Array(rgbBatteryDisplay);
                        nrf_delay_ms(500);
                        break;
                    case SINGLE_COLOR_DISPLAY_YELLOW:
                        if(setupCompleteFlag == 0)
                        {
                            for(j = 0; j < 2; j++)
                            {
                                for(i = 0; i < 16; i++)
                                {
                                    rgbSingleColorDisplay[i] = (rgb_led){.r = 255, .g = 255, .b = 0};
                                }
                                LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
                            }
                            setupCompleteFlag = 1;
                        }
                        break;
                    case SINGLE_COLOR_DISPLAY_RED:
                        if(setupCompleteFlag == 0)
                        {
                            for(j = 0; j < 2; j++)
                            {
                                for(i = 0; i < 16; i++)
                                {
                                    rgbSingleColorDisplay[i] = (rgb_led){.r = 255, .g = 0, .b = 0};
                                }
                                LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
                            }
                            setupCompleteFlag = 1;
                        }
                        break;
                    case SINGLE_COLOR_DISPLAY_GREEN:
                        if(setupCompleteFlag == 0)
                        {
                            for(j = 0; j < 2; j++)
                            {
                                for(i = 0; i < 16; i++)
                                {
                                    rgbSingleColorDisplay[i] = (rgb_led){.r = 0, .g = 255, .b = 0};
                                }
                                LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
                            }
                            setupCompleteFlag = 1;
                        }
                        break;
                    case SINGLE_COLOR_DISPALY_BLUE:
                        if(setupCompleteFlag == 0)
                        {
                            for(j = 0; j < 2; j++)
                            {
                                for(i = 0; i < 16; i++)
                                {
                                    rgbSingleColorDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 255};
                                }
                                LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
                            }
                            setupCompleteFlag = 1;
                        }
                        break;
                    case SINGLE_COLOR_DISPLAY_ORANGE:
                        if(setupCompleteFlag == 0)
                        {
                            for(j = 0; j < 2; j++)
                            {
                                for(i = 0; i < 16; i++)
                                {
                                    rgbSingleColorDisplay[i] = (rgb_led){.r = 255, .g = 60, .b = 0};
                                }
                                LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
                            }
                            setupCompleteFlag = 1;
                        }
                        break;
                    case MOTION_MAP_DISPLAY:
                        if(imuNewGyroFlag == 1)
                        {
                            ConvertQuaternion(quat, &convertedQuat);
                            QuatToEulerAngle(&convertedQuat, &eulerAngle);
                            channelHistory = channel;
                            colorHistory = color;
                            EulerAngleToLED(&eulerAngle, &channel, &color);
                            if((channel != channelHistory) || (color.r != colorHistory.r) || (color.g != colorHistory.g))
                            {
                                for(i = 0; i < 16; i++)
                                {
                                    rgbDmpMappingDisplay[i] = (rgb_led){.r = 10, .g = 5, .b = 0};
                                }
                                rgbDmpMappingDisplay[channel] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                                if(channel == 0)
                                {
                                    rgbDmpMappingDisplay[1] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                                    rgbDmpMappingDisplay[15] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                                }
                                else if(channel == 15)
                                {
                                    rgbDmpMappingDisplay[0] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                                    rgbDmpMappingDisplay[14] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};            
                                }
                                else
                                {
                                    rgbDmpMappingDisplay[channel+1] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                                    rgbDmpMappingDisplay[channel-1] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                                }
                                LED1642GW_RGB_Translation_Array(rgbDmpMappingDisplay);
                            }
#if (UART_DEBUGGING == 1)
                            printf("Euler Angles: roll: %f, red: %u, pitch: %f, green: %u\r",eulerAngle.roll*180/PI, color.r, eulerAngle.pitch*180/PI, color.g);
#endif
                            imuNewGyroFlag = 0;
                        }
                        break;
                    case COLOR_TRANSITION:
                        
                        for(j = 0; j < 16; j++)
                        {
                            for(i = 0; i < 16; i++)
                            {
                                if(driveTrainBreakFlag == 1)
                                {
                                    i = j = 100;
                                    driveTrainBreakFlag = 0;
                                    break;
                                }
                                else
                                {
                                    if(i <= j)
                                    {
                                        rgbTrainForwardDisplay[j-i] = (rgb_led){.r = 0 + (15-i)*15, .g = 120-(15-i)*8, .b = 0};
                                    }
                                    else
                                    {
                                        rgbTrainForwardDisplay[i+j] = (rgb_led){.r = 0 + i*15, .g = 120-i*8, .b = 0};
                                    }
                                }
                            }
                            LED1642GW_RGB_Translation_Array(rgbTrainForwardDisplay);
                            nrf_delay_ms(500);
                        }
                        
                        break;
                    case OTA_UPDATE:
                        halTime = ((float)HAL_GetTick())*.0001f;
                        rgbOTAUpdateDisplay[0].g = (255.0f*(sin(halTime * TWO_PI + TWO_PI * .333333f) * .5f + .5f));
                        rgbOTAUpdateDisplay[0].r = (255.0f*(sin(halTime * TWO_PI + TWO_PI * .666666f) * .5f + .5f));
                        rgbOTAUpdateDisplay[0].b = (255.0f*(sin(halTime * TWO_PI) * .5f + .5f));;
                        for(i = 1; i < 16; i++)
                        {
                            rgbOTAUpdateDisplay[i] = (rgb_led){.r = rgbOTAUpdateDisplay[0].r, .g = rgbOTAUpdateDisplay[0].g, .b = rgbOTAUpdateDisplay[0].b};
                        }
                        LED1642GW_RGB_Translation_Array(rgbOTAUpdateDisplay);
                        nrf_delay_ms(100);
                        break;
                    case COS_DISPLAY:
                        halTime = ((float)HAL_GetTick())*.0004f;
                        rgbOTAUpdateDisplay[0].g = (255.0f*(cos(halTime * TWO_PI + TWO_PI * .333333f) * .5f + .5f));
                        rgbOTAUpdateDisplay[0].r = (255.0f*(cos(halTime * TWO_PI + TWO_PI * .666666f) * .5f + .5f));
                        rgbOTAUpdateDisplay[0].b = (255.0f*(cos(halTime * TWO_PI) * .5f + .5f));;
                        for(i = 1; i < 16; i++)
                        {
                            rgbOTAUpdateDisplay[i] = (rgb_led){.r = rgbOTAUpdateDisplay[0].r, .g = rgbOTAUpdateDisplay[0].g, .b = rgbOTAUpdateDisplay[0].b};
                        }
                        LED1642GW_RGB_Translation_Array(rgbOTAUpdateDisplay);
                        nrf_delay_ms(100);
                        break;
                    case SIN_PLUS_COS_DISPLAY:
                        halTime = ((float)HAL_GetTick())*.0006f;
                        rgbOTAUpdateDisplay[0].g = (255.0f*(cos(halTime * TWO_PI + TWO_PI * .333333f) * .5f + .5f));
                        rgbOTAUpdateDisplay[0].r = (255.0f*(sin(halTime * TWO_PI + TWO_PI * .666666f) * .5f + .5f));
                        rgbOTAUpdateDisplay[0].b = (255.0f*(sin(halTime * TWO_PI) * .5f + .5f));;
                        for(i = 1; i < 16; i++)
                        {
                            rgbOTAUpdateDisplay[i] = (rgb_led){.r = rgbOTAUpdateDisplay[0].r, .g = rgbOTAUpdateDisplay[0].g, .b = rgbOTAUpdateDisplay[0].b};
                        }
                        LED1642GW_RGB_Translation_Array(rgbOTAUpdateDisplay);
                        nrf_delay_ms(100);
                        break;
                    default:
                        break;
                }
                if(batteryChargingFlag == 1)
                {
                    powerOnFlag = 0;
                }
            }
        }
        /* For charging status: sample SAADC and based on charged voltage and batteryChargingFlag display color. */
        else if(batteryChargingFlag == 1)
        {
            for(i = 0; i < 16; i++)
            {
                rgbSingleColorDisplay[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
            }
            LED1642GW_RGB_Translation_Array(rgbSingleColorDisplay);
            Enter_Low_Power_Mode();
            deviceInitFlag = 0;
            __WFI();
        }
        else
        {
            Enter_Low_Power_Mode();
            deviceInitFlag = 0;
            __WFI();
        }
    }
}
