#ifndef __GAMEPAD_BUTTONS_H__
#define __GAMEPAD_BUTTONS_H__

#include <stdio.h>

static FILE *gamepad;

int gamepad_init (void);
void sig_handler (int signo);

#endif