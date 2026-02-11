#ifndef DRIVERS_FRAMEBUFFER_H
#define DRIVERS_FRAMEBUFFER_H

#include "kernel/types.h"

typedef struct {
    void*    addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;
    uint8_t  red_shift;
    uint8_t  green_shift;
    uint8_t  blue_shift;
} fb_info_t;

void fb_init(void* multiboot_info);
void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
uint32_t fb_make_color(uint8_t r, uint8_t g, uint8_t b);
fb_info_t* fb_get_info(void);

#endif
