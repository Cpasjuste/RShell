/*
	PSP2SHELL
	Copyright (C) 2016, Cpasjuste

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/socket.h>

#include "rs_cmd.h"

int rs_cmd_receive(int sock, rs_cmd_t *cmd) {

    char buffer[RS_SIZE_CMD];
    memset(buffer, 0, RS_SIZE_CMD);

    ssize_t read = recv(sock, buffer, RS_SIZE_CMD, 0);
    if (read < 2) {
        return read <= 0 ? RS_ERR_SOCKET : RS_ERR_INVALID_CMD;
    }

    bool is_cmd = rs_cmd_to_cmd(cmd, buffer) == 0;
    if (!is_cmd) {
        return RS_ERR_INVALID_CMD;
    }

    return 0;
}

int rs_cmd_wait_result(int sock) {

    char buffer[RS_SIZE_CMD];
    memset(buffer, 0, RS_SIZE_CMD);

    ssize_t read = recv(sock, buffer, RS_SIZE_CMD, 0);
    if (read < 2) {
        return -1;
    }

    rs_cmd_t cmd;
    if (rs_cmd_to_cmd(&cmd, buffer) != 0) {
        return -1;
    }

    if (cmd.type != CMD_OK) {
        return -1;
    }

    return 0;
}

size_t rs_cmd_receive_buffer(int sock, void *buffer, size_t size) {

    ssize_t len;
    size_t left = size;

    while (left) {
        len = recv(sock, buffer, left, 0);
        if (len == 0) {
            size = 0;
            break;
        };
        if (len == -1) {
            break;
        } else {
            left -= len;
            buffer += len;
        }
    }

    return size;
}

void rs_cmd_send(int sock, int cmdType) {

    char buffer[4];
    memset(buffer, 0, 4);
    snprintf(buffer, 4, "%i", cmdType);
    send(sock, buffer, 4, 0);
}

void rs_cmd_send_cmd(int sock, rs_cmd_t *cmd) {

    char buffer[RS_SIZE_CMD];
    memset(buffer, 0, RS_SIZE_CMD);

    if (rs_cmd_to_string(buffer, cmd) == 0) {
        send(sock, buffer, strlen(buffer), 0);
    }
}

void rs_cmd_send_fmt(int sock, const char *fmt, ...) {

    char buffer[RS_SIZE_CMD];
    memset(buffer, 0, RS_SIZE_CMD);
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, RS_SIZE_CMD, fmt, args);
    va_end(args);

    send(sock, buffer, strlen(buffer), 0);
}

void rs_cmd_send_string(int sock, int cmdType, const char *value) {

    char buffer[RS_SIZE_STRING + 6];
    memset(buffer, 0, RS_SIZE_STRING + 6);
    snprintf(buffer, RS_SIZE_STRING + 6, "%i\"%s\"", cmdType, value);
    send(sock, buffer, strlen(buffer), 0);
}

void rs_cmd_send_int(int sock, int cmdType, int value) {

    char buffer[16];
    memset(buffer, 0, 16);
    snprintf(buffer, 16, "%i\"%i\"", cmdType, value);
    send(sock, buffer, strlen(buffer), 0);
}

void rs_cmd_send_long(int sock, int cmdType, long value) {

    char buffer[32];
    memset(buffer, 0, 32);
    snprintf(buffer, 32, "%i\"%ld\"", cmdType, value);
    send(sock, buffer, strlen(buffer), 0);
}

int rs_cmd_to_string(char *buffer, rs_cmd_t *c) {

    if (!buffer || !c) {
        return -1;
    }

    memset(buffer, 0, RS_SIZE_CMD);
    sprintf(buffer, "%i", c->type);
    for (int i = 0; i < RS_MAX_ARGS; i++) {
        snprintf(buffer + strlen(buffer), RS_SIZE_CMD, "\"%s", c->args[i]);
    }
    strncat(buffer, "\"", RS_SIZE_CMD);

    return 0;
}

int rs_cmd_to_cmd(rs_cmd_t *c, const char *buffer) {

    if (!c || !buffer) {
        return -1;
    }

    if (strlen(buffer) < 2) {
        return -1;
    }

    memset(c, 0, sizeof(rs_cmd_t));

    // type
    char tmp[2];
    strncpy(tmp, buffer, 2);
    c->type = atoi(tmp);
    if (c->type < CMD_START) {
        c->type = 0;
        return -1;
    }

    const char *start = NULL, *end = buffer;

    for (int i = 0; i < RS_MAX_ARGS; i++) {

        start = strstr(end, "\"");
        if (!start) {
            break;
        }
        start += 1;

        end = strstr(start, "\"");
        if (!end) {
            break;
        }

        strncpy(c->args[i], start, end - start);
    }

    return 0;
}
