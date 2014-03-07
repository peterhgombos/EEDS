#include <stdint.h>

#include "efm32gg.h"

#include "buttons.h"
#include "fsm.h"

#define MAX_CHECKS 10
#define NUM_BUTTONS 8

volatile uint8_t debounced_state;
volatile uint8_t prev_debounced_state = 0xFF;
uint16_t state[MAX_CHECKS];
uint16_t btn_index;

static inline uint8_t buttons_raw_state (void)
{
    /* Only select switches from the port */
    return ~(*GPIO_PC_DIN);
}

void buttons_debounce (void)
{
  uint8_t i,j;
  prev_debounced_state = debounced_state;
  state[btn_index] = buttons_raw_state ();
  ++btn_index;
  j = 0xff;
  for(i = 0; i < MAX_CHECKS; i++)
  j = j & state[i];
  debounced_state = j;
  if (btn_index>=MAX_CHECKS) btn_index=0;
}

void buttons_update (uint8_t button)
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
