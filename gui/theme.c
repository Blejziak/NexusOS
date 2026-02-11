/**
 * theme.c - System motywów GUI
 * NexusOS
 */

#include "gui/gui.h"
#include "drivers/framebuffer.h"

static int current_theme = 0;

static const theme_t themes[THEME_COUNT] = {
    /* Niebieski */
    { 0x003366, 0x004080, 0x0066AA, 0x004080, 0xFFFFFF, 0xE8EEF2 },
    /* Zielony */
    { 0x1B4D3E, 0x2E7D5C, 0x3D9970, 0x2E7D5C, 0xFFFFFF, 0xE8F5E9 },
    /* Fioletowy */
    { 0x3D2B56, 0x5C3D7D, 0x7B5C9E, 0x5C3D7D, 0xFFFFFF, 0xF3E5F5 },
    /* Ciemny */
    { 0x1A1A2E, 0x16213E, 0x0F3460, 0x16213E, 0xE0E0E0, 0x2D2D44 }
};

void theme_get_current(theme_t* t)
{
    *t = themes[current_theme];
}

void theme_set(int index)
{
    if (index >= 0 && index < THEME_COUNT)
        current_theme = index;
}

int theme_get_index(void)
{
    return current_theme;
}
