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

#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1
#define DIRECTION_UP -1
#define DIRECTION_DOWN 1

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
    position direction;
    position pos;
}
puck;

paddle *paddle_factory (int width, int height, int x, int y);
puck *puck_factory (int radius, int direction_x, int direction_y, int x, int y);

void game_init (void);
void game_loop (void);
void update (void);

void move_paddle (paddle *p, int player_up, int player_down);
void move_puck (puck *p);

int paddle_puck_overlap (paddle *pa, puck *pu);
void draw_paddle(paddle *p);
void draw_puck(puck *p);
void draw_rectangle(position pos, int height, int width, int color);

#endif
