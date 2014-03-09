#include <stdint.h>
#include <stdlib.h>

#include "dac.h"
#include "timer.h"
#include "songs.h"
#include "efm32gg.h"

#include "resources/converted-wavs/intro.h"
#include "resources/converted-wavs/smb_bowserfalls.h"
#include "resources/converted-wavs/smb_coin.h"
#include "resources/converted-wavs/smb_pipe.h"

#define SONG_QUEUE_SIZE 4 /* Current logic assumes max queue size of 8 */

unsigned char *sample;
static song_t songs[SONG_QUEUE_SIZE];
static uint8_t song_playing;
static uint8_t song_selected;

static void m_set_led_output(void)
{
  *GPIO_PA_DOUT = ~(1 << (8 + song_selected));
}

void songs_init (void)
{
  songs[0] = intro;
  songs[1] = smb_bowserfalls;
  songs[2] = smb_coin;
  songs[3] = smb_pipe;

  m_set_led_output();
}

void songs_next (void)
{
  if (SONG_QUEUE_SIZE - 1 > song_selected)
  {
    song_selected++;
  }
  else
  {
    song_selected = 0;
  }

  m_set_led_output();
}

void songs_prev (void)
{
  if (0 < song_selected)
  {
    song_selected--;
  }
  else
  {
    song_selected = SONG_QUEUE_SIZE - 1;
  }

  m_set_led_output();
}

void songs_play (void)
{
  song_playing = song_selected;

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
  return songs[song_playing];
}
