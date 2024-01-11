#include "MKL25Z4.h"

void UART0_Transmit(uint8_t data);
void UART0_Transmit_String(const char* str);
uint8_t UART0_Receive_NonBlocking(void);
int UART0_Available(void);
uint8_t UART0_ReadChar(void);
void UART0_Init(uint32_t baud_rate);
void UART0_IRQHandler(void);
