#include "game.h"
#include "graphics.h"
#include "defines.h"

#if DEVELOPMENT
#include <allegro.h>
#endif

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


extern int H4CK3R_GR33N;
extern int H4CK3R_BL4CK;
uint16_t *fbp;
int fbfd;
struct fb_copyarea fb_rect;
#if DEVELOPMENT
extern BITMAP *buffer;
#endif

#if DEVELOPMENT == 0
void init_graphics (void)
{
        fb_rect.dx = 0;
        fb_rect.dy = 0;
        fb_rect.width = 320;
        fb_rect.height = 240;

        /* open framebuffer device for reading */
        fbfd = open("/dev/fb0", O_RDWR);
        if (fbfd == -1){ 
            perror("Could not open framebuffer");
            exit(1);
        }

        /* map screen to memory region */
        fbp = (uint16_t *) mmap(NULL, SCREENSIZE_BYTES, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd,0);
        if ((int) fbp == -1){
            perror("Could not map framebuffer to memory");
            exit(2);
        }
        set_solid_color(H4CK3R_BL4CK);
}
#endif

void refresh_fb(void)
{
    ioctl(fbfd, 0x4680, &fb_rect);
}

void draw_pixel(int x, int y, int color)
{
    int location = x  + y * 320;
    fbp[location] = color;
}

void draw_rectangle(position pos, int height, int width, int color)
{
    #if DEVELOPMENT
        rectfill(buffer, pos.x, pos.y, pos.x + width, pos.y + height, color);
    #else
        for (int i = pos.x; i < pos.x + width; i++){
            for (int j = pos.y; j < pos.y + height + 1; j++){
                draw_pixel(i, j, color);
            }
        }
    #endif
}

void draw_puck(puck *p)
{
    // TODO: Only redraw if position changed
    draw_rectangle(p->pos, p->radius, p->radius, H4CK3R_GR33N);
}

void draw_paddle(paddle *p)
{
    // TODO: Only redraw if position changed
    draw_rectangle(p->pos, p->height, p->width, H4CK3R_GR33N);
}

#if DEVELOPMENT == 0
void set_solid_color(int color)
{
    for (int i = 0; i < SCREENSIZE_PIXELS; i++) {
        fbp[i] = color;
    }
}
#endif
