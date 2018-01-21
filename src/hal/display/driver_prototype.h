/*
 * driver.h
 *
 *  Created on: 12 янв. 2018 г.
 *      Author: fossa
 */

#ifndef HAL_DISPLAY_DRIVER_PROTOTYPE_H_
#define HAL_DISPLAY_DRIVER_PROTOTYPE_H_

/**
 * Display sizes.
 */
#define DISPLAY_X_SIZE 128
#define DISPLAY_Y_SIZE 64

/**
 * Call this function to initialize display-related hardware (set up port directions etc).
 */
void InitializeDisplay(void);

/**
 * Turn display on (on != 0x00) or off (on == 0x00).
 */
void DisplayOnOff(uint8_t on);

/**
 * Pushes framebuffer (or at least modified parts) to display.
 */
void PushFramebuffer(void);



#endif /* HAL_DISPLAY_DRIVER_PROTOTYPE_H_ */
