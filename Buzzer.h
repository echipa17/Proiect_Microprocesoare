#include "MKL25Z4.h"
void BUZZER_init(void);
void SW_init(void);
void BUZZER_play(uint32_t frequency, uint32_t duration);
void BUZZER_toggle(void);
void delay(volatile unsigned int time);
void PORTA_IRQHandler(void);
int getVal(void);
extern int val;
