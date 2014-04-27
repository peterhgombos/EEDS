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
void graphics_init (void)
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
    fbp = (uint16_t *) mmap(NULL, SCREENSIZE_BYTES,
        PROT_READ | PROT_WRITE, MAP_SHARED, fbfd,0);
    if ((int) fbp == -1){
        perror("Could not map framebuffer to memory");
        exit(2);
    }

    set_solid_color(H4CK3R_BL4CK);
}

void graphics_deinit (void)
{
    /* Ignore return value, as we can't do anything about it if close fails */
    (void) close(fbfd);

    munmap (fbp, SCREENSIZE_BYTES);
}

void set_solid_color(int color)
{
    for (int i = 0; i < SCREENSIZE_PIXELS; i++) {
        fbp[i] = color;
    }

    ioctl(fbfd, 0x4680, &fb_rect);
}
#endif

void draw_pixel (int x, int y, int color)
{
    int location = x  + y * 320;
    fbp[location] = color;
}

void draw_rectangle (position pos, int height, int width, int color)
{
    #if DEVELOPMENT
        rectfill(buffer, pos.x, pos.y, pos.x + width, pos.y + height, color);
    #else
        for (int i = pos.x; i < pos.x + width; i++){
            for (int j = pos.y; j < pos.y + height + 1; j++){
                draw_pixel(i, j, color);
            }
        }

        struct fb_copyarea redraw_rect = {
          .dx = pos.x,
          .dy = pos.y,
          .width = width,
          .height = height
        };

        ioctl(fbfd, 0x4680, &redraw_rect);
    #endif
}

void draw_puck(puck *p)
{
    if (p->pos_prev.x == p->pos.x && p->pos_prev.y == p->pos.y) {
        return;
    }

    draw_rectangle(p->pos_prev, p->radius, p->radius, H4CK3R_BL4CK);
    draw_rectangle(p->pos, p->radius, p->radius, H4CK3R_GR33N);
}

void draw_paddle(paddle *p, int incremental)
{
    if (p->pos_prev.x == p->pos.x && p->pos_prev.y == p->pos.y) {
        return;
    }

    if (incremental == 0 || (p->pos_prev.x == 0 && p->pos_prev.y == 0)) {
        draw_rectangle(p->pos_prev, p->height, p->width, H4CK3R_BL4CK);
        draw_rectangle(p->pos, p->height, p->width, H4CK3R_GR33N);
        return;
    }

    if (p->pos.y > p->pos_prev.y) {
        position pos_prev_bottom = {
            .x = p->pos_prev.x,
            .y = p->pos_prev.y + p->height
        };

        draw_rectangle(p->pos_prev,
            p->pos.y - p->pos_prev.y,
            p->width,
            H4CK3R_BL4CK);
        draw_rectangle(pos_prev_bottom,
            p->pos.y + p->height - pos_prev_bottom.y,
            p->width,
            H4CK3R_GR33N);
      } else {
        position pos_bottom = {
          .x = p->pos.x,
          .y = p->pos.y + p->height
        };

        draw_rectangle(pos_bottom,
            p->pos_prev.y + p->height - pos_bottom.y,
            p->width,
            H4CK3R_BL4CK);
        draw_rectangle(p->pos,
            p->pos_prev.y - p->pos.y,
            p->width,
            H4CK3R_GR33N);
    }
}

void draw_number (int offset_x, int offset_y, int number)
{
    int CORNER_WIDTH = 2;
    int SEGMENT_HEIGHT = 12;
    int SEGMENT_WIDTH = 10;

    position origo = {offset_x, offset_y};
    position top = {offset_x + CORNER_WIDTH, offset_y};
    position middle = {offset_x + CORNER_WIDTH, offset_y + SEGMENT_HEIGHT + CORNER_WIDTH * 1};
    position bottom = {offset_x + CORNER_WIDTH, offset_y + SEGMENT_HEIGHT * 2 + CORNER_WIDTH * 2};

    position top_left = {offset_x, offset_y + CORNER_WIDTH};
    position bottom_left = {offset_x, offset_y + SEGMENT_HEIGHT + CORNER_WIDTH * 2};

    position top_right = {offset_x + SEGMENT_WIDTH + CORNER_WIDTH, offset_y + CORNER_WIDTH};
    position bottom_right = {offset_x + SEGMENT_WIDTH + CORNER_WIDTH, offset_y + SEGMENT_HEIGHT + CORNER_WIDTH * 2};

    draw_rectangle(origo,
            SEGMENT_HEIGHT * 2 + CORNER_WIDTH * 3,
            SEGMENT_WIDTH + CORNER_WIDTH * 2,
            H4CK3R_BL4CK);


    switch (number)
    {
        case 10:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            break;

        case 9:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 8:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 7:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 6:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);

            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 5:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);

            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 4:
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 3:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 2:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(middle, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(bottom_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 1:
            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;

        case 0:
            draw_rectangle(top, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom, CORNER_WIDTH, SEGMENT_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_left, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);

            draw_rectangle(top_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            draw_rectangle(bottom_right, SEGMENT_HEIGHT, CORNER_WIDTH, H4CK3R_GR33N);
            break;
    }
}
