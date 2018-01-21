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
 * Available colors.
 */
#define DISPLAY_PIXEL_ON_COLOR 0xFF
#define DISPLAY_PIXEL_OFF_COLOR 0x00

/**
 * Call this function to initialize display-related hardware (set up port directions etc).
 */
void InitializeDisplay(void);

/**
 * Turn display on (on != 0x00) or off (on == 0x00).
 */
void DisplayOnOff(uint8_t on);

/**
 * Clears framebuffer.
 */
void ClearFramebuffer(void);

/**
 * Pushes framebuffer (or at least modified parts) to display.
 */
void PushFramebuffer(void);

/**
 * Draws a pixel of current color.
 */
void DrawPixel(uint16_t y, uint16_t x);

/*
 * Sets active color for drawing.
 */
void SetActiveColor(uint32_t color);




#endif /* HAL_DISPLAY_DRIVER_PROTOTYPE_H_ */
