#include "Adc.h"
#include "Tempsenzor.h"
#include "Uart.h"
#include <stdio.h>
#include "Buzzer.h"

#define TEMPERATURE_SENSOR_CHN   (26)
#define BANDGAP_VOLTAGE_CHN      (27)
#define DISABLE_ADC  (31)
#define VREF_FACTOR (300)
#define STAGE_1_MAX_TEMP 20 /* Prag 0-20 degrees Celsius */
#define STAGE_2_MAX_TEMP 30 /* Prag 20-30 degrees Celsius */

/* Funcții sau variabile statice, locale pentru acest fișier */
static uint16_t adc_result;
static uint16_t adc_result_avg;
static uint16_t Vddcal;
static uint16_t Vtemp25;
static uint16_t Vtemp;
static uint16_t TempSlope;
static double temp;

void itoa(int num, char* str)
{
    int i;
    int isNegative;
    int start;
    int end; 

    i = 0;
    isNegative = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }


    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = (char)((rem > 9)? (rem-10) + 'a' : rem + '0');
        num = num/10;
    }


    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';


    start = 0;
    end = i - 1;
    while (start < end)
    {
        char localTemp = str[start];
        str[start] = str[end];
        str[end] = localTemp;
        start++;
        end--;
    }
}

void floatToString(double num, char *str) {
    int intPart;
    double fractionalPart;
    *str++ = 'T';
    *str++ = ':';

    /* Handling negative numbers */
    if (num < 0) {
        *str++ = '-';
        num = -num;
    }

    /* Separate the number into integer and fractional parts */
    intPart = (int)num;
    fractionalPart = num - (double)intPart;

    /* Convert integer part to string */
    itoa(intPart, str);

    /* Find the end of the integer string */
    while (*str != '\0') {
        str++;
    }

    /* Add decimal point */
    *str++ = '.';

    /* Convert fractional part to string */
    fractionalPart *= 100; /* for two decimal places */
    intPart = (int)fractionalPart;

    /* If fractional part is zero, add zeros */
    if (intPart == 0) {
        *str++ = '0';
        *str++ = '0';
    } else {
        itoa(intPart, str);
    }
}

/* Funcții sau variabile statice, locale pentru acest fișier */
static void monitor_temperature_and_play_buzzer(double temperature) {
    static int currentStage = 0;
    int newStage;

    /* Determine the new stage based on the temperature */
    if (temperature < STAGE_1_MAX_TEMP) { newStage = 1; } 
    else if (temperature < STAGE_2_MAX_TEMP) { newStage = 2; } 
    else { newStage = 3;}

    /* Check if the stage has changed */
    if (newStage != currentStage) {
        /* Update the current stage */
        currentStage = newStage;

        /* Play buzzer based on the new stage */
        switch (newStage) {
            case 1:
                BUZZER_play(500, 500); /* Frequency 500 Hz, duration 500 ms */
                break;
            case 2:
                BUZZER_play(800, 500); /* Frequency 1000 Hz, duration 500 ms */
                break;
            case 3:
                BUZZER_play(1000, 500); /* Frequency 1500 Hz, duration 500 ms */
                break;
                
        }
    }
}

void scan_temperature(void) {
    /* Funcții sau variabile statice, locale pentru acest fișier */
    static uint8_t loopcntr;
        /* Funcții sau variabile statice, locale pentru acest fișier */
    static uint8_t bandgap_vltg_cal;
        
    loopcntr++;
     
    if (!bandgap_vltg_cal) {
        adc_result += ADC0_READ(BANDGAP_VOLTAGE_CHN);
        if (loopcntr >= 16) {
            adc_result_avg = adc_result >> 4;
            /* Vddcal = (VREF_FACTOR * 1000) / adc_result_avg; */
            Vddcal = 33; /* testing */
            Vtemp25 = 7173 / Vddcal;
            TempSlope = 1657 / Vddcal;
             
            loopcntr = 0;
            adc_result = 0;
            bandgap_vltg_cal = 1;
        }
    } else {
        adc_result += ADC0_READ(TEMPERATURE_SENSOR_CHN);
        if (loopcntr >= 16) { 
            char temp_str[20];

            Vtemp = adc_result >> 4;
            temp = (25 - (((Vtemp - Vtemp25) * 100) / TempSlope));
            monitor_temperature_and_play_buzzer(temp);
            floatToString(temp, temp_str);
            UART0_Transmit_String(temp_str);
            loopcntr = 0;
            adc_result = 0;
        }
    }
    ADC0_SC1A |= ADC_SC1_ADCH(DISABLE_ADC); /* Disable module */
}
