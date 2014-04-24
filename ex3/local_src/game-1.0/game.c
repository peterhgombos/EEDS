#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include "gameobjects.c"

#define SCREEN_HEIGHT 20
#define SCREEN_WIDTH 320

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

int main(int argc, char *argv[])
{
        printf("Hello World, I'm game!\n");

        /* Open gamepad char device */
        if (gamepad_init () == EXIT_FAILURE) {
                exit (EXIT_FAILURE);
        }

        while(1) {
                sleep(1);
        }

        exit (EXIT_SUCCESS);
}
