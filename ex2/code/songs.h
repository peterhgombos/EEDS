#ifndef SONGS_H__
#define SONGS_H__

typedef struct
{
  uint8_t is_stereo;
  uint8_t *notes;
  uint32_t notes_size;
} song_t;

void songs_init (void);
void songs_first (void);
void songs_next (void);
void songs_prev (void);
void songs_play (void);
void songs_pause (void);
void songs_stop (void);
void songs_playback (void);
song_t songs_current_get (void); 

#endif /* SONGS_H__ */
