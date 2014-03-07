#include <stdint.h>
#include <stdbool.h>

#include "fsm.h"

#include "efm32gg.h"

/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles. Set to 44.1kHz */
#define   SAMPLE_PERIOD   317

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();

int main(void) 
{  
  /* Call the peripheral setup functions */
  setupGPIO();
  setupDAC();
  setupTimer(SAMPLE_PERIOD);

  /* Enable interrupt handling */
  setupNVIC();

  fsm_init();

  while(1)
  {
    fsm_update();
  }

  return 0;
}

void setupNVIC()
{
  /* Enable Timer 1 interrupt */
  *ISER0 |= (1 << IRQ_TIMER1);
  *ISER0 |= (1 << IRQ_GPIO_EVEN);
  *ISER0 |= (1 << IRQ_GPIO_ODD);
}
