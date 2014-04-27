#include "gamepad_buttons.h"
#include "game.h"

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

extern int player_buttons[8];
static FILE *gamepad;

void gamepad_interrupt_handler (int sig, siginfo_t *siginfo, void *context)
{
    int input;

    input = fgetc(gamepad);
    player_buttons[PLAYER_1_UP]     = input & (1 << 1);
    player_buttons[PLAYER_1_DOWN]   = input & (1 << 3);
    player_buttons[PLAYER_2_UP]     = input & (1 << 5);
    player_buttons[PLAYER_2_DOWN]   = input & (1 << 7);
}

int gamepad_init (void)
{
    struct sigaction act;

    if (!(gamepad = fopen ("/dev/gamepad", "rb")))
    {
        goto err;
    }

    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &gamepad_interrupt_handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGIO, &act, NULL) < 0) {
        goto err;
    }

    if (fcntl(fileno(gamepad), F_SETOWN, getpid()) == -1) {
        goto err;
    }

    if ((fcntl(fileno(gamepad), F_SETFL,
        fcntl(fileno(gamepad), F_GETFL) | FASYNC) == -1)) {
        goto err;
    }

    printf("Gamepad initialized\n");
    return EXIT_SUCCESS;

err:
    printf("Couldn't initialize gamepad\n");
    return EXIT_FAILURE;
}

void gamepad_deinit (void)
{
    /* Unregister gamepad signal handler */
    signal (SIGIO, SIG_DFL);

    fclose (gamepad);
}
