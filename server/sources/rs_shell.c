//
// Created by cpasjuste on 16/06/19.
//

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "rs_internal.h"

static RS_CLIENT client;
static int server_listen_port = 3333;
static int server_sock_msg;
static int server_sock_cmd;
static int quit = 0;

void rs_print_color(int color, const char *fmt, ...) {

    if (client.msg_sock < 0) {
        return;
    }

    client.msg.color = color;
    memset(client.msg.buffer, 0, RS_KMSG_SIZE);
    va_list args;
    va_start(args, fmt);
    vsnprintf(client.msg.buffer, RS_KMSG_SIZE, fmt, args);
    va_end(args);

    rs_msg_send_msg(client.msg_sock, &client.msg);
}

static void welcome() {

    PRINT("\n\n                     ________         .__           .__  .__   \n");
    PRINT("______  ____________ \\_____  \\   _____|  |__   ____ |  | |  |  \n");
    PRINT("\\____ \\/  ___/\\____ \\ /  ____/  /  ___/  |  \\_/ __ \\|  | |  |  \n");
    PRINT("|  |_> >___ \\ |  |_> >       \\  \\___ \\|   Y  \\  ___/|  |_|  |__\n");
    PRINT("|   __/____  >|   __/\\_______ \\/____  >___|  /\\___  >____/____/\n");
    PRINT("|__|       \\/ |__|           \\/     \\/     \\/     \\/ %s\n\n", __DATE__);
    PRINT("\r\n");
}

static void close_server() {

    if (server_sock_msg >= 0) {
        rs_net_close(server_sock_msg);
        server_sock_msg = -1;
    }
    if (server_sock_cmd >= 0) {
        rs_net_close(server_sock_cmd);
        server_sock_cmd = -1;
    }
}

static int open_server() {

    close_server();

    server_sock_msg = rs_net_bind(server_sock_msg, server_listen_port);
    if (server_sock_msg < 0) {
        return -1;
    }
    server_sock_cmd = rs_net_bind(server_sock_cmd, server_listen_port + 1);
    if (server_sock_cmd < 0) {
        return -1;
    }

    printf("server_sock_msg: %i, server_sock_cmd: %i\n", server_sock_msg, server_sock_cmd);

    return 0;
}

static int rs_shell_update_client() {

    RS_CMD cmd;


}

int rs_shell_init() {

    printf("rs_shell_init\n");

    // TODO: create client wait thread

    int res = rs_net_init();
    if (res != 0) {
        printf("rs_shell_init: rs_net_init failed\n");
        return -1;
    }

    // setup sockets
    res = open_server();
    if (res != 0) {
        printf("rs_shell_init: open_server failed\n");
        return -1;
    }

    // accept incoming connections
    printf("rs_shell_init: waiting for connections...\n");
    int client_sock = rs_net_get_sock(server_sock_msg);
    printf("rs_shell_init: new client (sock = %i)\n", client_sock);

    // init client
    memset(&client, 0, sizeof(client));
    strcpy(client.path, "/");
    client.msg_sock = client_sock;
    welcome();

    rs_net_close(client_sock);
    rs_net_close(server_sock_msg);
    rs_net_close(server_sock_cmd);

    printf("rs_shell_init: done\n");

    return 0;
}
