#include "MKL25Z4.h"
#include "Buzzer.h" 
#include "Uart.h"
#include "Adc.h"
#include "Tempsenzor.h"
#include "Led.h"
#include "ClockSettings.h"
#include "Touch.h"

extern int val;

int main(void) {
    SystemClock_Configure();
    SystemClockTick_Configure();
    UART0_Init(9600);
    ADC0_Init();
    BUZZER_init();    
    RRGBLED_init();
    PIT_Init();
    TSI_Init();
    
    for (;;) { 
        if (flag_50ms) {
            scan_temperature();
            flag_50ms = 0U; 
            TSI_Read1(TOUCH_CHANNEL_0);
            TSI_Read2(TOUCH_CHANNEL_1);
        }

        
            if (UART0_Available()) {
                int receivedChar;
                receivedChar = UART0_ReadChar();
                if (receivedChar == '1') { val = 1; }
                else if (receivedChar == '0') {val = 0; }
                else if (receivedChar == '2') {val = 2;}
            }
    }
}
