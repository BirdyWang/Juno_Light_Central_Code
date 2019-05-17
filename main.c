#include "Juno_Light_Central_Config.h"
#include "Juno_Light_Central_Pin_Map.h"
#include "Juno_BLE.h"
#include "GPIO.h"
#include "PWM.h"
#include "Command_Handling.h"
#include "MPU6500.h"
#include "LED1642GW.h"
#include "ble_dfu.h"
#define INJECT_I2C_COMMANDS

#define SLEEP_TIMEOUT_VALUE 2000

#define MPU6500_TEST        0
#define LED_TEST            1

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
    volatile ret_code_t err_code;
    //err_code = ble_dfu_buttonless_async_svci_init();
    //APP_ERROR_CHECK(err_code);
    //uint8_t result;
    timers_init();
    /*Initialize peripherals*/
    
    GPIO_Init();
    
    PWM_init();
    PWM_PWCLK_init();
    UART_Init();
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
    MPU6500_Connection_Test();
    __asm{NOP};
    MPU6500_Setup();
    __asm{NOP};
    //mpu_lp_motion_interrupt(500, 1, 5);
    short gyro[3], accel[3], sensors;
    unsigned char more;
    long quat[4];
    while(1)
    {
        if(imu_new_gyro == 1)
        {
            dmp_read_fifo(gyro, accel, quat, &sensors, &more);
            //if (sensors & INV_XYZ_GYRO)
                //send_packet(PACKET_TYPE_GYRO, gyro);
            //if (sensors & INV_XYZ_ACCEL)
                //send_packet(PACKET_TYPE_ACCEL, accel);
            if (sensors & INV_WXYZ_QUAT)
                send_packet(PACKET_TYPE_QUAT, quat);
            __asm{NOP};
            imu_new_gyro = 0;
        }
        /*
        Channel_Write(0, 0, 0);
        nrf_delay_ms(100);
        Channel_Write(0, 1, 0);
        nrf_delay_ms(100);
        */
    }
#endif
#if(LED_TEST)
    LED_SPI_Init();
    PWM_PWCLK_play();
    LED1642GW_Driver_Count();
    rgb_led rgbTestColor = {.r = 0, .g = 255, .b = 0};
    /*
    for(int i = 0; i < 16; i++)
    {
        LED1642GW_RGB_Translation_Individual_Channel(i, rgbTestColor);
        nrf_delay_ms(100);
        __asm{NOP};
    }
    */
    
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
            nrf_delay_ms(10);
        }
    }
#endif
}
