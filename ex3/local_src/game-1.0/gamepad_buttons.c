#include "gamepad_buttons.h"
#include "game.h"

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


extern int player_buttons[8];

void sig_handler (int signo)
{
    (void) signo;
    int input = fgetc(gamepad);
    player_buttons[PLAYER_1_UP] = input & (1 << 1);
    player_buttons[PLAYER_1_DOWN] = input & (1 << 3);
    player_buttons[PLAYER_2_UP] = input & (1 << 5);
    player_buttons[PLAYER_2_DOWN] = input & (1 << 7);

}

int gamepad_init (void)
{
    if (!(gamepad = fopen ("/dev/gamepad", "rb"))                       ||
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
