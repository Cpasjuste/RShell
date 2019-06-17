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

#include "rs_internal.h"

int rs_msg_receive(int sock, rs_msg_t *msg) {

    char buffer[RS_SIZE_MSG];
    memset(buffer, 0, RS_SIZE_MSG);

    ssize_t read = recv(sock, buffer, RS_SIZE_MSG, 0);
    if (read < 3) {
        memset(msg->buffer, 0, RS_KMSG_SIZE);
        strncpy(msg->buffer, buffer, RS_KMSG_SIZE);
        return read <= 0 ? RS_ERR_SOCKET : RS_ERR_INVALID_MSG;
    }

    bool is_msg = rs_msg_to_msg(msg, buffer) == 0;
    if (!is_msg) {
        memset(msg->buffer, 0, RS_KMSG_SIZE);
        strncpy(msg->buffer, buffer, RS_KMSG_SIZE);
        return RS_ERR_INVALID_MSG;
    }

    return 0;
}

void rs_msg_send(int sock, int color, const char *msg) {

    size_t len = strlen(msg) + 2;
    char buffer[len];
    memset(buffer, 0, len);
    snprintf(buffer, len, "%i%s", color, msg);
    send(sock, buffer, len, 0);
}

int rs_msg_send_msg(int sock, rs_msg_t *msg) {

    char buffer[RS_SIZE_MSG];

    if (rs_msg_to_string(buffer, msg) == 0) {
        send(sock, buffer, strlen(buffer), 0);
        return 0;
    }

    return -1;
}

int rs_msg_to_string(char *buffer, rs_msg_t *cmd) {

    if (!buffer || !cmd) {
        return -1;
    }

    memset(buffer, 0, RS_SIZE_MSG);
    sprintf(buffer, "%i", cmd->color);
    snprintf(buffer + 2, RS_SIZE_MSG, "%s", cmd->buffer);

    return 0;
}

int rs_msg_to_msg_advanced(rs_msg_t *msg, const char *buffer, size_t len) {

    if (!msg || !buffer) {
        return -1;
    }

    if (len < 0) {
        return -1;
    }

    memset(msg, 0, sizeof(rs_msg_t));

    // type
    char tmp[2];
    strncpy(tmp, buffer, 2);
    msg->color = atoi(tmp);
    if (msg->color < COL_NONE) {
        msg->color = 0;
        return -1;
    }

    if (len > 0) {
        strncpy(msg->buffer, buffer + 2, len);
    }

    return 0;
}

int rs_msg_to_msg(rs_msg_t *msg, const char *buffer) {

    return rs_msg_to_msg_advanced(msg, buffer, strlen(buffer) - 2);
}
