#include "game.h"
#include "gamepad_buttons.h"
#include "defines.h"
#include "graphics.h"

#if DEVELOPMENT
#include <allegro.h>
#endif

#include <time.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SCORE 9

#define MS_PER_UPDATE 42
#define PX_PER_TICK 3

#define EDGE_DISTANCE 20
#define PADDLE_WIDTH 5
#define PADDLE_HEIGHT SCREEN_HEIGHT / 5

int H4CK3R_BL4CK;
int H4CK3R_GR33N;
static int SCREEN_SAFE_WIDTH = EDGE_DISTANCE + PADDLE_WIDTH;

int player_buttons[8];

int player1_score;
int player2_score;

bool running;

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

void allegro_get_input (void)
{
    player_buttons[PLAYER_1_UP] = key[KEY_UP];
    player_buttons[PLAYER_1_DOWN] = key[KEY_DOWN];

    player_buttons[PLAYER_2_UP] = key[KEY_W];
    player_buttons[PLAYER_2_DOWN] = key[KEY_S];
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
        },
        .pos_prev = {
            .x = 0,
            .y = SCREEN_HEIGHT / 2
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
        },
        .pos_prev = {
            .x = 0,
            .y = 0
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
        H4CK3R_GR33N = HACKERGREEN;
        graphics_init ();

        if (gamepad_init () == EXIT_FAILURE) {
            exit (EXIT_FAILURE);
        }
    #endif
    srand(time(NULL));

    player1 = paddle_factory(
            PADDLE_WIDTH,
            PADDLE_HEIGHT,
            EDGE_DISTANCE,
            SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2);

    player2 = paddle_factory(
            PADDLE_WIDTH,
            PADDLE_HEIGHT,
            SCREEN_WIDTH - EDGE_DISTANCE - PADDLE_WIDTH,
            SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2);

    pong = puck_factory(5, DIRECTION_RIGHT, DIRECTION_UP, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    player1_score = 0;
    player2_score = 0;
}

void game_deinit (int signo)
{
    graphics_deinit ();

    free(player1);
    free(player2);
    free(pong);

    running = false;
}

void draw_scores (void)
{
    draw_number (3, 3, player1_score);
    draw_number (SCREEN_WIDTH - 17, 3, player2_score);

    #if DEVELOPMENT
    draw_sprite (screen, buffer, 0, 0);
    #else
    graphics_update ();
    #endif
}

void player_scored (void)
{
    if (pong->pos.x > 10) {
        player1_score++;
    } else {
        player2_score++;
    }

    pong->pos.x = SCREEN_WIDTH / 2;
    pong->pos.y = SCREEN_HEIGHT / 2;

    player1->pos.x = EDGE_DISTANCE;
    player1->pos.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;

    player2->pos.x = SCREEN_WIDTH - EDGE_DISTANCE - PADDLE_WIDTH;
    player2->pos.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;

    if (player2_score > MAX_SCORE || player1_score > MAX_SCORE) {
        player1_score = player2_score = 0;
    }

    draw_scores ();
}

void draw_game (void)
{
    draw_paddle(player1);
    draw_paddle(player2);
    draw_puck(pong);

    #if DEVELOPMENT
        draw_sprite(screen, buffer, 0, 0);
    #else
        graphics_update ();
    #endif
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
        p->pos.y -= PX_PER_TICK;
    } else if (player_buttons[player_down] && p->pos.y < SCREEN_HEIGHT - p->height - 1) {
        p->pos.y += PX_PER_TICK;
    }
}

void move_puck (puck *p)
{
    if (p->pos.y == 0 || p->pos.y == SCREEN_HEIGHT - p->radius - 1) {
        p->direction.y *= -1;
    }

    if (player1->pos.x <= SCREEN_SAFE_WIDTH &&
            paddle_puck_overlap(player1, pong) &&
            p->direction.x < 0) {
        p->direction.x *= -1;
    } else if (player2->pos.x >= SCREEN_WIDTH - SCREEN_SAFE_WIDTH &&
            paddle_puck_overlap(player2, pong) &&
            p->direction.x > 0) {
        p->direction.x *= -1;
    }

    p->pos.y += p->direction.y * PX_PER_TICK;
    p->pos.x += p->direction.x * PX_PER_TICK;

    if (p->pos.x <= 0 || p->pos.x > SCREEN_WIDTH) {
        player_scored ();
    }
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

void cp_pos_to_prev (void)
{
    player1->pos_prev = player1->pos;
    player2->pos_prev = player2->pos;
    pong->pos_prev = pong->pos;
}

void game_loop (void)
{
    struct timespec spec;
    long previous;
    long lag;
    long sleep;
    long elapsed;
    long current;

    lag = 0;
    clock_gettime(CLOCK_REALTIME, &spec);
    previous = round(spec.tv_nsec / 1.0e6) + spec.tv_sec * 1000;
    draw_scores ();

    running = true;
    while (running)
    {
        clock_gettime(CLOCK_REALTIME, &spec);
        current = round(spec.tv_nsec / 1.0e6) + spec.tv_sec * 1000;
        elapsed = current - previous;
        previous = current;
        lag += elapsed;

        #if DEVELOPMENT
        allegro_get_input();
        #endif

        while (lag >= MS_PER_UPDATE)
        {
            update();
            lag -= MS_PER_UPDATE;
        }

        draw_game();
        cp_pos_to_prev();

        sleep = MS_PER_UPDATE - lag;
        sleep > 0 && usleep(sleep * 1000);
    }
}

int main(int argc, char *argv[])
{
    struct sigaction act;

    /* Install sigterm handler */
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &game_deinit;
    act.sa_flags = SA_SIGINFO;
    (void) sigaction(SIGTERM, &act, NULL);

    game_init();
    game_loop();
    exit (EXIT_SUCCESS);
}
#if DEVELOPMENT
END_OF_MAIN();
#endif
