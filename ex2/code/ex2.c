#include <stdint.h>
#include <stdbool.h>

#include "buttons.h"
#include "dac.h"
#include "fsm.h"
#include "gpio.h"
#include "songs.h"
#include "timer.h"

#include "efm32gg.h"

/* The period between sound samples, in clock cycles. Set to 44.1kHz */
#define   SAMPLE_PERIOD   317
#define   BUTTON_PERIOD   11200

/* Declaration of peripheral setup functions */
void setupNVIC();

int main(void) 
{  
  /* Call the peripheral setup functions */
  gpio_init();
  timer1_init(SAMPLE_PERIOD);
  timer2_init(BUTTON_PERIOD);

  dac_init();

  /* Enable interrupt handling */
  setupNVIC();

  songs_init();
  fsm_init();

  *SCR |= SCR_SLEEPONEXIT;    /* Sleep on return from an ISR */
  *SCR |= SCR_SLEEPDEEP;      /* Use energy mode 2 when sleeping */

  while(1)
  {
    fsm_update();
    __asm__("wfi");
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
