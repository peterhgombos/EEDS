#include <stdint.h>

#include "buttons.h"
#include "efm32gg.h"

#include "fsm.h"
#include "timer.h"

#define MAX_CHECKS 10
#define NUM_BUTTONS 8

volatile uint8_t debounced_state;
volatile uint8_t prev_debounced_state = 0xFF;
volatile uint8_t timer_counter;

uint16_t state[MAX_CHECKS];
uint16_t btn_index;

static inline uint8_t m_raw_state (void)
{
    /* Only select switches from the port */
    return ~(*GPIO_PC_DIN);
}

static void m_update (uint8_t button)
{
  fsm_event_t event;

  if ((debounced_state & (1 << button)) && (~prev_debounced_state & (1 << button)))
  {
    switch(button)
    {
      case BUTTON_LEFT_UP:
        event = EV_START;
        break;
      case BUTTON_LEFT_LEFT:
        event = EV_PREV;
        break;
      case BUTTON_LEFT_RIGHT:
        event = EV_NEXT;
        break;
      default:
        return;
    }
    
    fsm_event_put(event);
  }
}

static void m_debounce (void)
{
  uint8_t i,j;

  prev_debounced_state = debounced_state;
  state[btn_index++] = m_raw_state ();

  for (i = 0, j = 0xFF; i < MAX_CHECKS; i++)
  {
    j = j & state[i];
  }

  debounced_state = j;

  if (btn_index >= MAX_CHECKS)
  {
    btn_index = 0;
  }
}

void buttons_gpio_irq (void)
{
  timer2_enable();
}

void buttons_timer_irq (void)
{
  /* Disable timer if required */
  if (MAX_CHECKS == timer_counter++)
  {
    timer2_disable();
    return;
  }

  m_debounce();
  m_update (BUTTON_LEFT_LEFT);
  m_update (BUTTON_LEFT_UP);
  m_update (BUTTON_LEFT_RIGHT);
}
