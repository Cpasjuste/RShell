//
// Created by cpasjuste on 16/06/19.
//

#ifndef LIBRSHELL_RS_INTERNAL_H
#define LIBRSHELL_RS_INTERNAL_H

#include "rs_shell.h"
#include "rs_sys.h"
#include "rs_io.h"
#include "rs_cmd.h"
#include "rs_msg.h"
#include "rs_net.h"
#include "rs_thread.h"
#include "rs_list.h"

typedef struct {
    int msg_sock;
    int cmd_sock;
    rs_cmd_t cmd;
    rs_msg_t msg;
    rs_thread_t *thread;
    char path[RS_MAX_PATH];
} rs_client_t;

void rs_print_color_cli(rs_client_t *client, int color, const char *fmt, ...);

#define RS_PRINT_CLI(client, ...) rs_print_color_cli(client, COL_YELLOW, __VA_ARGS__)
#define RS_PRINT_CLI_COL(client, color, ...) RS_PRINT_CLI(client, color, __VA_ARGS__)
#define RS_PRINT_CLI_ERR(client, fmt, ...) RS_PRINT_CLI(client, COL_RED, "\n\nNOK: " fmt "\n", ## __VA_ARGS__)
#define RS_PRINT_CLI_ERR_CODE(client, func_name, code) RS_PRINT_CLI(client, COL_RED, "\n\nNOK: %s = 0x%08X\n", func_name, code)
#define RS_PRINT_CLI_PROMPT(client) RS_PRINT_CLI(client, COL_NONE, "\r\n");

#endif //LIBRSHELL_RS_INTERNAL_H
