#include <stdint.h>
#include <stdlib.h>

#include "efm32gg.h"

#include "songs.h"
#include "resources/airhorn.h"

#define QUEUE_SIZE 5

unsigned char *sample;
static song_t songs[QUEUE_SIZE];
static uint8_t position;

void songs_init (void)
{
}

void songs_first (void)
{
  position = 0;
  *GPIO_PA_DOUT = (position << 8);
}

void songs_next (void)
{
  if (QUEUE_SIZE > position)
  {
    position++;
  }
  else
  {
    position = 0;
  }

  *GPIO_PA_DOUT = (position << 8);
}

void songs_prev (void)
{
  if (0 < position)
  {
    position--;
  }
  else
  {
    position = QUEUE_SIZE;
  }
  *GPIO_PA_DOUT = (position << 8);
}

void songs_play (void)
{
}

void songs_stop (void)
{
}

void songs_playback (void)
{
}
