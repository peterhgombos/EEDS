#include <stdint.h>
#include <stdbool.h>

#include "fsm.h"
#include "timer.h"
#include "efm32gg.h"

#include "buttons.h"
#include "songs.h"

static uint8_t playing;

/* Timer 1 is used to feed the DAC */
void timer1_init (uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;    /* Enable clock to TIMER 1 */

  *TIMER1_TOP = period;                           /* Set interrupt period */
  *TIMER1_IEN |= (1 << 0);                        /* Enable overflow interrupt */
  *ISER0 |= (1 << 12);                            /* Enable timer 1 interrupt request line */

  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;   /* Disable clock */
}

void timer1_enable (void)
{
  playing = 1;
  *SCR &= ~SCR_SLEEPDEEP;                         /* Use energy mode 1 */

  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;    /* Enable clock */
  *TIMER1_CMD = 1;                                /* Enable timer 1 */
}

void timer1_disable (void)
{
  *TIMER1_CMD = 0;                                /* Disable timer 1 */
  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;   /* Disable clock */

  playing = 0;
  *SCR |= SCR_SLEEPDEEP;                          /* Use energy mode 2 */
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
  *SCR &= ~SCR_SLEEPDEEP;                         /* Use energy mode 1 */

  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER2;    /* Enable clock */
  *TIMER2_CMD = 1;                                /* Enable timer 2 */
}

void timer2_disable (void)
{
  *TIMER2_CMD = 0;                                /* Disable timer 2 */
  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER2;   /* Disable clock */

  if (!playing)
  {
    *SCR |= SCR_SLEEPDEEP;                          /* Use energy mode 2 */
  }
}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{
  /* Clear timer 1 interrupt */
  *TIMER1_IFC = 1;

  static int count = 0;

  song_t current_song = songs_current_get();

  if (count < current_song.notes_size)
  {
    if (current_song.is_stereo == 1)
    {
      *DAC0_CH0DATA = current_song.notes[count];
      *DAC0_CH1DATA = current_song.notes[count + 1];
      count += 2;
    }
    else
    {
      *DAC0_CH0DATA = current_song.notes[count];
      *DAC0_CH1DATA = current_song.notes[count];
      count += 1;
    }
  }
  else
  {
    count = 0;
    fsm_event_put(EV_STOP);
  }
}

/* TIMER2 interrupt handler */
void __attribute__ ((interrupt)) TIMER2_IRQHandler()
{
  /* Clear timer 2 interrupt */
  *TIMER2_IFC = 1;
  buttons_timer_irq();
}
