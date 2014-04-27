#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define SCREENSIZE_BYTES 153600
#define SCREENSIZE_PIXELS 76800

/* Graphics functions */
void graphics_update (void);
void draw_pixel(int x, int y, int color);
void draw_puck(puck *p);
void draw_paddle(paddle *p);
void draw_rectangle(position pos, int height, int width, int color);
void draw_number (int offset_x, int offset_y, int number);

#if DEVELOPMENT == 0
void init_graphics (void);
void deinit_graphics (void);
void set_solid_color (int color);
#endif

#endif
