#include "MPU6500.h"

#define DEFAULT_MPU_HZ  15

extern uint8_t SPI_command; 
extern uint8_t spi_complete;
extern uint8_t brightness;
extern volatile uint8_t imuNewGyroFlag;
extern volatile uint8_t powerOnFlag;
extern volatile uint8_t batteryChargingFlag;
volatile uint8_t ledDisplayMode = 0;
volatile uint8_t setupCompleteFlag = 0;
volatile uint8_t driveTrainBreakFlag = 0;

uint8_t MPU6500_Connection_Test(void)
{
    uint8_t SPI_data[2];
    SPI_command = MPU6500_WHO_AM_I + MPU6500_READ;
    //Start_Transaction(SS_CH7);
    IMU_SPI_Receive(&SPI_command, SPI_data, 1, 2);
    //Stop_Transaction(SS_CH7);
    if(SPI_data[1] == 0x70)
        return 1;
    return 0;
}

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from thei
 * driver(s).
 * TODO: The following matrices refer to the configuration on an internal test
 * board at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */

static signed char gyro_orientation[9] = {1, 0, 0,
                                          0, 1, 0,
                                          0, 0, 1};

enum packet_type_e {
    PACKET_TYPE_ACCEL,
    PACKET_TYPE_GYRO,
    PACKET_TYPE_QUAT,
    PACKET_TYPE_TAP,
    PACKET_TYPE_ANDROID_ORIENT,
    PACKET_TYPE_PEDO,
    PACKET_TYPE_MISC
};
uint8_t MPU6500_Setup(void)
{
    int result;
    uint8_t accel_fsr;
    uint16_t gyro_rate, gyro_fsr;
    result = mpu_init();
    if(result)
    {
        return 0;
    }
    /* Wake up all sensors. */
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    /* Push both gyro and accel data into the FIFO. */
    mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    mpu_set_sample_rate(DEFAULT_MPU_HZ);
    /* Read back configuration in case it was set improperly. */
    mpu_get_sample_rate(&gyro_rate);
    mpu_get_gyro_fsr(&gyro_fsr);
    mpu_get_accel_fsr(&accel_fsr);
    
    dmp_load_motion_driver_firmware();
    dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
    dmp_register_tap_cb(MPU6500_tap_cb);
    dmp_register_android_orient_cb(MPU6500_orient_cb);
    MPU6500_Enable_Full_Features();
    dmp_set_fifo_rate(DEFAULT_MPU_HZ);
    dmp_set_interrupt_mode(DMP_INT_GESTURE);

    return 0;
}

void MPU6500_Enable_DMP(void)
{
    //Enable DMP
    mpu_set_dmp_state(1);
}

void MPU6500_Disable_DMP(void)
{
    //Enable DMP
    mpu_set_dmp_state(0);
}

void MPU6500_Enable_Tap_Feature(void)
{
    uint16_t dmp_features = DMP_FEATURE_TAP;
    dmp_enable_feature(dmp_features);
}

void MPU6500_Enable_Full_Features(void)
{
    uint16_t dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL;
    dmp_enable_feature(dmp_features);
}

void getDMP_Data(void)
{
    int16_t gyro[3], accel[3], sensors;
    unsigned char more;
    long quat[4];
    if(imuNewGyroFlag)
    {
        dmp_read_fifo(gyro, accel, quat, &sensors, &more);
        if(!more)
        {
            imuNewGyroFlag = 0;
        }
    }
}

static void MPU6500_tap_cb(uint8_t direction, uint8_t count)
{
    if(direction == TAP_Z_DOWN)
    {
        setupCompleteFlag = 0;
        ledDisplayMode ++;
        if(ledDisplayMode == LED_MODE_NUM)
        {
            ledDisplayMode = SINGLE_COLOR_DISPLAY_BATTERY;
        }
        if(ledDisplayMode == SINGLE_COLOR_DISPLAY_BATTERY)
        {
            LED1642GW_Brightness_Set(0x00);
        }
        if(ledDisplayMode == MOTION_MAP_DISPLAY)
        {
            MPU6500_Disable_DMP();
            dmp_set_interrupt_mode(DMP_INT_CONTINUOUS);
            MPU6500_Enable_DMP();
        }
        else if(ledDisplayMode == COLOR_TRANSITION)
        {
            MPU6500_Disable_DMP();
            dmp_set_interrupt_mode(DMP_INT_GESTURE);
            MPU6500_Enable_DMP();
        }
        else if(ledDisplayMode == OTA_UPDATE)
        {
            driveTrainBreakFlag = 1;
            advertising_start();
        }
    }
    else if(direction == TAP_X_UP)
    {
        LED1642GW_Brightness_Control_PowerOn(BRIGHTNESS_INCREASE);
    }
    else if(direction == TAP_X_DOWN)
    {
        LED1642GW_Brightness_Control_PowerOn(BRIGHTNESS_DECREASE);
    }
    imuNewGyroFlag = 0;
    //send_packet(PACKET_TYPE_TAP, data);
}

