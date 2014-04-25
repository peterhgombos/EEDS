#include "game.h"
#include "gamepad_buttons.h"
#include "defines.h"

#include <allegro.h>

#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int H4CK3R_BL4CK;
int H4CK3R_GR33N;

puck player1;
puck player2;
ball pong;

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

puck puck_factory (int width, int height, int x, int y)
{
    return (puck) {
        .width = width,
        .height = height,
        .pos = {
            .x = x,
            .y = y
        }
    };
}

void game_init (void)
{
    if (DEVELOPMENT)
    {
        init_allegro();
    }
    else
    {
        if (gamepad_init () == EXIT_FAILURE) {
            exit (EXIT_FAILURE);
        }
    }

    srand(time(NULL));

    if (DEVELOPMENT) {
        H4CK3R_BL4CK = makecol(0, 0, 0);
        H4CK3R_GR33N = makecol(0, 255, 0);
    }

    int puck_height = SCREEN_HEIGHT / 5;
    int puck_width = 5;
    int edge_distance = 20;

    player1 = puck_factory(
            puck_width,
            puck_height,
            edge_distance,
            SCREEN_HEIGHT / 2 - puck_height / 2);

    player2 = puck_factory(
            puck_width,
            puck_height,
            SCREEN_WIDTH - edge_distance - puck_width,
            SCREEN_HEIGHT / 2 - puck_height / 2);
}

void get_input (void)
{
    if (DEVELOPMENT) {
        player_buttons[PLAYER_1_UP] = key[KEY_UP];
        player_buttons[PLAYER_1_DOWN] = key[KEY_DOWN];

        printf("Input?");
        for (int i=0; i<8; i++) {
            printf("%d", player_buttons[i]);
        }
    } else {
        // Parse cached values from gamepad drivers
    }
}

void draw_game (void)
{
    draw_puck(player1);
    draw_puck(player2);

    if (DEVELOPMENT) {
        draw_sprite(screen, buffer, 0, 0);
    }
}

void draw_puck(puck p)
{
    // TODO: Only redraw if position changed
    draw_rectangle(p.pos, p.height, p.width, H4CK3R_GR33N);
}

void draw_rectangle(position pos, int height, int width, int color)
{
    if (DEVELOPMENT) {
        //printf("x: %d, y:, h: %d, w: %d\n", pos.x, pos.y, width, height);
        rectfill(buffer, pos.x, pos.y, pos.x + width, pos.y + height, color);
    }
}

void update (void)
{
    printf("I am update :D\n");
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
        printf("Total lag: %ld. Sleeping for %ld\n", lag, sleep);
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
