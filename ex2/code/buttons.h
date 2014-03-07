#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>

#define BUTTON_LEFT_LEFT   0
#define BUTTON_LEFT_UP     1
#define BUTTON_LEFT_RIGHT  2
#define BUTTON_LEFT_DOWN   3

#define BUTTON_RIGHT_LEFT   4
#define BUTTON_RIGHT_UP     5
#define BUTTON_RIGHT_RIGHT  6
#define BUTTON_RIGHT_DOWN   7

void buttons_debounce (void);
void buttons_update (uint8_t button);

#endif /* BUTTONS_H_ */
