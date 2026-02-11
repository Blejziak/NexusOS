/**
 * settings.c - Okno Ustawienia systemowe
 * NexusOS - informacje o systemie, motyw, RAM, wyłączenie
 */

#include "apps/settings.h"
#include "gui/gui.h"
#include "gui/window.h"
#include "drivers/framebuffer.h"
#include "kernel/types.h"
#include "gui/gui.h"

#define SETTINGS_W 400
#define SETTINGS_H 320
#define LINE_H     24
#define MARGIN     16

static void settings_draw_content(void* win, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    (void)win;
    (void)w;
    (void)h;
    fb_info_t* fb = fb_get_info();
    if (!fb) return;
    theme_t th;
    theme_get_current(&th);
    /* Tło już narysowane przez window_draw */
    /* Informacje o systemie - bez fontu rysujemy pasek */
    fb_fill_rect(x + MARGIN, y + MARGIN, 200, LINE_H, th.accent);
    fb_fill_rect(x + MARGIN, y + MARGIN + LINE_H + 8, 200, LINE_H, 0xDDDDDD);
    /* Zmiana motywu - 4 przyciski */
    uint32_t by = y + MARGIN + (LINE_H + 8) * 2 + 16;
    int saved_theme = theme_get_index();
    theme_t t;
    for (int i = 0; i < THEME_COUNT; i++) {
        uint32_t bx = x + MARGIN + i * (60 + 8);
        theme_set(i);
        theme_get_current(&t);
        fb_fill_rect(bx, by, 60, 28, t.accent);
    }
    theme_set(saved_theme);
    /* RAM - placeholder */
    fb_fill_rect(x + MARGIN, by + 40, 150, LINE_H, 0xCCCCCC);
    /* Przyciski Wyłącz / Restart */
    fb_fill_rect(x + MARGIN, h - MARGIN - 32, 100, 28, 0x993333);
    fb_fill_rect(x + MARGIN + 110, h - MARGIN - 32, 100, 28, 0x339933);
}

static int themes_loaded = 0;

static void load_themes(void)
{
    (void)themes_loaded;
    themes_loaded = 1;
}

static void settings_click(void* win, uint32_t wx, uint32_t wy)
{
    window_t* w = (window_t*)win;
    uint32_t by = MARGIN + (LINE_H + 8) * 2 + 16;
    uint32_t bx0 = MARGIN;
    for (int i = 0; i < THEME_COUNT; i++) {
        uint32_t bx = bx0 + i * (60 + 8);
        if (wx >= bx && wx < bx + 60 && wy >= by && wy < by + 28) {
            theme_set(i);
            return;
        }
    }
    /* Shutdown / Restart - przyciski na dole */
    uint32_t py = w->h - TITLE_BAR_H - MARGIN - 32;
    if (wy >= py && wy < py + 28) {
        if (wx >= MARGIN && wx < MARGIN + 100)
            ; /* shutdown - TODO: wywołanie APM/ACPI */
        else if (wx >= MARGIN + 110 && wx < MARGIN + 210)
            ; /* reboot - TODO */
    }
}

window_t* settings_open(void)
{
    load_themes();
    fb_info_t* fb = fb_get_info();
    if (!fb) return NULL;
    uint32_t x = (fb->width - SETTINGS_W) / 2;
    uint32_t y = (fb->height - SETTINGS_H) / 2;
    if (y < TASKBAR_HEIGHT) y = TASKBAR_HEIGHT;
    window_t* win = window_create(x, y, SETTINGS_W, SETTINGS_H, "Settings");
    if (!win) return NULL;
    window_set_content_draw(win, settings_draw_content);
    window_set_click(win, settings_click);
    return win;
}
