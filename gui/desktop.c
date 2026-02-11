/**
 * desktop.c - Pulpit z paskiem zadań i dockiem
 * NexusOS - taskbar, dock, ikona ustawień, zegar
 */

#include "gui/desktop.h"
#include "gui/gui.h"
#include "gui/window.h"
#include "drivers/framebuffer.h"
#include "drivers/mouse.h"
#include "drivers/timer.h"
#include "kernel/types.h"
#include "apps/settings.h"
#include "apps/shell.h"
#include <stddef.h>

#define SETTINGS_ICON_LEFT  8
#define SETTINGS_ICON_TOP   (TASKBAR_HEIGHT - DOCK_ICON_SIZE) / 2
#define CLOCK_RIGHT        16
#define CLOCK_WIDTH        80
#define CLOCK_TOP          8
#define DOCK_MAX_ICONS     8
#define DESKTOP_BG         0x1a1a2e

static uint32_t screen_w = 0, screen_h = 0;
static int settings_window_open = 0;
static window_t* settings_win = NULL;
static int dock_hover = -1;
static uint32_t last_draw_ticks = 0;
static int boot_logo_shown = 0;

/* Współrzędne ikon docka (wyśrodkowane) */
static uint32_t dock_icon_x[DOCK_MAX_ICONS];
static int dock_icon_count = 2; /* Settings + Shell na starcie */

static void draw_background(void)
{
    fb_info_t* fb = fb_get_info();
    if (!fb) return;
    fb_fill_rect(0, 0, screen_w, screen_h, DESKTOP_BG);
}

static void draw_taskbar(void)
{
    theme_t th;
    theme_get_current(&th);
    fb_fill_rect(0, 0, screen_w, TASKBAR_HEIGHT, th.taskbar_bg);
}

/* Ikona ustawień - zębatka: prosty kwadrat z wcięciami */
static void draw_settings_icon(int hover)
{
    theme_t th;
    theme_get_current(&th);
    uint32_t c = hover ? th.accent : 0xAAAAAA;
    uint32_t x = SETTINGS_ICON_LEFT;
    uint32_t y = SETTINGS_ICON_TOP;
    uint32_t s = DOCK_ICON_SIZE;
    fb_fill_rect(x, y, s, s, c);
    /* Wewnętrzny kwadrat (symulacja zębatki) */
    fb_fill_rect(x + 8, y + 8, s - 16, s - 16, th.taskbar_bg);
}

static void draw_clock(void)
{
    theme_t th;
    theme_get_current(&th);
    uint32_t x = screen_w - CLOCK_RIGHT - CLOCK_WIDTH;
    uint32_t y = CLOCK_TOP;
    fb_fill_rect(x, y, CLOCK_WIDTH, TASKBAR_HEIGHT - 16, th.dock_bg);
    /* Zegar - na razie placeholder (bez fontu: prostokąt) */
    (void)th;
}

static void draw_dock_icons(void)
{
    theme_t th;
    theme_get_current(&th);
    for (int i = 0; i < dock_icon_count; i++) {
        uint32_t x = dock_icon_x[i];
        uint32_t y = (TASKBAR_HEIGHT - DOCK_ICON_SIZE) / 2;
        uint32_t c = (i == dock_hover) ? th.accent : th.dock_bg;
        fb_fill_rect(x, y, DOCK_ICON_SIZE, DOCK_ICON_SIZE, c);
    }
}

static void layout_dock(void)
{
    uint32_t total = dock_icon_count * DOCK_ICON_SIZE + (dock_icon_count - 1) * DOCK_ICON_GAP;
    uint32_t start_x = (screen_w - total) / 2;
    for (int i = 0; i < dock_icon_count; i++)
        dock_icon_x[i] = start_x + i * (DOCK_ICON_SIZE + DOCK_ICON_GAP);
}

static int hit_settings_icon(uint32_t mx, uint32_t my)
{
    return (mx >= SETTINGS_ICON_LEFT && mx < SETTINGS_ICON_LEFT + DOCK_ICON_SIZE &&
            my >= SETTINGS_ICON_TOP && my < SETTINGS_ICON_TOP + DOCK_ICON_SIZE);
}

