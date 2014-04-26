#include "game.h"
#include "gamepad_buttons.h"
#include "defines.h"
#include "graphics.h"

#if DEVELOPMENT
#include <allegro.h>
#endif

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if DEVELOPMENT
int H4CK3R_BL4CK;
int H4CK3R_GR33N;
#endif

paddle *player1;
paddle *player2;
puck *pong;


#if DEVELOPMENT
BITMAP *buffer;
void init_allegro (void)
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    clear_to_color(buffer, H4CK3R_BL4CK);
}
#endif

paddle *paddle_factory (int width, int height, int x, int y)
{
    paddle *p = malloc(sizeof(paddle));
    *p = (paddle) {
        .width = width,
        .height = height,
        .pos = {
            .x = x,
            .y = y
        }
    };

    return p;
}

puck *puck_factory (int radius, int direction_x, int direction_y, int x, int y)
{
    puck *p = malloc(sizeof(puck));
    *p = (puck) {
        .radius = radius,
        .direction = {
            .x = direction_x,
            .y = direction_y
        },
        .pos = {
            .x = x,
            .y = y
        }
    };

    return p;
}

void game_init (void)
{
    #if DEVELOPMENT
        init_allegro();
        H4CK3R_BL4CK = makecol(0, 0, 0);
        H4CK3R_GR33N = makecol(0, 255, 0);
    #else
        H4CK3R_BL4CK = HACKERBLACK;
        H4CK3R_GR33N = HACKERGREEn;
        /*
        if (gamepad_init () == EXIT_FAILURE) {
            exit (EXIT_FAILURE);
        }
        */
    #endif
    srand(time(NULL));

    int paddle_height = SCREEN_HEIGHT / 5;
    int paddle_width = 5;
    int edge_distance = 20;

    player1 = paddle_factory(
            paddle_width,
            paddle_height,
            edge_distance,
            SCREEN_HEIGHT / 2 - paddle_height / 2);

    player2 = paddle_factory(
            paddle_width,
            paddle_height,
            SCREEN_WIDTH - edge_distance - paddle_width,
            SCREEN_HEIGHT / 2 - paddle_height / 2);

    pong = puck_factory(5, DIRECTION_RIGHT, DIRECTION_UP, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void get_input (void)
{
    #if DEVELOPMENT
        player_buttons[PLAYER_1_UP] = key[KEY_UP];
        player_buttons[PLAYER_1_DOWN] = key[KEY_DOWN];

        player_buttons[PLAYER_2_UP] = key[KEY_W];
        player_buttons[PLAYER_2_DOWN] = key[KEY_S];

    #else
        // Parse cached values from gamepad drivers
    #endif
    
}

void draw_game (void)
{
    #if DEVELOPMENT
    clear_to_color(buffer, H4CK3R_BL4CK);
    #endif
    // TODO: clear screen on redraw
    draw_paddle(player1);
    draw_paddle(player2);
    draw_puck(pong);
}

void draw_paddle(paddle *p)
{
    // TODO: Only redraw if position changed
    draw_rectangle(p->pos, p->height, p->width, H4CK3R_GR33N);
}



void draw_puck(puck *p)
{
    draw_rectangle(p->pos, p->radius, p->radius, H4CK3R_GR33N);
}

void update (void)
{
    move_paddle(player1, PLAYER_1_UP, PLAYER_1_DOWN);
    move_paddle(player2, PLAYER_2_UP, PLAYER_2_DOWN);
    move_puck(pong);
}

void move_paddle (paddle *p, int player_up, int player_down)
{
    if (player_buttons[player_up] == player_buttons[player_down]) {
        return;
    }

    if (player_buttons[player_up] && p->pos.y > 0) {
        p->pos.y--;
    }

    if (player_buttons[player_down] && p->pos.y < SCREEN_HEIGHT - p->height - 1) {
        p->pos.y++;
    }
}

void move_puck (puck *p)
{
    if (p->pos.y == 0 || p->pos.y == SCREEN_HEIGHT - p->radius - 1) {
        p->direction.y *= -1;
    }

    if (paddle_puck_overlap(player1, pong) || paddle_puck_overlap(player2, pong)) {
        p->direction.x *= -1;
    }

    p->pos.y += p->direction.y;
    p->pos.x += p->direction.x;
}

int paddle_puck_overlap (paddle *pa, puck *pu)
{
    if (pu->pos.x < pa->pos.x + pa->width &&
            pu->pos.x + pu->radius > pa->pos.x &&
            pu->pos.y < pa->pos.y + pa->height &&
            pu->pos.y + pu->radius > pa->pos.y) {
        return YES;
    } else {
        return NO;
    }
}

void game_loop (void)
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long previous = round(spec.tv_nsec / 1.0e6) + spec.tv_sec * 1000;
    long lag = 0;
    long MS_PER_UPDATE = 16;

    while(YES) {

        clock_gettime(CLOCK_REALTIME, &spec);
        long current = round(spec.tv_nsec / 1.0e6) + spec.tv_sec * 1000;
        long elapsed = current - previous;
        previous = current;
        lag += elapsed;

        get_input();

        while (lag >= MS_PER_UPDATE)
        {
            update();
            lag -= MS_PER_UPDATE;
        }

        draw_game();

        long sleep = MS_PER_UPDATE - lag;
        sleep > 0 && usleep(sleep * 1000);
    }
}

int main(int argc, char *argv[])
{
    game_init();
    game_loop();
    exit (EXIT_SUCCESS);
}
END_OF_MAIN();
