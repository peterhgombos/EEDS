#include <stdint.h>
#include <stdlib.h>

#include "dac.h"
#include "timer.h"
#include "songs.h"
#include "efm32gg.h"

#include "resources/airhorn.h"
//#include "resources/wishyou.h"

#define SONG_QUEUE_SIZE 1 /* Current logic assumes max queue size of 8 */

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
  //songs[1] = wishyou;
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
