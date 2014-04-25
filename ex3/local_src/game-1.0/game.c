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

void game_init (void)
{

}

int main(int argc, char *argv[])
{
    /* Open gamepad char device */
    //if (gamepad_init () == EXIT_FAILURE) {
    //exit (EXIT_FAILURE);
    //}

    srand(time(NULL));
    H4CK3R_BL4CK = makecol(0, 0, 0);
    H4CK3R_GR33N = makecol(0, 255, 0);

    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    clear_to_color(buffer, H4CK3R_BL4CK);

    while(1) {
        draw_rectangle(10, 10, 40, 40);
        draw_sprite(screen, buffer, 0, 0);

        sleep(1);
    }

    exit (EXIT_SUCCESS);
}
END_OF_MAIN();

void draw_rectangle(int height, int width, int x, int y)
{
    putpixel(buffer, x, y, makecol(255, 255, 255));
}
