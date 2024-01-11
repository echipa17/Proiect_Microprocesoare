/* Include bibliotecile necesare*/
#include "Led.h"
/* Include bibliotecile necesare*/
#include <assert.h>
#define PIT_CLOCK_FREQ 20971520u 
#define DELAY_MS 836
volatile LedState currentLedState = LIGHT_WHITE;

/* initializarea RGBLED config*/
void RRGBLED_init(void){
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
    PORTB->PCR[RED_LED_PIN] = PORT_PCR_MUX(1);
    PORTB->PCR[GREEN_LED_PIN] = PORT_PCR_MUX(1);
    PORTD->PCR[BLUE_LED_PIN] = PORT_PCR_MUX(1);
	PTB->PSOR = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);
	PTD->PSOR = (1 << BLUE_LED_PIN);
    PTB->PDDR |= (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);
    PTD->PDDR |= (1 << BLUE_LED_PIN);
}
/* initializarea PIT*/
void PIT_Init(void) {
  	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->CHANNEL[0].LDVAL = (PIT_CLOCK_FREQ/1000) * DELAY_MS;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    currentLedState = (currentLedState + 1) % NUM_STATES;
    switch (currentLedState) {
        case LIGHT_WHITE:
            PTB->PCOR = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN); 
		    PTD->PCOR = (1 << BLUE_LED_PIN); 
            break;
        case LIGHT_RED:
            PTB->PCOR = (1 << RED_LED_PIN);
            PTB->PSOR = (1 << GREEN_LED_PIN);
            PTD->PSOR = (1 << BLUE_LED_PIN);
            break;
        case LIGHT_PURPLE:
            PTB->PCOR = (1 << RED_LED_PIN);
            PTB->PSOR = (1 << GREEN_LED_PIN);
            PTD->PCOR = (1 << BLUE_LED_PIN);
            break;
        case LIGHT_YELLOW:
            PTB->PCOR = (1 << RED_LED_PIN);
            PTB->PCOR = (1 << GREEN_LED_PIN);
            PTD->PSOR = (1 << BLUE_LED_PIN);
            break;
        case NUM_STATES:
            break;
    }
}

void Change_LED_PIT_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    currentLedState = (currentLedState + 1) % NUM_STATES;
    switch (currentLedState) {

        case LIGHT_YELLOW:
            PTB->PCOR = (1 << RED_LED_PIN);
            PTB->PCOR = (1 << GREEN_LED_PIN);
            PTD->PSOR = (1 << BLUE_LED_PIN);
            break;
        case LIGHT_PURPLE:
            PTB->PCOR = (1 << RED_LED_PIN);
            PTB->PSOR = (1 << GREEN_LED_PIN);
            PTD->PCOR = (1 << BLUE_LED_PIN);
            break;
        case LIGHT_RED:
            PTB->PCOR = (1 << RED_LED_PIN);
            PTB->PSOR = (1 << GREEN_LED_PIN);
            PTD->PSOR = (1 << BLUE_LED_PIN);
            break;
        case LIGHT_WHITE:
            PTB->PCOR = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN); 
		    PTD->PCOR = (1 << BLUE_LED_PIN); 
            break;

        case NUM_STATES:
            break;
    }
}

