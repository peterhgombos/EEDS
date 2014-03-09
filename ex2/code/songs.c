#include <stdint.h>
#include <stdlib.h>

#include "dac.h"
#include "timer.h"
#include "songs.h"
#include "efm32gg.h"

#include "resources/airhorn.h"
#include "resources/smw1_1up_converted.h"
#include "resources/smw_coin.h"

#define SONG_QUEUE_SIZE 2 /* Current logic assumes max queue size of 8 */

unsigned char *sample;
static song_t songs[SONG_QUEUE_SIZE];
static uint8_t position;

static void m_set_led_output(void)
{
  *GPIO_PA_DOUT = ~(1 << (8 + position));
}

void songs_init (void)
{
  songs[0] = airhorn;
  songs[1] = smw1_1up_converted;
  songs[2] = smw_coin;

  m_set_led_output();
}

void songs_next (void)
{
  if (SONG_QUEUE_SIZE > position)
  {
    position++;
  }
  else
  {
    position = 0;
  }

  m_set_led_output();
}

void songs_prev (void)
{
  if (0 < position)
  {
    position--;
  }
  else
  {
    position = SONG_QUEUE_SIZE;
  }

  m_set_led_output();
}

void songs_play (void)
{
  dac_enable();
  timer1_enable();
}

void songs_pause (void)
{
  timer1_disable();
}

void songs_stop (void)
{
  timer1_disable();
  dac_disable();
}

void songs_playback (void)
{
}

song_t songs_current_get (void)
{
  return songs[position];
}
