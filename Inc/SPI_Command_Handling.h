#ifndef SPI_COMMAND_HANDLING_H
#define SPI_COMMAND_HANDLING_H

#include "SPI.h"
#include "Grips.h"
#include <stdint.h>

#define NUM_CMD_BYTES       5
#define NUM_SPI_BYTES       14

#define CMD_LED_OFF 		0
#define CMD_LED_ON 			1
#define CMD_CHANGE_PWM 		2
#define CMD_CHANGE_IQ		3
#define CMD_GET_ENCODER_POS	5	//the flux position of the rotor in ABSOLUTE frame (dh, once unwrap works maybe record offset)
#define CMD_HARD_BRAKE		6
#define CMD_DRIVER_ENABLE	7
#define CMD_DRIVER_DISABLE	8
#define CMD_ROTOR_POS		9
#define CMD_ROTOR_SPEED		10
#define CMD_RESET_T			11	//reset commutation interval estimation
#define CMD_RESET_POS		12

#define CMD_SLEEP			14
#define CMD_WAKEUP			23

#define CMD_PLAY_TONE		15	//play a tone! master data corresponds to

#define CMD_SET_FOC_MODE	16
#define CMD_SET_TRAP_MODE	17
#define CMD_SET_SIN_MODE	18

#define CMD_EN_PRES			19
#define CMD_DIS_PRES		20
#define CMD_READ_PRES		21

#define CMD_NO_ACTION		22
#define CMD_BOOTLOAD        24

void Channel_Write(int channel, uint8_t cmd, int32_t tx_data);
void Channel_Read(int channel, uint8_t cmd, int32_t tx_data, int32_t * rx_data);    
void Command_Parse(int32_t cmd_in, uint8_t * cmd_out);

#endif
