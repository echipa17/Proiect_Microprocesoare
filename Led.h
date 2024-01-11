#include "MKL25Z4.h"
#define RED_LED_PIN (18)
#define GREEN_LED_PIN (19)
#define BLUE_LED_PIN (1)
typedef enum {LIGHT_WHITE,LIGHT_RED,LIGHT_PURPLE,LIGHT_YELLOW,NUM_STATES} LedState;
extern volatile LedState currentLedState;
void PIT_Init(void);
void PIT_IRQHandler(void);
void RRGBLED_init(void);
void Change_LED_PIT_IRQHandler(void);