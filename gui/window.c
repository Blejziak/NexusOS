/**
 * window.c - Menedżer okien i rysowanie
 * NexusOS
 */

#include "gui/window.h"
#include "gui/gui.h"
#include "drivers/framebuffer.h"
#include "kernel/types.h"
#include <stddef.h>

static window_t* windows[MAX_WINDOWS];
static int num_windows = 0;
static window_t* window_list_head = NULL;
static window_t* focused_window = NULL;

static void draw_titlebar(window_t* win)
{
    theme_t th;
    theme_get_current(&th);
    fb_info_t* fb = fb_get_info();
    if (!fb) return;
    fb_fill_rect(win->x, win->y, win->w, TITLE_BAR_H, th.titlebar_bg);
    /* Przycisk zamknięcia (X) - prosty kwadrat */
    uint32_t bx = win->x + win->w - WIN_BUTTON_W - 4;
    uint32_t by = win->y + 2;
    fb_fill_rect(bx, by, WIN_BUTTON_W, TITLE_BAR_H - 4, 0xCC3333);
    /* Tytuł - bez fontu, pomijamy tekst (można dodać prosty bitmap font) */
    (void)win->title;
}

void window_init(void)
{
    num_windows = 0;
    window_list_head = NULL;
    focused_window = NULL;
    for (int i = 0; i < MAX_WINDOWS; i++)
        windows[i] = NULL;
}

window_t* window_create(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const char* title)
{
    if (num_windows >= MAX_WINDOWS) return NULL;
    static window_t pool[MAX_WINDOWS];
    int idx = -1;
    for (int i = 0; i < MAX_WINDOWS; i++)
        if (!windows[i]) { idx = i; break; }
    if (idx < 0) return NULL;
    window_t* win = &pool[idx];
    windows[idx] = win;
    num_windows++;
    win->x = x;
    win->y = y;
    win->w = w;
    win->h = h;
    win->visible = 1;
    win->focused = 0;
    win->draw_content = NULL;
    win->on_click = NULL;
    win->userdata = NULL;
    win->next = window_list_head;
    window_list_head = win;
    for (int i = 0; i < WINDOW_TITLE_LEN - 1 && title && title[i]; i++)
        win->title[i] = title[i];
    win->title[WINDOW_TITLE_LEN - 1] = '\0';
    focused_window = win;
    win->focused = 1;
    return win;
}

void window_draw(window_t* win)
{
    if (!win || !win->visible) return;
    theme_t th;
    theme_get_current(&th);
    draw_titlebar(win);
    fb_fill_rect(win->x, win->y + TITLE_BAR_H, win->w, win->h - TITLE_BAR_H, th.window_bg);
    if (win->draw_content)
        win->draw_content(win, win->x, win->y + TITLE_BAR_H, win->w, win->h - TITLE_BAR_H);
}

void window_draw_all(void)
{
    window_t* w = window_list_head;
    while (w) {
        window_draw(w);
        w = w->next;
    }
}

void window_close(window_t* win)
{
    if (!win) return;
    win->visible = 0;
    for (int i = 0; i < MAX_WINDOWS; i++)
        if (windows[i] == win) {
            windows[i] = NULL;
            num_windows--;
            break;
        }
    if (focused_window == win)
        focused_window = window_list_head;
}

/* Zwraca okno „na wierzchu” (pierwsze na liście) zawierające punkt */
window_t* window_at(uint32_t sx, uint32_t sy)
{
    window_t* w = window_list_head;
    while (w) {
        if (w->visible && sx >= w->x && sx < w->x + w->w && sy >= w->y && sy < w->y + w->h)
            return w;
        w = w->next;
    }
    return NULL;
}

int window_is_close_button(window_t* win, uint32_t wx, uint32_t wy)
{
    uint32_t bx = win->x + win->w - WIN_BUTTON_W - 4;
    uint32_t by = win->y + 2;
    return (wx >= bx && wx < bx + WIN_BUTTON_W && wy >= by && wy < by + TITLE_BAR_H - 4);
}

int window_is_titlebar(window_t* win, uint32_t wx, uint32_t wy)
{
    return (wy >= win->y && wy < win->y + TITLE_BAR_H);
}

void window_move(window_t* win, uint32_t dx, uint32_t dy)
{
    win->x += dx;
    win->y += dy;
}

void window_bring_top(window_t* win)
{
    if (window_list_head == win) return;
    window_t* prev = NULL;
    window_t* w = window_list_head;
    while (w && w != win) {
        prev = w;
        w = w->next;
    }
    if (!w) return;
    if (prev)
        prev->next = win->next;
    else
        window_list_head = win->next;
    win->next = window_list_head;
    window_list_head = win;
    focused_window = win;
}

void window_set_content_draw(window_t* win, window_draw_cb cb)
{
    if (win) win->draw_content = cb;
}

void window_set_click(window_t* win, window_click_cb cb)
{
    if (win) win->on_click = cb;
}

void window_set_userdata(window_t* win, void* data)
{
    if (win) win->userdata = data;
}
