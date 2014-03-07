#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer */
void setupTimer1(uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = period;
  *TIMER1_IEN |= (1 << 0);
  *ISER0 |= (1 << 12);
  *TIMER1_CMD |= (1 << 0);
}

void setupTimer2(uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER2;
  *TIMER2_TOP = period;
  *TIMER2_IEN |= (1 << 0);
  *ISER0 |= (1 << 13);
  *TIMER2_CMD |= (1 << 0);
}
