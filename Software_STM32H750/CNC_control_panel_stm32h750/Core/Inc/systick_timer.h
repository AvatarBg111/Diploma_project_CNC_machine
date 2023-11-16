#ifndef __SYSTICK_TIMER_H
#define __SYSTICK_TIMER_H

// Includes
#include "main.h"


#ifdef __cplusplus
extern "C" {
#endif


// Exported macros and typedefs


// Exported variables


// Private function declarations
uint32_t GetSysTickTimer_ms(void);

void clear_wait_ms();
uint8_t wait_ms(uint32_t time);

void clear_wait_ms_ch(uint8_t channel);
void reset_wait_ms_ch(uint8_t channel);
uint8_t wait_ms_ch(uint8_t channel, uint32_t time);


#ifdef __cplusplus
}
#endif

#endif /* __SYSTICK_TIMER_H */

/************************ (C) COPYRIGHT GreenBo ****************END OF FILE****/
