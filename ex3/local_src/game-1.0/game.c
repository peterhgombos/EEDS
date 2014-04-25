#include <allegro.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include "gameobjects.c"

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320

BITMAP *buffer;

int H4CK3R_BL4CK;
int H4CK3R_GR33N;

puck player1;
puck player2;
ball pong;

static int DEVELOPMENT = 1;

static int gamepad_init (void);
static void sig_handler (int signo);
static FILE *gamepad;

static void sig_handler (int signo)
{
    (void) signo;
    int input = fgetc(gamepad);
    int i;

    for (i = 0; i < 8; i++) {
        if (input & 1)
            printf("1");
        else
            printf("0");

        input >>= 1;
    }
    printf("\n");
}

static int gamepad_init (void)
{
    if (!(gamepad = fopen ("/dev/gamepad", "rb"))                   ||
            (signal(SIGIO, &sig_handler) == SIG_ERR)                    ||
            (fcntl(fileno(gamepad), F_SETOWN, getpid()) == -1)          ||
            (fcntl(fileno(gamepad), F_SETFL,
                   fcntl(fileno(gamepad), F_GETFL) | FASYNC) == -1)) {

        printf("Couldn't initialize gamepad\n");
        return EXIT_FAILURE;
    }

    printf("Gamepad initialized\n");

    return EXIT_SUCCESS;
}

void init_allegro (void)
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    clear_to_color(buffer, H4CK3R_BL4CK);
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
    H4CK3R_BL4CK = makecol(0, 0, 0);
    H4CK3R_GR33N = makecol(0, 255, 0);

    int puck_height = SCREEN_HEIGHT / 5;
    int puck_width = 5;
    int edge_distance = 20;

    player1 = (puck) {
        .width = puck_width,
        .height = puck_height,
        .pos = {
            .x = edge_distance,
            .y = SCREEN_HEIGHT / 2 - puck_height / 2
        }
    };

    player2 = (puck) {
        .width = puck_width,
        .height = puck_height,
        .pos = {
            .x = SCREEN_WIDTH - edge_distance - puck_width,
            .y = SCREEN_HEIGHT / 2 - puck_height / 2
        }
    };
}

void get_input (void)
{

}

void draw_game (void)
{
    draw_puck(player1);
    draw_puck(player2);
    draw_sprite(screen, buffer, 0, 0);
}

/* Draw utility functions */
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

void game_loop (void)
{
    while(1) {
        get_input();
        draw_game();
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    game_init();
    game_loop();
    exit (EXIT_SUCCESS);
}
END_OF_MAIN();
