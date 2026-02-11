/**
 * shell.c - Powłoka tekstowa (aplikacja w docku)
 * NexusOS - komendy: help, ls, cd, clear, echo, reboot, shutdown
 */

#include "kernel/types.h"
#include "gui/window.h"
#include "drivers/framebuffer.h"
#include "gui/gui.h"

#define SHELL_W 640
#define SHELL_H 400
#define LINE_H  16
#define MARGIN  8
#define MAX_LINES 20
#define CMD_LEN  80

static char cmd_buf[CMD_LEN];
static int cmd_len = 0;
static char* lines[MAX_LINES];
static int num_lines = 0;
static char line_buf[MAX_LINES][CMD_LEN];

static void shell_draw_content(void* win, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    (void)win;
    fb_info_t* fb = fb_get_info();
    if (!fb) return;
    fb_fill_rect(x, y, w, h, 0x1E1E1E);
    /* Bez fontu - placeholder dla tekstu */
    fb_fill_rect(x + MARGIN, y + h - LINE_H - MARGIN, w - 2*MARGIN, LINE_H, 0x333333);
}

static void shell_click(void* win, uint32_t wx, uint32_t wy)
{
    (void)win;
    (void)wx;
    (void)wy;
}

/* Parsowanie argumentów - prosty split po spacjach */
static int parse_args(char* cmd, char* argv[], int max)
{
    int n = 0;
    while (n < max && *cmd) {
        while (*cmd == ' ') cmd++;
        if (!*cmd) break;
        argv[n++] = cmd;
        while (*cmd && *cmd != ' ') cmd++;
        if (*cmd) *cmd++ = '\0';
    }
    return n;
}

static void exec_cmd(const char* line)
{
    char buf[CMD_LEN];
    for (int i = 0; i < CMD_LEN && line[i]; i++) buf[i] = line[i];
    buf[CMD_LEN-1] = '\0';
    char* argv[16];
    int argc = parse_args(buf, argv, 16);
    if (argc == 0) return;
    /* help */
    if (argv[0][0] == 'h' && argv[0][1] == 'e' && argv[0][2] == 'l' && argv[0][3] == 'p') {
        if (num_lines < MAX_LINES) {
            char* l = line_buf[num_lines++];
            l[0] = 'h'; l[1] = 'e'; l[2] = 'l'; l[3] = 'p'; l[4] = ','; l[5] = ' '; l[6] = 'l'; l[7] = 's'; l[8] = ','; l[9] = ' '; l[10] = 'c'; l[11] = 'd'; l[12] = '\0';
        }
        return;
    }
    /* clear */
    if (argv[0][0] == 'c' && argv[0][1] == 'l' && argv[0][2] == 'e' && argv[0][3] == 'a' && argv[0][4] == 'r') {
        num_lines = 0;
        return;
    }
    /* echo - zapisz linię do wyświetlenia */
    if (argv[0][0] == 'e' && argv[0][1] == 'c' && argv[0][2] == 'h' && argv[0][3] == 'o') {
        if (num_lines < MAX_LINES && argc > 1) {
            int j = 0;
            for (int i = 0; argv[1][i] && j < CMD_LEN-1; i++) line_buf[num_lines][j++] = argv[1][i];
            line_buf[num_lines][j] = '\0';
            lines[num_lines] = line_buf[num_lines];
            num_lines++;
        }
        return;
    }
    /* reboot, shutdown - placeholder */
    (void)argc;
}

window_t* shell_open(void)
{
    fb_info_t* fb = fb_get_info();
    if (!fb) return NULL;
    uint32_t x = (fb->width - SHELL_W) / 2;
    uint32_t y = (fb->height - SHELL_H) / 2;
    if (y < TASKBAR_HEIGHT) y = TASKBAR_HEIGHT;
    window_t* win = window_create(x, y, SHELL_W, SHELL_H, "Shell");
    if (!win) return NULL;
    cmd_len = 0;
    num_lines = 0;
    window_set_content_draw(win, shell_draw_content);
    window_set_click(win, shell_click);
    return win;
}
