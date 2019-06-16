//
// Created by cpasjuste on 16/06/19.
//


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>

#include "rs_internal.h"

int rs_net_init() {
    return 0;
}

int rs_net_bind(int sock, int port) {

    struct sockaddr_in serverAddress;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    // prepare the sockaddr structure
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons((unsigned short) port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind
    if (bind(sock, (const struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printf("rs_net_bind: bind failed\n");
        return -1;
    }

    // listen
    if (listen(sock, 64) < 0) {
        printf("rs_net_bind: listen failed\n");
        return -1;
    }

    return sock;
}

int rs_net_close(int sock) {
    return close(sock);
}

int rs_net_get_sock(int sock) {

    struct sockaddr_in clientAddress;
    unsigned int c = sizeof(clientAddress);
    return accept(sock, (struct sockaddr *) &clientAddress, &c);
}

size_t res_net_recv(int sock, int fd, long size) {

    size_t len, received = 0, left = (size_t) size;
    int bufSize = RS_SIZE_DATA;

    unsigned char *buffer = malloc(RS_SIZE_DATA);
    if (buffer == NULL) {
        return 0;
    }

    memset(buffer, 0, RS_SIZE_DATA);
    while (left > 0) {
        if (left < bufSize) bufSize = left;
        len = (size_t) rs_net_recvall(sock, buffer, bufSize, 0);
        fwrite(buffer, 1, len, (FILE *) &fd);
        left -= len;
        received += len;
    }

    free(buffer);

    return received;
}

int rs_net_recvall(int sock, void *buffer, int size, int flags) {

    int len;
    size_t sizeLeft = (size_t) size;

    while (sizeLeft) {
        len = recv(sock, buffer, sizeLeft, flags);
        if (len == 0) {
            size = 0;
            break;
        };
        if (len == -1) {
            break;
        } else {
            sizeLeft -= len;
            buffer += len;
        }
    }
    return size;
}
