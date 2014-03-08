#include <stdint.h>

#include "fsm.h"
#include "efm32gg.h"

#include "queue.h"
#include "songs.h"

static uint8_t st_init_ev_any (void)
{
  /* TODO: Play startup sound */
  /* TODO: Select first song */
  return ST_PAUSED;
}

static uint8_t st_paused_ev_next (void)
{
  songs_next();
  return ST_PAUSED;
}

static uint8_t st_paused_ev_prev (void)
{
  /* TODO: Select previous song */
  songs_prev();
  return ST_PAUSED;
}

static uint8_t st_paused_ev_start (void)
{
  /* TODO: Start playback */
  songs_play();
  return ST_PLAYING;
}

static uint8_t st_playing_ev_next (void)
{
  songs_next();
  return ST_PLAYING;
}

static uint8_t st_playing_ev_prev (void)
{
  songs_prev();
  return ST_PLAYING;
}

static uint8_t st_playing_ev_start (void)
{
  songs_play();
  return ST_PLAYING;
}

static uint8_t st_playing_ev_stop (void)
{
  songs_stop();
  return ST_PAUSED;
}

static uint8_t st_any_ev_any (void)
{
  songs_stop();
  return ST_PAUSED;
}

fsm_transition_t trans[] = {
      { ST_PAUSED,  EV_NEXT,    &st_paused_ev_next    },
      { ST_PAUSED,  EV_PREV,    &st_paused_ev_prev    },
      { ST_PAUSED,  EV_START,   &st_paused_ev_start   },
      { ST_PLAYING, EV_NEXT,    &st_playing_ev_next   },
      { ST_PLAYING, EV_PREV,    &st_playing_ev_prev   },
      { ST_PLAYING, EV_START,   &st_playing_ev_start  },
      { ST_PLAYING, EV_STOP,    &st_playing_ev_stop   },
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
  fsm_event_put(EV_ANY);
}

uint8_t fsm_update (void)
{
  fsm_event_t event = -1;
  uint8_t ret_val = 0;

  // Receive event from queue
  if (!queue_pop(&event_queue, &event))
  {
    return ret_val;
  }

  for (int i = 0; i < TRANS_COUNT; i++)
  {
    if ((state == trans[i].st) || (ST_ANY == trans[i].st))
    {
      if ((event == trans[i].ev) || (EV_ANY == trans[i].ev))
      {
        state = (trans[i].fn)();
        ret_val = 1;
        break;
      }
    }
  }

  return ret_val;
}

uint8_t fsm_event_put (fsm_event_t event)
{
    return queue_push (&event_queue, event);
}

uint8_t fsm_get_state (void)
{
    return state;
}
