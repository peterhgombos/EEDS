#include <stdint.h>
#include <stdbool.h>

#include "buttons.h"
#include "fsm.h"
#include "songs.h"

#include "efm32gg.h"

/* The period between sound samples, in clock cycles. Set to 44.1kHz */
#define   SAMPLE_PERIOD   317
#define   BUTTON_PERIOD   11200

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer1(uint32_t period);
void setupTimer2(uint32_t period);
void setupDAC();
void setupNVIC();

int main(void) 
{  
  /* Call the peripheral setup functions */
  setupGPIO();
  setupDAC();
  setupTimer1(SAMPLE_PERIOD);
  setupTimer2(BUTTON_PERIOD);

  /* Enable interrupt handling */
  setupNVIC();

  songs_init();
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
  *ISER0 |= (1 << IRQ_TIMER2);
  *ISER0 |= (1 << IRQ_GPIO_EVEN);
  *ISER0 |= (1 << IRQ_GPIO_ODD);
}
