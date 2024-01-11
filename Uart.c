#include "Uart.h"
#include "Buzzer.h"
#include "Led.h"
#include <stdint.h>
#define ERROR 0xFF

void UART0_Transmit_String(const char* str) { 
	while(*str != '\0') {  
		UART0_Transmit(*str); str++;  
	}
	UART0_Transmit(0x0A);
	UART0_Transmit(0x0D);
}
void UART0_Transmit(uint8_t data){ 
	while(!(UART0->S1 & UART0_S1_TDRE_MASK))
		; 
	UART0->D = data;
}
uint8_t UART0_Receive_NonBlocking(void) {
    if (!(UART0->S1 & UART0_S1_RDRF_MASK)) {
        return ERROR;
    }
    return UART0->D;
}
void UART0_Init(uint32_t baud_rate){
	uint32_t osr;
	uint32_t sbr;
	uint8_t temp;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(01);
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[1] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* Configurare RX pentru UART0 */
	PORTA->PCR[2] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* Configurare TX pentru UART0 */
	UART0->C2 &= ~((UART0_C2_RE_MASK) | (UART0_C2_TE_MASK)); 
	osr = 15; 
	sbr = 48000000UL / ((osr + 1)*baud_rate); /* 15+1 = 16 */
	temp = UART0->BDH & ~(UART0_BDH_SBR(0x1F));
	UART0->BDH = temp | UART0_BDH_SBR(((sbr & 0x1F00)>> 8));
	UART0->BDL = (uint8_t)(sbr & UART_BDL_SBR_MASK);
	UART0->C4 |= UART0_C4_OSR(osr);
	UART0->C1 = 0;
	UART0->C2 |= UART0_C2_TIE(0);
	UART0->C2 |= UART0_C2_TCIE(0);
	UART0->C2 |= UART0_C2_RIE(1);
	UART0->C2 |= ((UART_C2_RE_MASK) | (UART_C2_TE_MASK));
	NVIC_EnableIRQ(UART0_IRQn);	
}

int UART0_Available(void) {
    return (UART0->S1 & UART0_S1_RDRF_MASK);
}

uint8_t UART0_ReadChar(void) {
    while (!UART0_Available());  
    return UART0->D;
}

/* Handler pentru întreruperea UART0 */
void UART0_IRQHandler(void) {
    if(UART0->S1 & UART0_S1_RDRF_MASK) {
        char receivedChar = UART0->D;
        if (receivedChar == '1') { val = 1; BUZZER_play(1500, 500);  } 
		else if (receivedChar == '0') { val = 0; BUZZER_toggle(); }
		else if (receivedChar == '2') { val = 2; Change_LED_PIT_IRQHandler(); }  
		
    }
}
