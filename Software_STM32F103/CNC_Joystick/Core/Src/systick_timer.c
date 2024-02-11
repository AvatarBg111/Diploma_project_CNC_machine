/* Private includes ----------------------------------------------------------*/
#include "systick_timer.h"
#include "stm32f1xx.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t msResetFlag_ch[16] = {0};
uint32_t msWaitTime_ch[16] = {0};


/* Private function prototypes  ----------------------------------------------*/
inline uint32_t GetSysTickTimer_ms(void);
void delay_ms(uint32_t);
void clear_wait_ms_ch(uint8_t);
void reset_wait_ms_ch(uint8_t);
void set_wait_ms_ch(uint8_t, uint32_t);
uint8_t wait_ms_ch(uint8_t, uint32_t);


/* Private user code  --------------------------------------------------------*/
/**
  * @brief Return the system ticks as milliseconds
  */
inline uint32_t GetSysTickTimer_ms(void){
    return uwTick;
}

/**
  * @brief Delay procedure
  */
void delay_ms(uint32_t time){
    uint32_t curr_tick = uwTick;

    for(; time > 0; time--){
    	while(curr_tick == uwTick);
    	curr_tick = uwTick;
    }
}

/**
  * @brief Clear wait channel counter
  */
void clear_wait_ms_ch(uint8_t channel){
    msResetFlag_ch[channel] = 1;
}

/**
  * @brief Reset wait channel counter
  */
void reset_wait_ms_ch(uint8_t channel){
	msWaitTime_ch[channel] = GetSysTickTimer_ms();
}

/**
  * @brief Set wait channel counter
  */
void set_wait_ms_ch(uint8_t channel, uint32_t time){
    msWaitTime_ch[channel] = time;
}

/**
  * @brief Wait functionality with channels
  */
uint8_t wait_ms_ch(uint8_t channel, uint32_t time){
    static uint32_t start, end;

    if(msResetFlag_ch[channel]){
		msResetFlag_ch[channel] = 0;
		reset_wait_ms_ch(channel);
    }

    start = msWaitTime_ch[channel];
    end = start + time;
    if(start < end){
		if(GetSysTickTimer_ms() >= end){
			reset_wait_ms_ch(channel);
			return (1);
		}
    }else{
		if((GetSysTickTimer_ms() < start) && (GetSysTickTimer_ms() > end)){
			reset_wait_ms_ch(channel);
			return (1);
		}
    }
    return (0);
}
