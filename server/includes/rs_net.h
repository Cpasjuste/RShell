//
// Created by cpasjuste on 16/06/19.
//

#ifndef LIBRSHELL_RS_NET_H
#define LIBRSHELL_RS_NET_H

int rs_net_init();

int rs_net_exit();

int rs_net_bind(int sock, int port);

int rs_net_close(int sock);

int rs_net_get_sock(int sock);

size_t rs_net_recv(int sock, int fd, long size);

int rs_net_recvall(int sock, void *buffer, int size, int flags);

#endif //LIBRSHELL_RS_NET_H
