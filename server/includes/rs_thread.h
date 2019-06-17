//
// Created by cpasjuste on 16/06/19.
//

#ifndef LIBRSHELL_RS_THREAD_H
#define LIBRSHELL_RS_THREAD_H

typedef struct {
    void *handle;
} rs_thread_t;

typedef void (*RsThreadEntry)(void *);

rs_thread_t *rs_thread_create(RsThreadEntry entry, void *args);

void rs_thread_join(rs_thread_t *thread);

void rs_thread_close(rs_thread_t *thread);

#endif //LIBRSHELL_RS_THREAD_H
