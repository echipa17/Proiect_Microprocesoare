#include "MKL25Z4.h"
#define TOUCH_THRESHOLD 2100
#define TOUCH_CHANNEL_0 9
#define TOUCH_CHANNEL_1 10
uint16_t TSI_Read1(uint8_t channel);
uint16_t TSI_Read2(uint8_t channel);
void TSI_Init(void);
void UART_Send_String(char* str);
void UART_FlushBuffer(void);
