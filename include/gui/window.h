/**
 * window.h - Okna GUI
 * NexusOS
 */

#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "kernel/types.h"

#define MAX_WINDOWS 16
#define WINDOW_TITLE_LEN 64

typedef void (*window_draw_cb)(void* win, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
typedef void (*window_click_cb)(void* win, uint32_t wx, uint32_t wy);

typedef struct window {
    uint32_t x, y, w, h;
    char title[WINDOW_TITLE_LEN];
    int visible;
    int focused;
    window_draw_cb draw_content;
    window_click_cb on_click;
    void* userdata;
    struct window* next;
} window_t;

void window_init(void);
window_t* window_create(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const char* title);
void window_draw(window_t* win);
void window_draw_all(void);
void window_close(window_t* win);
window_t* window_at(uint32_t sx, uint32_t sy);
void window_move(window_t* win, uint32_t dx, uint32_t dy);
void window_bring_top(window_t* win);
void window_set_content_draw(window_t* win, window_draw_cb cb);
void window_set_click(window_t* win, window_click_cb cb);
void window_set_userdata(window_t* win, void* data);
int window_is_close_button(window_t* win, uint32_t wx, uint32_t wy);
int window_is_titlebar(window_t* win, uint32_t wx, uint32_t wy);

#endif
