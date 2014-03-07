#include <stdint.h>

#include "efm32gg.h"

#include "queue.h"
#include "fsm.h"

static uint8_t st_init_ev_any (void)
{
  /* TODO: Play startup sound */
  /* TODO: Select first song */
  *GPIO_PA_DOUT = 1 << 8;
  return ST_PAUSED;
}

static uint8_t st_paused_ev_next (void)
{
  /* TODO: Select next song */
  //*GPIO_PA_DOUT = 0x0F00;

  *GPIO_PA_DOUT = 2 << 8;
  return ST_PAUSED;
}

static uint8_t st_paused_ev_prev (void)
{
  /* TODO: Select previous song */
  //*GPIO_PA_DOUT = 0x00F0;

  *GPIO_PA_DOUT = 3 << 8;
  return ST_PAUSED;
}

static uint8_t st_paused_ev_start (void)
{
  /* TODO: Start playback */

  *GPIO_PA_DOUT = 4 << 8;
  return ST_PLAYING;
}

static uint8_t st_playing_ev_next (void)
{
  /* TODO: Select next song */

  *GPIO_PA_DOUT = 4 << 8;
  return ST_PLAYING;
}

static uint8_t st_playing_ev_prev (void)
{
  /* TODO: Select previous song */

  *GPIO_PA_DOUT = 5 << 8;
  return ST_PLAYING;
}

static uint8_t st_playing_ev_start (void)
{
  /* TODO: Select previous song */

  *GPIO_PA_DOUT = 6 << 8;
  return ST_PLAYING;
}

static uint8_t st_playing_ev_finish (void)
{
  *GPIO_PA_DOUT = 7 << 8;
  return ST_PAUSED;
}

static uint8_t st_any_ev_any (void)
{
  /* TODO: Disable playback */
  /* TODO: Select first song */

  *GPIO_PA_DOUT = 8 << 8;
  return ST_PAUSED;
}

fsm_transition_t trans[] = {
      { ST_PAUSED,  EV_NEXT,    &st_paused_ev_next    },
      { ST_PAUSED,  EV_PREV,    &st_paused_ev_prev    },
      { ST_PAUSED,  EV_START,   &st_paused_ev_start   },
      { ST_PLAYING, EV_NEXT,    &st_playing_ev_next   },
      { ST_PLAYING, EV_PREV,    &st_playing_ev_prev   },
      { ST_PLAYING, EV_START,   &st_playing_ev_start  },
      { ST_PLAYING, EV_FINISH,  &st_playing_ev_finish },
      { ST_INIT,    EV_ANY,     &st_init_ev_any       },
      { ST_ANY,     EV_ANY,     &st_any_ev_any        }
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

static queue_t event_queue;
static uint8_t state;

void fsm_init (void)
{
  state = ST_INIT;
  queue_init (&event_queue);
}

void fsm_update (void)
{
  fsm_event_t event = -1;
    
  // Receive event from queue
  if (!queue_pop(&event_queue, &event))
  {
    return;
  }

  for (int i = 0; i < TRANS_COUNT; i++)
  {
    if ((state == trans[i].st) || (ST_ANY == trans[i].st))
    {
      if ((event == trans[i].ev) || (EV_ANY == trans[i].ev))
      {
        state = (trans[i].fn)();
        break;
      }
    }
  }
}

uint8_t fsm_event_put (fsm_event_t event)
{
    return queue_push (&event_queue, event);
}

uint8_t fsm_get_state (void)
{
    return state;
}
