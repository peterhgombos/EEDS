#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

static int gamepad_init (void);
static void sig_handler (int signo);

static FILE *gamepad;

static void sig_handler (int signo)
{
        (void) signo;

        printf("sig_handler()\n");
        /*printf("%s", fgetc(gamepad));*/
}

static int gamepad_init (void)
{
        if (!(gamepad = fopen ("/dev/gamepad", "rb"))                   ||
            (signal(SIGIO, &sig_handler) == SIG_ERR)                    ||
            (fcntl(fileno(gamepad), F_SETOWN, getpid()) == -1)          ||
            (fcntl(fileno(gamepad), F_SETFL,
                  fcntl(fileno(gamepad), F_GETFL) | FASYNC) == -1)) {
                printf("Couldn't initialize gamepad\n");
                goto err;
        }

        printf("Gamepad initialized\n");

        return EXIT_SUCCESS;
err:
        signal(SIGIO, SIG_DFL);
        (void) fclose (gamepad);

        return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
        printf ("Hello World, I'm game!\n");


        /* Open gamepad char device */
        if (gamepad_init () == EXIT_FAILURE) {
                exit (EXIT_FAILURE);
        }

        while(1) {
                sleep(1);
        }

        exit (EXIT_SUCCESS);
}
