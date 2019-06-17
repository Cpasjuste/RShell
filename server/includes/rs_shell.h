//
// Created by cpasjuste on 16/06/19.
//

#ifndef LIBRSHELL_RS_SHELL_H
#define LIBRSHELL_RS_SHELL_H

#define RS_HOME_PATH "/"

int rs_shell_init();

void rs_shell_exit();

void rs_print_color(int color, const char *fmt, ...);

enum rs_colors_t {
    COL_NONE = 10,
    COL_RED,
    COL_YELLOW,
    COL_GREEN,
    COL_HEX
};

#define RS_PRINT(...) rs_print_color(COL_YELLOW, __VA_ARGS__)
#define RS_PRINT_COL(color, ...) RS_PRINT(color, __VA_ARGS__)
#define RS_PRINT_ERR(fmt, ...) RS_PRINT(COL_RED, "\n\nNOK: " fmt "\n", ## __VA_ARGS__)
#define RS_PRINT_ERR_CODE(func_name, code) RS_PRINT(COL_RED, "\n\nNOK: %s = 0x%08X\n", func_name, code)
#define RS_PRINT_PROMPT() RS_PRINT(COL_NONE, "\r\n");

#endif //LIBRSHELL_RS_SHELL_H
