#ifndef __R61529_H
#define __R61529_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "fonts.h"
#include "stm32h7xx_hal.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define swap(a,b) {int16_t t=a;a=b;b=t;}
#define rgb565(r,g,b) {(uint16_t)((uint16_t)(b & 0x1F) | ((uint16_t)(g & 0x3F) << 5) | ((uint16_t)(g & 0x3F) << 11))}

/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define  RESET_ACTIVE   HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);
#define  RESET_IDLE     HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
#define  RS_ACTIVE   HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, GPIO_PIN_RESET);
#define  RS_IDLE     HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, GPIO_PIN_SET);
#define  CS_ACTIVE   HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
#define  CS_IDLE     HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
#define  RD_ACTIVE   HAL_GPIO_WritePin(TFT_RD_GPIO_Port, TFT_RD_Pin, GPIO_PIN_RESET);
#define  RD_IDLE     HAL_GPIO_WritePin(TFT_RD_GPIO_Port, TFT_RD_Pin, GPIO_PIN_SET);
#define  WR_ACTIVE   HAL_GPIO_WritePin(TFT_WR_GPIO_Port, TFT_WR_Pin, GPIO_PIN_RESET);
#define  WR_IDLE     HAL_GPIO_WritePin(TFT_WR_GPIO_Port, TFT_WR_Pin, GPIO_PIN_SET);

// Определяем адреса
// Для записи данных
#define LCD_DATA    			    0x60080000
#define DATA 1
// Для записи команд
#define LCD_REG   		  	    0x60000000
#define COMMAND 0

#define  BLACK   0x0000
#define  GRAY    0xAD55
#define  WHITE   0xFFFF
#define  BROWN   0xB380
#define  RED     0xF800
#define  ORANGE	 0xF564
#define  YELLOW  0xFFE0
#define  GREEN   0x07E0
#define  CYAN    0x07FF
#define  BLUE    0x001F
#define  MAGENTA 0xF81F
#define	 PURPLE  0xC01F

#define ROTATE_DEFAULT 0
#define ROTATE_RIGHT 1
#define ROTATE_UPSIDE_DOWN 2
#define ROTATE_LEFT 3

void Init_R61529(void);
void R61529_SetRotation(unsigned char r);
void R61529_Flood(uint16_t color, uint32_t len);
void R61529_FillScreen(uint16_t color);

void R61529_FillRect(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
void R61529_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color);
void R61529_DrawPixel(int x, int y, uint16_t color);
void R61529_DrawLine(uint16_t color, uint16_t x1, uint16_t y1,	uint16_t x2, uint16_t y2);
void R61529_DrawRect(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
void Draw_Image(const short *image_array, uint16_t x_coordinat, uint16_t y_coordinat, uint16_t img_width, uint16_t img_height, uint32_t s_img);
void R61529_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

#ifdef __cplusplus
}
#endif

#endif // __FONTS_H
