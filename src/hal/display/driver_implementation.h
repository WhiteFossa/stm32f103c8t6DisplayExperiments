/*
 * driver_implementation.h
 *
 *  Created on: 12 янв. 2018 г.
 *      Author: fossa
 */

#ifndef HAL_DISPLAY_DRIVER_IMPLEMENTATION_H_
#define HAL_DISPLAY_DRIVER_IMPLEMENTATION_H_

#include "stm32f10x.h"
#include "../../helpers/stm32_macroses.h"
#include "driver_prototype.h"

/**
 *		Display pins		MCU pins
 *
 * 1	#CS1				PORT A8
 * 2	#CS2				PORT A9
 * 3	GND
 * 4	VCC
 * 5	Contrast adjust
 * 6	Data / #Command		PORT A10
 * 7	Read / #Write		GND
 * 8	Enable				PORT A11
 *
 * 9	DB0					PORT A0
 * ..	..
 * 16	DB7					PORT A7
 */

/**
 * Port A bits.
 */
#define DISP_CS1			8U
#define DISP_CS2			9U
#define DISP_DC				10U
#define DISP_E				11U

/**
 * Data bus value can be obtained by applying this mask to argument of SendToDisplay().
 */
#define DISP_DATA_MASK		0x00FFU

/**
 * Bits for SendToDisplay word.
 */

/**
 * Set this bit in SendToDisplay word to make request to left controller. If not set, request will go to right controller.
 */
#define DISP_STD_LEFT_CTRLR		8U

/**timer ticks
 * Set this bit in SendToDisplay word to send command to display. Command always are being sent to both controllers, ignoring STD_LEFT_CTRLR.
 */
#define DISP_STD_COMMAND			9U

/**
 * With 36 MHz timer and this prescaler we will get near 100nS rate.
 */
#define DISP_TIMER_PRESCALER		0x04U

/**
 * Time in timer ticks from setting data on bus to strobe.
 */
#define DISP_DATA_TO_STROBE_TIME	100U

/**
 * Display strobe duration in timer ticks.
 */
#define DISP_STROBE_LENGTH			100U

/**
 * Time in timer ticks from strobe end to new command.
 */
#define DISP_STROBE_END_TO_IDLE_TIME	100U

/*
 * Mask for masking display Y address.
 */
#define DISP_Y_ADDRESS_MASK 0b00111111

/*
 * Mask for masking display X address.
 */
#define DISP_X_ADDRESS_MASK 0b00000111;

/*
 * Mask for masking display start line.
 */
#define DISP_START_LINE_MASK 0b00111111;

/**
 * Possible states of 'send command' finite automate.
 */
typedef enum
{
	Idle = 0U,
	DataSent = 1U,
	StrobeSet = 2U,
	StrobeCleared = 3U
} SendToDisplayStates;

/**
 * State of SendCommandState automate.
 */
SendToDisplayStates SendToDisplayState;

/**
 * Tries to send command to display. Returns 0 and do nothing if display isn't ready now. Otherwise schedules command
 * and immediately returns with not 0.
 */
uint8_t SendToDisplay(uint16_t data);

/**
 * Waits until display will be able to get next command, then send it.
 */
void SendToDisplayWithWait(uint16_t data);

/**
 * Sets display Y address.
 */
void DisplaySetYAddress(uint8_t addr);

/**
 * Sets display X address.
 */
void DisplaySetXAddress(uint8_t addr);

/**
 * Sets display start line.
 */
void DisplaySetStartLine(uint8_t line);


#endif /* HAL_DISPLAY_DRIVER_IMPLEMENTATION_H_ */
