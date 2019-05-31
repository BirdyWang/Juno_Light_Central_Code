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

#define MAIN_CODE           1
#define MPU6500_TEST        0
#define LED_TEST            0
#define INTERRUPT_TEST      0
#define BLE_ENABLE          0
#define UART_DEBUGGING      0

unsigned int led_ts = 0;

extern uint8_t BLE_cmd[3];
extern volatile uint8_t imuNewGyroFlag;
extern volatile uint8_t powerOnFlag;
extern volatile uint8_t batteryChargingFlag;
uint8_t deviceInitFlag = 0;
uint8_t ledDisplayMode = 0;

enum LED_display_modes {
    SINGLE_COLOR_DISPLAY,
    MOTION_MAP_DISPLAY,
    COLOR_TRANSITION,
    INCREASE_BRIGHTNESS,
    DECREASE_BRIGHTNESS
};
    
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
    rgb_led rgbTestArray[16];
    uint8_t channel;
    uint8_t channelHistory;
    rgb_led colorHistory;
    short gyro[3], accel[3], sensors;
    unsigned char more;
    long quat[4];
    rgb_led color;
    Quaternion convertedQuat;
    EulerAngle eulerAngle;
    
    volatile ret_code_t err_code;
    //err_code = ble_dfu_buttonless_async_svci_init();
    //APP_ERROR_CHECK(err_code);
    
    /*Initialize peripherals*/
    timers_init();
    GPIO_Init();
    PWM_init();
    PWM_PWCLK_init();
    PWM_PWCLK_play();
#if (UART_DEBUGGING == 1)
    UART_Init();
