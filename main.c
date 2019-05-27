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

#define MPU6500_TEST        1
#define LED_TEST            0
#define INTERRUPT_TEST      0
unsigned int led_ts = 0;

extern uint8_t BLE_cmd[3];
extern volatile uint8_t imu_new_gyro;
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
    rgb_led rgbTestArray[16];
    nrf_saadc_value_t touchInValue;
    volatile ret_code_t err_code;
    Quaternion convertedQuat;
    EulerAngle eulerAngle;
    //err_code = ble_dfu_buttonless_async_svci_init();
    //APP_ERROR_CHECK(err_code);
    //uint8_t result;
    timers_init();
    /*Initialize peripherals*/
    
    //GPIO_Init();
    
    PWM_init();
    PWM_PWCLK_init();
    UART_Init();
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
    //SAADC_Init();

    
    /*
    nrf_gpio_pin_write(BOOT_CH3, 1);
    power_management_init();
    ble_stack_init();
    gap_params_init();
    gatt_init();
    services_init();
    advertising_init();
    conn_params_init();
    advertising_start();
    */
    
#if (MPU6500_TEST == 1)
    IMU_SPI_Init();
    __asm{NOP};
    //MPU6500_Connection_Test();
    __asm{NOP};
    MPU6500_Setup();
    __asm{NOP};
    //mpu_lp_motion_interrupt(500, 1, 5);
    short gyro[3], accel[3], sensors;
    unsigned char more;
    long quat[4];
    uint8_t channel;
    rgb_led color;
    uint8_t channel_history;
    rgb_led color_history;
    while(1)
    {
        if(imu_new_gyro == 1)
        {
            dmp_read_fifo(gyro, accel, quat, &sensors, &more);
            //if (sensors & INV_XYZ_GYRO)
                //send_packet(PACKET_TYPE_GYRO, gyro);
            //if (sensors & INV_XYZ_ACCEL)
                //send_packet(PACKET_TYPE_ACCEL, accel);
            //if (sensors & INV_WXYZ_QUAT)
                //send_packet(PACKET_TYPE_QUAT, quat);
            //__asm{NOP};
            ConvertQuaternion(quat, &convertedQuat);
            QuatToEulerAngle(&convertedQuat, &eulerAngle);
            channel_history = channel;
            color_history = color;
            EulerAngleToLED(&eulerAngle, &channel, &color);
            if((channel != channel_history) || (color.r != color_history.r) || (color.g != color_history.g))
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
            printf("Euler Angles: roll: %f, red: %u, pitch: %f, green: %u\r",eulerAngle.roll*180/PI, color.r, eulerAngle.pitch*180/PI, color.g);
            imu_new_gyro = 0;
        }
        
        //
        
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
