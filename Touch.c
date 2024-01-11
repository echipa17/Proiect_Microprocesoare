#include "Touch.h"
#include "Uart.h"
#include <stdio.h>
#include "Buzzer.h"
#define PORTB_BASE (0x4004A000u) /*test*/
#define PORTB ((PORT_Type *)PORTB_BASE)
	
/* serati registrii pentru initializarea modulului tactil al KL25Z*/
void TSI_Init(void) {
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;
    PORTB->PCR[16] = PORT_PCR_MUX(0);
    PORTB->PCR[17] = PORT_PCR_MUX(0);
    TSI0->GENCS = TSI_GENCS_OUTRGF_MASK | TSI_GENCS_MODE(0) | TSI_GENCS_REFCHRG(4) | TSI_GENCS_DVOLT(0) | TSI_GENCS_EXTCHRG(7) | TSI_GENCS_PS(4) | TSI_GENCS_NSCN(31) | TSI_GENCS_TSIEN_MASK;
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
}

/* functia de citire a touch-ului pentru canallul 9 stanga suprafetei tactile*/
uint16_t TSI_Read1(uint8_t channel) {
    int16_t counter;
    TSI0->DATA = TSI_DATA_TSICH(channel);
    TSI0->DATA |= TSI_DATA_SWTS_MASK;
    while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
    counter = (int16_t)(TSI0->DATA & TSI_DATA_TSICNT_MASK);
    if(counter > TOUCH_THRESHOLD) 
    { 
         BUZZER_play(500, 800); 
    }
    return (uint16_t)counter; 
}
/* functia de citire a touch-ului pentru canallul 10 dreapta suprafetei tactile*/
uint16_t TSI_Read2(uint8_t channel) {
    int16_t counter;
    TSI0->DATA = TSI_DATA_TSICH(channel);
    TSI0->DATA |= TSI_DATA_SWTS_MASK;
    while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
    counter = (int16_t)(TSI0->DATA & TSI_DATA_TSICNT_MASK);
    if(counter > TOUCH_THRESHOLD)  /* intra in bucla doar daca TOUCH_THRESHOLD depaseste valoarea 2100 care regleaza sensibilitatea suprafetei tactile  */
    { 
         BUZZER_play(1000, 800); 
    }
    return (uint16_t)counter; 
}
