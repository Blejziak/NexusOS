/**
 * gui.h - Wspólne definicje GUI
 * NexusOS
 */

#ifndef GUI_H
#define GUI_H

#include "kernel/types.h"

#define TASKBAR_HEIGHT  40
#define DOCK_ICON_SIZE  36
#define DOCK_ICON_GAP   8
#define TITLE_BAR_H     24
#define WIN_BUTTON_W    20
#define THEME_COUNT     4

/* Kolory motywu: tło paska, dock, akcent */
typedef struct {
    uint32_t taskbar_bg;
    uint32_t dock_bg;
    uint32_t accent;
    uint32_t titlebar_bg;
    uint32_t titlebar_text;
    uint32_t window_bg;
} theme_t;

void theme_get_current(theme_t* t);
void theme_set(int index);
int  theme_get_index(void);

#endif
