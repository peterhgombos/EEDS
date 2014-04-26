#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

/* Graphics functions */
void init_graphics (void);
void draw_pixel(int x, int y, int color);
void draw_puck(puck p);
void draw_rectangle(position pos, int height, int width, int color);

#endif
