//
// Created by cpasjuste on 16/06/19.
//

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "rs_internal.h"

static int server_listen_port = 3333;
static int server_sock_msg = -1;
static int server_sock_cmd = -1;
static rs_thread_t *wait_client_thread = NULL;
static rs_list *client_list = NULL;
static bool quit = false;

void rs_print_color_cli(rs_client_t *client, int color, const char *fmt, ...) {

    if (client == NULL || client->msg_sock < 0) {
        return;
    }

    client->msg.color = color;
    memset(client->msg.buffer, 0, RS_KMSG_SIZE);
    va_list args;
    va_start(args, fmt);
    vsnprintf(client->msg.buffer, RS_KMSG_SIZE, fmt, args);
    va_end(args);
    rs_msg_send_msg(client->msg_sock, &client->msg);
}

void rs_print_color(int color, const char *fmt, ...) {

    rs_node *node = NULL;
    for (node = client_list->head; node != NULL; node = node->next) {
        rs_client_t *client = (rs_client_t *) node->data;
        if (client->msg_sock < 0) {
            continue;
        }
        client->msg.color = color;
        memset(client->msg.buffer, 0, RS_KMSG_SIZE);
        va_list args;
        va_start(args, fmt);
        vsnprintf(client->msg.buffer, RS_KMSG_SIZE, fmt, args);
        va_end(args);
        rs_msg_send_msg(client->msg_sock, &client->msg);
    }
}

static void welcome(rs_client_t *client) {

    RS_PRINT_CLI(client, "\n\n                     ________         .__           .__  .__   \n");
    RS_PRINT_CLI(client, "______  ____________ \\_____  \\   _____|  |__   ____ |  | |  |  \n");
    RS_PRINT_CLI(client, "\\____ \\/  ___/\\____ \\ /  ____/  /  ___/  |  \\_/ __ \\|  | |  |  \n");
    RS_PRINT_CLI(client, "|  |_> >___ \\ |  |_> >       \\  \\___ \\|   Y  \\  ___/|  |_|  |__\n");
    RS_PRINT_CLI(client, "|   __/____  >|   __/\\_______ \\/____  >___|  /\\___  >____/____/\n");
    RS_PRINT_CLI(client, "|__|       \\/ |__|           \\/     \\/     \\/     \\/ %s\n\n", __DATE__);
    RS_PRINT_CLI(client, "\r\n");
}

static void close_sockets() {

    if (server_sock_msg >= 0) {
        rs_net_close(server_sock_msg);
        server_sock_msg = -1;
    }
    if (server_sock_cmd >= 0) {
        rs_net_close(server_sock_cmd);
        server_sock_cmd = -1;
    }
}

static int open_sockets() {

    close_sockets();

    server_sock_msg = rs_net_bind(server_sock_msg, server_listen_port);
    if (server_sock_msg < 0) {
        return -1;
    }
    server_sock_cmd = rs_net_bind(server_sock_cmd, server_listen_port + 1);
    if (server_sock_cmd < 0) {
        return -1;
    }

    return 0;
}

static void client_thread_func(void *args) {

    printf("client_thread: started\n");

    rs_client_t *client = (rs_client_t *) args;
    welcome(client);

    // TODO: loop

    rs_net_close(client->msg_sock);
    rs_net_close(client->cmd_sock);
    printf("list count: %i\n", rs_list_get_count(client_list));
    rs_list_remove(client_list, client);
    printf("list count: %i\n", rs_list_get_count(client_list));

    printf("client_thread: ended\n");
}

static void wait_client_thread_func(void *args) {

    printf("wait_client_thread: started\n");

    // init client list
    client_list = rs_list_create();

    while (!quit) {

        // waiting for client connection
        printf("wait_client_thread: waiting for connections...\n");
        int client_sock = rs_net_get_sock(server_sock_msg);
        if (client_sock < 0) {
            if (quit) {
                break;
            }
            printf("wait_client_thread: network disconnection, restarting...\n");
            rs_sleep(1000);
            open_sockets();
            continue;
        }

        if (quit) {
            break;
        }

        // init client
        printf("wait_client_thread: new client (sock = %i)\n", client_sock);
        rs_client_t *client = malloc(sizeof(rs_client_t));
        if (client == NULL) {
            printf("wait_client_thread: could not allocate client\n");
            continue;
        }
        rs_list_add_tail(client_list, client);
        memset(client, 0, sizeof(rs_client_t));
        strncpy(client->path, RS_HOME_PATH, RS_MAX_PATH - 1);
        client->msg_sock = client_sock;
        // create client thread
        rs_thread_t *thread = rs_thread_create(client_thread_func, client);
        if (thread == NULL) {
            printf("wait_client_thread: client_thread creation failed\n");
            rs_net_close(client->msg_sock);
            free(client);
            continue;
        }
        client->thread = thread;
    }

    // close clients
    if (client_list != NULL) {
        rs_node *node = NULL;
        for (node = client_list->head; node != NULL; node = node->next) {
            printf("rs_shell_exit: closing client...\n");
            rs_net_close(((rs_client_t *) node->data)->msg_sock);
            rs_net_close(((rs_client_t *) node->data)->cmd_sock);
            rs_thread_close(((rs_client_t *) node->data)->thread);
        }
        rs_list_delete(client_list);
        client_list = NULL;
    }

    // close listening sockets
    close_sockets();

    printf("wait_client_thread: ended\n");
}

int rs_shell_init() {

    printf("rs_shell_init\n");

    // init networking, if needed by platform
    int res = rs_net_init();
    if (res != 0) {
        printf("rs_shell_init: rs_net_init failed\n");
        return -1;
    }

    // setup sockets
    res = open_sockets();
    if (res != 0) {
        printf("rs_shell_init: open_sockets failed\n");
        return -1;
    }

    // start main thread
    wait_client_thread = rs_thread_create(wait_client_thread_func, NULL);
    if (wait_client_thread == NULL) {
        printf("rs_shell_init: wait_client_thread creation failed\n");
        close_sockets();
        return -1;
    }

    printf("rs_shell_init: done\n");

    return 0;
}

void rs_shell_exit() {

    printf("rs_shell_exit: start\n");
    quit = true;
    close_sockets();
    if (wait_client_thread) {
        rs_thread_join(wait_client_thread);
        rs_thread_close(wait_client_thread);
    }

    printf("rs_shell_exit: end\n");
}
