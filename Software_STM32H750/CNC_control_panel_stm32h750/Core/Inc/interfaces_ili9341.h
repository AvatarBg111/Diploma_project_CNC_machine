#ifndef __INTERFACES_ILI9341_H__
#define __INTERFACES_ILI9341_H__

// Includes
#include "main.h"


// Exported macros and typedefs


// Exported variables
extern UART_HandleTypeDef huart2;


// Exported function declarations
/**
  * @brief Turn on LEDs on control panel
  */
void turn_on_leds(void);

/**
  * @brief Turn off LEDs on control panel
  */
void turn_off_leds(void);

/**
  * @brief Set button channels
  */
void set_button_channels(void);

/**
  * @brief Function for traversing through menu
  * Must be called from a loop in order to refresh interface on certain actions
  */
void menu(void);

#endif //__INTERFACES_ILI9341_H_
