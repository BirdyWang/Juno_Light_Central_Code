#include "App_Test_PCB_Pin_Map.h"
#include "App_Test_PCB_Config.h"
#include "Psyonic_BLE.h"
#include "GPIO.h"
#include "SPI.h"
#include "PWM.h"
#include "Motor_Driver_Control.h"
#include "Grips.h"
#include "FOC_Master.h"
#include "Command_Handling.h"
#include "MPU6500.h"
#include "ble_dfu.h"
#include "STM32_Bootloader.h"
#define INJECT_I2C_COMMANDS

#define SLEEP_TIMEOUT_VALUE 2000

unsigned int led_ts = 0;

extern float qd_set[NUM_CHANNELS];
extern float qd[NUM_CHANNELS];
float q[NUM_CHANNELS] = {0,0,0,0,0,0};
float dq[NUM_CHANNELS] = {0,0,0,0,0,0};
const float pgain = 0.3;
const float dgain = 0.000;
float k[NUM_CHANNELS] = {pgain, pgain, pgain, pgain, pgain, pgain};
float kd[NUM_CHANNELS] = {dgain, dgain, dgain, dgain, dgain, dgain};

extern waypoint open_grasp[NUM_CHANNELS];
extern waypoint power_grasp[NUM_CHANNELS];
extern waypoint key_grasp[NUM_CHANNELS];
extern waypoint pinch_grasp[NUM_CHANNELS];
extern waypoint chuck_grasp[NUM_CHANNELS];
extern waypoint horns_grasp[NUM_CHANNELS];
extern waypoint point_grasp[NUM_CHANNELS];
extern waypoint rude_point_grasp[NUM_CHANNELS];	//THE BIRD
extern waypoint open_thumb_up_grasp[NUM_CHANNELS];
extern waypoint open_thumb_down_grasp[NUM_CHANNELS];
extern waypoint mode_switch_closed_grasp[4];
extern waypoint mode_switch_thumb_flexor;
extern waypoint mode_switch_open_grasp[4];
extern waypoint mode_switch_open_thumb_flexor;

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
    SPI_Init();
    UART_Init();
    int i,j;
    rgb_pwm l = {0, 0, 0};
    
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 100; j++)
        {
            if(i == 0)
            {
                l.r = 10*j;
            }
            if(i == 1)
            {
                l.g = 10*j;
            }
            if(i == 2)
            {
                l.b = 10*j;
            }
            PWM_play(l);
            __asm{NOP};
            nrf_delay_ms(5);
        }
    }
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
    //uint8_t poop2[4] = "POOP";
    //UART_Send_Multiple(poop2, 4);
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
}
