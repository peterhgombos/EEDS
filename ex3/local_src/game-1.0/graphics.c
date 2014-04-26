#include "graphics.h"
#include "defines.h"

#include <stdlib.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mmap.h>


char *fbp;

void init_graphics (void)
{
    if (!DEVELOPMENT) {
        int fbfd = 0;
        long int screensize = 153600;

        /* open framebuffer device for reading */
        fbfd = open("/dev/fb0", O_RDRW);

        /* map screen to memory region */
        fbp = (char *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, 0);
    }

}

void draw_pixel(int x, int y, int color)
{
    int location = x * 2 + y * 319;
    *(fbp + location) = color;
}

void draw_rectangle(position pos, int height, int width, int color)
{
    if (DEVELOPMENT) {
        //printf("x: %d, y:, h: %d, w: %d\n", pos.x, pos.y, width, height);
        rectfill(buffer, pos.x, pos.y, pos.x + width, pos.y + height, color);
    }
}

void draw_puck(puck p)
{
    // TODO: Only redraw if position changed
    if (DEVELOPMENT) {
        draw_rectangle(p.pos, p.height, p.width, H4CK3R_GR33N);
    }
}
