/**
 * framebuffer.c - Linear framebuffer z Multiboot2
 * NexusOS - rysowanie pikseli, prostokąty
 */

#include "kernel/types.h"
#include "kernel/multiboot.h"
#include "drivers/framebuffer.h"

static fb_info_t fb_info;
static int fb_ready = 0;

/* Parsowanie struktury Multiboot2 - szukamy tagu framebuffer (type 8) */
static void parse_multiboot2(uint32_t addr)
{
    uint32_t total_size = *(uint32_t*)(addr);
    uint32_t offset = 8;
    while (offset < total_size) {
        uint32_t type = *(uint32_t*)(addr + offset);
        uint32_t size = *(uint32_t*)(addr + offset + 4);
        if (type == MULTIBOOT_TAG_TYPE_END)
            break;
        if (type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER && size >= 24) {
            struct multiboot_tag_framebuffer* fb = (struct multiboot_tag_framebuffer*)(addr + offset);
            fb_info.addr   = (void*)(uint32_t)fb->framebuffer_addr;
            fb_info.pitch  = fb->framebuffer_pitch;
            fb_info.width  = fb->framebuffer_width;
            fb_info.height = fb->framebuffer_height;
            fb_info.bpp    = fb->framebuffer_bpp;
            if (fb_info.bpp == 32) {
                fb_info.red_shift   = 16;
                fb_info.green_shift = 8;
                fb_info.blue_shift  = 0;
            } else if (fb_info.bpp == 24) {
                fb_info.red_shift   = 16;
                fb_info.green_shift = 8;
                fb_info.blue_shift  = 0;
            } else {
                fb_info.red_shift = 16;
                fb_info.green_shift = 8;
                fb_info.blue_shift = 0;
            }
            fb_ready = 1;
            return;
        }
        offset += (size + 7) & ~7;
    }
}

void fb_init(void* multiboot_info)
{
    if (!multiboot_info)
        return;
    /* multiboot_info to wskaźnik do struktury boot info; pierwsze 4 B to total_size */
    parse_multiboot2((uint32_t)multiboot_info);
}

uint32_t fb_make_color(uint8_t r, uint8_t g, uint8_t b)
{
    return (r << fb_info.red_shift) | (g << fb_info.green_shift) | (b << fb_info.blue_shift);
}

void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (!fb_ready || !fb_info.addr)
        return;
    if (x >= fb_info.width || y >= fb_info.height)
        return;
    uint32_t* pixel = (uint32_t*)((uint8_t*)fb_info.addr + y * fb_info.pitch + x * (fb_info.bpp / 8));
    *pixel = color;
}

void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    if (!fb_ready || !fb_info.addr)
        return;
    uint32_t x2 = x + w;
    uint32_t y2 = y + h;
    if (x2 > fb_info.width)  x2 = fb_info.width;
    if (y2 > fb_info.height) y2 = fb_info.height;
    for (uint32_t py = y; py < y2; py++) {
        uint32_t* row = (uint32_t*)((uint8_t*)fb_info.addr + py * fb_info.pitch + x * (fb_info.bpp / 8));
        for (uint32_t px = x; px < x2; px++)
            *row++ = color;
    }
}

fb_info_t* fb_get_info(void)
{
    return fb_ready ? &fb_info : NULL;
}
