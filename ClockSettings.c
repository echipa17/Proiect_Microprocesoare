/* Include header-ul pentru setările ceasului*/
#include "ClockSettings.h"

/*Variabile statice pentru a ține evidența timpului*/
static uint16_t base_tick;
static uint16_t tick_50ms;
static uint16_t tick_100ms;
static uint16_t tick_500ms;
static uint16_t tick_1s;

/*Variabile volatile pentru indicarea atingerii anumitor intervale de timp*/
volatile uint8_t flag_50ms;
volatile uint8_t flag_100ms;
volatile uint8_t flag_500ms;
volatile uint8_t flag_1s;
volatile uint8_t flag_5s;

/*Configurează ceasul sistemului*/
void SystemClock_Configure(void) {
    MCG->C1 |= MCG_C1_CLKS(0);
    MCG->C1 |= MCG_C1_IREFS_MASK;
    MCG->C4 |= MCG_C4_DRST_DRS(1);
    MCG->C4 |= MCG_C4_DMX32(1);
}

/*Configurează timerul SysTick*/
void SystemClockTick_Configure(void){
    SysTick->LOAD = (uint32_t)(48000000UL / 1000 - 1UL);
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    SysTick->VAL = 0UL;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | 
                                    SysTick_CTRL_TICKINT_Msk | 
                                    SysTick_CTRL_ENABLE_Msk); 
}

/*Handler pentru întreruperea SysTick*/
void SysTick_Handler(void) {
    
    ++base_tick;
    if(base_tick >= 50U) { base_tick = 0U; flag_50ms = 1U; ++tick_50ms;}
    if(tick_50ms >= 2U) { tick_50ms = 0U; flag_100ms = 1U; ++tick_100ms;}
    if(tick_100ms >= 5U) { tick_100ms = 0U; flag_500ms = 1U; ++tick_500ms; }
    if(tick_500ms >= 2U) { tick_500ms = 0U; flag_1s = 1U; ++tick_1s;}
    if(tick_1s >= 5U) { tick_1s = 0U; flag_5s = 1U; }
}
