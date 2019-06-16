//
// Created by cpasjuste on 16/06/19.
//

#ifndef LIBRSHELL_RS_INTERNAL_H
#define LIBRSHELL_RS_INTERNAL_H

#include "rs_shell.h"
#include "rs_io.h"
#include "rs_cmd.h"
#include "rs_msg.h"
#include "rs_net.h"

typedef struct {
    int msg_sock;
    int cmd_sock;
    RS_CMD cmd;
    RS_MSG msg;
    char path[512];
} RS_CLIENT;

void rs_print_color(int color, const char *fmt, ...);

#define PRINT(...) rs_print_color(COL_YELLOW, __VA_ARGS__)
#define PRINT_COL(color, ...) rs_print_color(color, __VA_ARGS__)
#define PRINT_ERR(fmt, ...) rs_print_color(COL_RED, "\n\nNOK: " fmt "\n", ## __VA_ARGS__)
#define PRINT_ERR_CODE(func_name, code) rs_print_color(COL_RED, "\n\nNOK: %s = 0x%08X\n", func_name, code)
#define PRINT_PROMPT() rs_print_color(COL_NONE, "\r\n");

#endif //LIBRSHELL_RS_INTERNAL_H
