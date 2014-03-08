#ifndef SONGS_H__
#define SONGS_H__

typedef struct
{
  const uint8_t is_stereo;
  const uint8_t *notes;
  const uint32_t notes_size;
} song_t;

void songs_init (void);
void songs_first (void);
void songs_next (void);
void songs_prev (void);
void songs_play (void);
void songs_stop (void);
void songs_playback (void);

#endif /* SONGS_H__ */
