#include "gamepad_buttons.h"

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

static void sig_handler (int signo)
{
    (void) signo;
    cached_gamepad_buttons = fgetc(gamepad);
}

static int gamepad_init (void)
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