#endif
    LED_SPI_Init();
    IMU_SPI_Init();
    //SAADC_Init();
    
    LED1642GW_Init();
    LED1642GW_Driver_Count();
    
    for(int i = 0; i < 16; i++)
    {
        rgbTestArray[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
    }
    LED1642GW_RGB_Translation_Array(rgbTestArray);
    SAADC_Init();

#if (BLE_ENABLE == 1)    
    BLE_Init();
    //advertising_start();
#endif    
    
#if (MAIN_CODE == 1)
    mpu_reset();
    while(1)
    {
        if(powerOnFlag == 1)
        {
            if(deviceInitFlag == 0)
            {
                MPU6500_Setup();
                MPU6500_Enable_DMP();
                PWM_PWCLK_play();
                nrf_delay_ms(10);
                LED1642GW_Driver_Count();
                ledDisplayMode = MOTION_MAP_DISPLAY;
                deviceInitFlag = 1;
            }
            else
            {
                switch(ledDisplayMode)
                {
                    case SINGLE_COLOR_DISPLAY:
                        LED1642_LED_All_On();
                        break;
                    case MOTION_MAP_DISPLAY:
                        if(imuNewGyroFlag == 1)
                        {
                            dmp_read_fifo(gyro, accel, quat, &sensors, &more);
                            ConvertQuaternion(quat, &convertedQuat);
                            QuatToEulerAngle(&convertedQuat, &eulerAngle);
                            channelHistory = channel;
                            colorHistory = color;
                            EulerAngleToLED(&eulerAngle, &channel, &color);
                            if((channel != channelHistory) || (color.r != colorHistory.r) || (color.g != colorHistory.g))
                            {
                                for(int i = 0; i < 16; i++)
                                {
                                    rgbTestArray[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
                                }
                                rgbTestArray[channel] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                                if(channel == 0)
                                {
                                    rgbTestArray[1] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                                    rgbTestArray[15] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                                }
                                else if(channel == 15)
                                {
                                    rgbTestArray[0] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                                    rgbTestArray[14] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};            
                                }
                                else
                                {
                                    rgbTestArray[channel+1] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                                    rgbTestArray[channel-1] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                                }
                                LED1642GW_RGB_Translation_Array(rgbTestArray);
                            }
                            else
                            {
                                
                            }
                            //LED1642GW_RGB_Translation_Individual_Channel(channel, color);
                #if (UART_DEBUGGING == 1)
                            printf("Euler Angles: roll: %f, red: %u, pitch: %f, green: %u\r",eulerAngle.roll*180/PI, color.r, eulerAngle.pitch*180/PI, color.g);
                #endif
                            imuNewGyroFlag = 0;
                        }
                        break;
                    case COLOR_TRANSITION:
                        break;
                    case INCREASE_BRIGHTNESS:
                        break;
                    case DECREASE_BRIGHTNESS:
                        break;
                    default:
                        break;
                    
                }
            }
        }
        else
        {
            Enter_Low_Power_Mode();
            deviceInitFlag = 0;
            __WFI();
        }
    }
#endif
    
#if (MPU6500_TEST == 1)
    IMU_SPI_Init();
    MPU6500_Setup();
    while(1)
    {
        if(imuNewGyroFlag == 1)
        {
            dmp_read_fifo(gyro, accel, quat, &sensors, &more);
            ConvertQuaternion(quat, &convertedQuat);
            QuatToEulerAngle(&convertedQuat, &eulerAngle);
            channelHistory = channel;
            colorHistory = color;
            EulerAngleToLED(&eulerAngle, &channel, &color);
            if((channel != channelHistory) || (color.r != colorHistory.r) || (color.g != colorHistory.g))
            {
                for(int i = 0; i < 16; i++)
                {
                    rgbTestArray[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
                }
                rgbTestArray[channel] = (rgb_led){.r = color.r, .g = color.g, .b = color.b};
                if(channel == 0)
                {
                    rgbTestArray[1] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                    rgbTestArray[15] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                }
                else if(channel == 15)
                {
                    rgbTestArray[0] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                    rgbTestArray[14] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};            
                }
                else
                {
                    rgbTestArray[channel+1] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                    rgbTestArray[channel-1] = (rgb_led){.r = color.r/4, .g = color.g/4, .b = color.b/4};
                }
                LED1642GW_RGB_Translation_Array(rgbTestArray);
            }
            else
            {
                
            }
            //LED1642GW_RGB_Translation_Individual_Channel(channel, color);
#if (UART_DEBUGGING == 1)
            printf("Euler Angles: roll: %f, red: %u, pitch: %f, green: %u\r",eulerAngle.roll*180/PI, color.r, eulerAngle.pitch*180/PI, color.g);
#endif
            imuNewGyroFlag = 0;
        }
    }
#endif
#if(LED_TEST)
    LED_SPI_Init();
    nrf_delay_ms(10);
    PWM_PWCLK_play();
    nrf_delay_ms(10);
    LED1642GW_Driver_Count();
    for(int i = 0; i < 16; i++)
    {
        rgbTestArray[i] = (rgb_led){.r = 0, .g = 0, .b = 0};
    }
    LED1642GW_RGB_Translation_Array(rgbTestArray);
    
    while(1)
    {
        for(int j = 0; j < 16; j++)
        {
            for(int i = 0; i < 16; i++)
            {
                if(i <= j)
                {
                    rgbTestArray[j-i] = (rgb_led){.r = 0 + (15-i)*15, .g = 150-(15-i)*10, .b = 0};
                }
                else
                {
                    rgbTestArray[i+j] = (rgb_led){.r = 0 + i*15, .g = 150-i*10, .b = 0};
                }
            }
            LED1642GW_RGB_Translation_Array(rgbTestArray);
            nrf_delay_ms(500);
        }
        
    }
    //
    //rgb_led rgbTestColor = {.r = 255, .g = 0, .b = 0};
    
    
    LED1642GW_RGB_Translation_Array(rgbTestArray);
    LED1642GW_RGB_Translation_Array(rgbTestArray);
    while(1)
    {
        
    }
    //LED1642_LED_RGB_Train_Forward();
    /*
    while(1)
    {
        rgbTestColor.r = 255;
        rgbTestColor.g = 0;
        rgbTestColor.b = 0;
        
        for(int i = 0; i < 16; i++)
        {
            
            rgbTestColor.r = rgbTestColor.r - 15 * i;
            rgbTestColor.g = rgbTestColor.g + 10 * i;
            rgbTestColor.b = rgbTestColor.b + 5 * i;
            
            LED1642GW_RGB_Translation_Individual_Channel(i, rgbTestColor);
            nrf_delay_ms(200);
        }
    }
    */
#endif
}
