#ifndef __GAME_H__
#define __GAME_H__

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
paddle;

typedef struct
{
    int radius;
    position pos;
}
puck;

paddle *paddle_factory (int width, int height, int x, int y);
void game_init (void);
void game_loop (void);
void update (void);
void move_paddle (paddle *p, int player_up, int player_down);
void draw_paddle(paddle *p);
void draw_rectangle(position pos, int height, int width, int color);

#endif