static int hit_clock(uint32_t mx, uint32_t my)
{
    uint32_t x = screen_w - CLOCK_RIGHT - CLOCK_WIDTH;
    return (mx >= x && mx < x + CLOCK_WIDTH && my >= CLOCK_TOP && my < TASKBAR_HEIGHT - 8);
}

static int hit_dock_icon(uint32_t mx, uint32_t my, int* which)
{
    uint32_t y = (TASKBAR_HEIGHT - DOCK_ICON_SIZE) / 2;
    if (my < y || my >= y + DOCK_ICON_SIZE) return 0;
    for (int i = 0; i < dock_icon_count; i++) {
        if (mx >= dock_icon_x[i] && mx < dock_icon_x[i] + DOCK_ICON_SIZE) {
            *which = i;
            return 1;
        }
    }
    return 0;
}

void gui_desktop_init(void)
{
    fb_info_t* fb = fb_get_info();
    if (!fb) return;
    screen_w = fb->width;
    screen_h = fb->height;
    window_init();
    layout_dock();
}

static void draw_boot_logo(void)
{
    theme_t th;
    theme_get_current(&th);
    fb_fill_rect(0, 0, screen_w, screen_h, DESKTOP_BG);
    uint32_t lw = 280;
    uint32_t lh = 80;
    uint32_t lx = (screen_w - lw) / 2;
    uint32_t ly = (screen_h - lh) / 2;
    fb_fill_rect(lx, ly, lw, lh, th.accent);
    fb_fill_rect(lx + 4, ly + 4, lw - 8, lh - 8, th.taskbar_bg);
}

void gui_desktop_redraw(void)
{
    if (!boot_logo_shown && timer_get_ticks() < 150) {
        draw_boot_logo();
        return;
    }
    if (!boot_logo_shown)
        boot_logo_shown = 1;
    draw_background();
    draw_taskbar();
    {
        int16_t mx, my;
        uint8_t bt;
        mouse_get(&mx, &my, &bt);
        draw_settings_icon(hit_settings_icon((uint32_t)mx, (uint32_t)my) ? 1 : 0);
    }
    draw_clock();
    draw_dock_icons();
    window_draw_all();
}

static void handle_mouse(void)
{
    int16_t mx, my;
    uint8_t bt;
    mouse_get(&mx, &my, &bt);
    if (mx < 0) mx = 0;
    if (my < 0) my = 0;
    if ((uint32_t)mx >= screen_w) mx = screen_w - 1;
    if ((uint32_t)my >= screen_h) my = screen_h - 1;
    mouse_set_position(mx, my);

    /* Hover na dock */
    int di = -1;
    if (my < TASKBAR_HEIGHT && hit_dock_icon((uint32_t)mx, (uint32_t)my, &di))
        dock_hover = di;
    else
        dock_hover = -1;

    /* Klik */
    if (!(bt & 1)) return;

    /* Klik w okno? */
    window_t* w = window_at((uint32_t)mx, (uint32_t)my);
    if (w) {
        window_bring_top(w);
        if (window_is_close_button(w, (uint32_t)mx, (uint32_t)my)) {
            if (w == settings_win) settings_window_open = 0;
            window_close(w);
        }
        else if (window_is_titlebar(w, (uint32_t)mx, (uint32_t)my))
            ; /* TODO: rozpocznij przeciąganie */
        else if (w->on_click)
            w->on_click(w, (uint32_t)mx - w->x, (uint32_t)my - w->y);
        return;
    }

    /* Klik w ikonę ustawień */
    if (my < TASKBAR_HEIGHT && hit_settings_icon((uint32_t)mx, (uint32_t)my)) {
        if (!settings_window_open) {
            settings_win = settings_open();
            settings_window_open = 1;
        }
        return;
    }

    /* Klik w dock - ikona 0 = ustawienia (już obsłużone po lewej), ikona 1 = shell */
    if (di >= 0 && my < TASKBAR_HEIGHT) {
        if (di == 1)
            shell_open();
    }
}

void gui_desktop_run(void)
{
    for (;;) {
        if (mouse_has_update())
            mouse_clear_update();
        handle_mouse();
        uint32_t t = timer_get_ticks();
        if (t - last_draw_ticks > 5) {
            last_draw_ticks = t;
            gui_desktop_redraw();
        }
    }
}
