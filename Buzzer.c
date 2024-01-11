

#include "Buzzer.h" 
#include "Uart.h" 
#include <stdbool.h>
#define buzzer (4) 
#define MASK(x) (1UL << (x)) 
int val;


/* Funcția de inițializare a buzzerului*/
void BUZZER_init(void) 
	{ 
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;	
		PORTD->PCR[buzzer] &= ~PORT_PCR_MUX_MASK;
		PORTD->PCR[buzzer] |= PORT_PCR_MUX(1);
		PTD->PDDR |= MASK(buzzer);
	}


/* Funcție de întârziere*/
void delay(volatile unsigned int time) 
{
		while(time--){ }
}

/* Funcția care redă un sunet pe buzzer*/
void BUZZER_play(uint32_t frequency, uint32_t duration_ms) 
{
    uint32_t period = 1000000 / frequency; /* Period in microseconds for half cycle */
    uint32_t duration = duration_ms * 1000; /* Convert duration to microseconds */
	uint32_t elapsed;
    for (elapsed = 0; elapsed < duration; elapsed += period)
    {
        PTD->PTOR = MASK(buzzer); /* Toggle the buzzer pin */
        delay(period / 2); /* Delay for half period */
    }
}

void BUZZER_toggle(void) {
    PTD->PCOR = MASK(buzzer); /* Clear the buzzer pin to stop the buzzer */
}
