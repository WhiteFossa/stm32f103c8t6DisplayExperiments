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
#define STD_LEFT_CTRLR		8U

/**
 * Set this bit in SendToDisplay word to send command to display. Command always are being sent to both controllers, ignoring STD_LEFT_CTRLR.
 */
#define STD_COMMAND			9U

/**
 * Call this function to send data/command to display. Data/command is in [0-7] bits.
 */
void SendToDisplay(uint16_t data);

/**
 * Dumb delay (later will be smarter).
 */
void Delay(void);

#endif /* HAL_DISPLAY_DRIVER_IMPLEMENTATION_H_ */
