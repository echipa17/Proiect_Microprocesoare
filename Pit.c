/* Include bibliotecile necesare */
#include "Pit.h"
/* Include bibliotecile necesare */
#include "Buzzer.h"
#define LED_PIN (12) 
#define MASK(x) (1UL << (x))

uint32_t timer_value;
uint8_t led_state;

/* Initializarea PIT */
void PIT_Init(void) {
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	PIT->CHANNEL[0].LDVAL = 0x9FFFFF;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	PIT->CHANNEL[1].LDVAL = 0x63FFFFF;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}
uint8_t UTILS_PrintTimer(uint32_t value){
	uint8_t minutes = value/60;
	uint8_t seconds = value%60;
	if(timer_value == 99*60+59){
		timer_value = 0;
	}
	
}
/* Handler pentru întreruperea temporizatorului PIT */
void PIT_IRQHandler(void) {
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) { /* channel 0 pentru led init */
		UTILS_PrintTimer(timer_value);
		timer_value++;
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		if(led_state) {
			GPIOA->PCOR |= (1<<LED_PIN);
			led_state = 0;
		}
	}
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) { /* channel 1 pentru led togle */
		GPIOA->PSOR |= (1<<LED_PIN);
		led_state = 1;
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}
}
