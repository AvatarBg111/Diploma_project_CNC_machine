/* vim: set ai et ts=4 sw=4: */
#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "fonts.h"
#include "main.h"
#include <stdbool.h>

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

/*** Redefine if necessary ***/
#define ILI9341_SPI_PORT hspi2

extern SPI_HandleTypeDef ILI9341_SPI_PORT;

enum ILI9341_DIRECTIONS{
	ILI9341_DIRECTION_DEFAULT,
	ILI9341_DIRECTION_RIGHT,
	ILI9341_DIRECTION_LEFT,
	ILI9341_DIRECTION_UPSIDE_DOWN
};

#define ILI9341_CS_Pin        GPIO_PIN_11
#define ILI9341_CS_GPIO_Port  GPIOB
#define ILI9341_DC_Pin        GPIO_PIN_14
#define ILI9341_DC_GPIO_Port  GPIOE
#define ILI9341_RES_Pin       GPIO_PIN_15
#define ILI9341_RES_GPIO_Port GPIOE

#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
/****************************/

// Color definitions
#define	ILI9341_BLACK   0x0000
#define	ILI9341_BLUE    0xF800
#define	ILI9341_RED     0x001F
#define	ILI9341_GREEN   0x0E70
#define ILI9341_CYAN    0xFFD0
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0x07FF
#define ILI9341_WHITE   0xFFFF
#define ILI9341_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

void ILI9341_Unselect();
void ILI9341_Init(void);
void ILI9341_SetRotation(uint8_t);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void ILI9341_InvertColors(bool invert);

#endif // __ILI9341_H__