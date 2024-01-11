#include "Adc.h"
#include "Uart.h"
#include "Buzzer.h"

static int adc_cal(void);


/*  Funcția de inițializare a modulului ADC0  configurarea registrilor*/
void ADC0_Init(void)
{
    int cal_res;
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
    do
    {
        /* Calibrate ADC */
        cal_res = adc_cal();
    } while (cal_res == -1);
 
    /* Configure ADC */
    ADC0_SC1A |= (ADC_SC1_AIEN_MASK);  /* Interrupt enabled */
    ADC0_SC1A &= ~ADC_SC1_DIFF_MASK;   /* Single Ended ADC */
    ADC0_CFG1 = 0; /* Reset register */
    ADC0_CFG1 |= (ADC_CFG1_MODE(2)  |   /* 10 bits mode */
                        ADC_CFG1_ADICLK(1)|   /* Input Bus Clock/2 (24 Mhz) */
                        ADC_CFG1_ADIV(3)  |     /* Clock divide by 8 (3 Mhz) */
                                ADC_CFG1_ADLPC_MASK); /* Low power mode */
    ADC0_SC3  &= ~ADC_SC3_AVGE_MASK;
}

/*  Funcția de calibrare ADC0 */
static int adc_cal(void)
{
    uint16_t calib;
     
    ADC0_CFG1 |= (ADC_CFG1_MODE(3)  |        /* 16 bits mode */
                        ADC_CFG1_ADICLK(1)|  /* Input Bus Clock divided by 2 (48Mhz / 2) */
                        ADC_CFG1_ADIV(3));   /* Clock divide by 8 (3 MHz) */
     
    ADC0_SC3  |= (ADC_SC3_AVGE_MASK |    /* Enable HW average */
                        ADC_SC3_AVGS(3)   |  /* Set HW average of 32 samples */
                        ADC_SC3_CAL_MASK);   /* Start calibration process */
     
    while (ADC0_SC3 & ADC_SC3_CAL_MASK); /* Wait for calibration to end */
     
    if (ADC0_SC3 & ADC_SC3_CALF_MASK)   /* Check for successful calibration */
        return -1; 
     
    calib = 0;
    calib += ADC0_CLPS + ADC0_CLP4 + ADC0_CLP3 + ADC0_CLP2 + ADC0_CLP1 + ADC0_CLP0;
    calib /= 2;
    calib |= 0x8000;    /* Set MSB */
    ADC0_PG = calib;
     
    calib = 0;
    calib += ADC0_CLMS + ADC0_CLM4 + ADC0_CLM3 + ADC0_CLM2 + ADC0_CLM1 + ADC0_CLM0;
    calib /= 2;
    calib |= 0x8000;    /* Set MSB */
    ADC0_MG = calib;
     
    return 0;
}

/*  Funcția de citire a ADC-ului*/
uint16_t ADC0_READ(uint8_t ch)
{
    ADC0_SC1A = ((ch & ADC_SC1_ADCH_MASK) |  (ADC0_SC1A &   (ADC_SC1_AIEN_MASK | ADC_SC1_DIFF_MASK))); 
    while(ADC0_SC2 & ADC_SC2_ADACT_MASK);    /* Conversion in progress */
    while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); /* Run until the conversion is complete*/
    return (uint16_t)ADC0_RA;
}
