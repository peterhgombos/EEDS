#ifndef __GAME_H__
#define __GAME_H__

#define YES 1
#define NO 0
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320
#define DEVELOPMENT 1

#define PLAYER_1_UP 0
#define PLAYER_1_DOWN 1
#define PLAYER_1_LEFT 2
#define PLAYER_1_RIGHT 3
#define PLAYER_2_UP 4
#define PLAYER_2_DOWN 5
#define PLAYER_2_LEFT 6
#define PLAYER_2_RIGHT 7

static int player_buttons[8];
static int cached_gamepad_buttons = 0;

typedef struct
{
    int x;
    int y;
}
position;

typedef struct
{
    int width;
    int height;
    position pos;
}
puck;

typedef struct
{
    int radius;
    position pos;
}
ball;

puck puck_factory (int width, int height, int x, int y);
void game_init (void);
void game_loop (void);
void update (void);
void draw_puck(puck p);
void draw_rectangle(position pos, int height, int width, int color);

#endif
