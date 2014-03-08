#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* Timer 1 is used to feed the DAC */
void timer1_init (uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;    /* Enable clock to TIMER 1 */
  *TIMER1_TOP = period;                           /* Set interrupt period */
  *TIMER1_IEN |= (1 << 0);                        /* Enable overflow interrupt */
  *ISER0 |= (1 << 12);                            /* Enable timer 1 interrupt request line */
}

void timer1_enable (void)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;    /* Enable clock */
  *TIMER1_CMD = 1;                                /* Enable timer 1 */
}

void timer1_disable (void)
{
  *TIMER1_CMD = 0;                                /* Disable timer 1 */
  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;   /* Disable clock */
}

/* Timer 2 is used for sampling buttons */
void timer2_init (uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER2;    /* Enable clock to TIMER 2 */

  *TIMER2_TOP = period;                           /* Set interrupt period */
  *TIMER2_IEN |= (1 << 0);                        /* Enable overflow interrupt */
  *ISER0 |= (1 << 13);                            /* Enable timer 1 interrupt request line */

  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER2;   /* Disable clock */
}

void timer2_enable (void)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER2;    /* Enable clock */
  *TIMER2_CMD = 1;                                /* Enable timer 2 */
}

void timer2_disable (void)
{
  *TIMER2_CMD = 0;                                /* Disable timer 2 */
  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER2;   /* Disable clock */
}