static void MPU6500_orient_cb(uint8_t orientation)
{
    //send_packet(PACKET_TYPE_ANDROID_ORIENT, &orientation);
    __asm{NOP};
}

static inline uint16_t inv_row_2_scale(const signed char *row)
{
    uint16_t b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static inline uint16_t inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    uint16_t scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

/* Send data to the Python client application.
 * Data is formatted as follows:
 * packet[0]    = $
 * packet[1]    = packet type (see packet_type_e)
 * packet[2+]   = data
 */
void send_packet(char packet_type, void *data)
{
#define MAX_BUF_LENGTH  (18)
    char buf[MAX_BUF_LENGTH], length;

    memset(buf, 0, MAX_BUF_LENGTH);
    buf[0] = '$';
    buf[1] = packet_type;

    if (packet_type == PACKET_TYPE_ACCEL || packet_type == PACKET_TYPE_GYRO) {
        short *sdata = (short*)data;
        buf[2] = (char)(sdata[0] >> 8);
        buf[3] = (char)sdata[0];
        buf[4] = (char)(sdata[1] >> 8);
        buf[5] = (char)sdata[1];
        buf[6] = (char)(sdata[2] >> 8);
        buf[7] = (char)sdata[2];
        length = 8;
    } else if (packet_type == PACKET_TYPE_QUAT) {
        long *ldata = (long*)data;
        buf[2] = (char)(ldata[0] >> 24);
        buf[3] = (char)(ldata[0] >> 16);
        buf[4] = (char)(ldata[0] >> 8);
        buf[5] = (char)ldata[0];
        buf[6] = (char)(ldata[1] >> 24);
        buf[7] = (char)(ldata[1] >> 16);
        buf[8] = (char)(ldata[1] >> 8);
        buf[9] = (char)ldata[1];
        buf[10] = (char)(ldata[2] >> 24);
        buf[11] = (char)(ldata[2] >> 16);
        buf[12] = (char)(ldata[2] >> 8);
        buf[13] = (char)ldata[2];
        buf[14] = (char)(ldata[3] >> 24);
        buf[15] = (char)(ldata[3] >> 16);
        buf[16] = (char)(ldata[3] >> 8);
        buf[17] = (char)ldata[3];
        length = 18;
    } else if (packet_type == PACKET_TYPE_TAP) {
        buf[2] = ((char*)data)[0];
        buf[3] = ((char*)data)[1];
        length = 4;
    } else if (packet_type == PACKET_TYPE_ANDROID_ORIENT) {
        buf[2] = ((char*)data)[0];
        length = 3;
    } else if (packet_type == PACKET_TYPE_PEDO) {
        long *ldata = (long*)data;
        buf[2] = (char)(ldata[0] >> 24);
        buf[3] = (char)(ldata[0] >> 16);
        buf[4] = (char)(ldata[0] >> 8);
        buf[5] = (char)ldata[0];
        buf[6] = (char)(ldata[1] >> 24);
        buf[7] = (char)(ldata[1] >> 16);
        buf[8] = (char)(ldata[1] >> 8);
        buf[9] = (char)ldata[1];
        length = 10;
    } else if (packet_type == PACKET_TYPE_MISC) {
        buf[2] = ((char*)data)[0];
        buf[3] = ((char*)data)[1];
        buf[4] = ((char*)data)[2];
        buf[5] = ((char*)data)[3];
        length = 6;
    }
    UART_Send_Multiple((uint8_t *)buf, length);
}
