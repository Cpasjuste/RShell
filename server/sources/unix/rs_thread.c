//
// Created by cpasjuste on 16/06/19.
//

#ifdef __PTHREADS__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "rs_thread.h"

rs_thread_t *rs_thread_create(RsThreadEntry entry, void *args) {

    rs_thread_t *rs_thread = malloc(sizeof(rs_thread_t));
    if (rs_thread == NULL) {
        printf("rs_thread_create: malloc failed\n");
        return NULL;
    }

    if (pthread_create((pthread_t *) &rs_thread->handle, NULL, (void *(*)(void *)) entry, args)) {
        printf("rs_thread_create: pthread_create failed\n");
        free(rs_thread);
        return NULL;
    }

    return rs_thread;
}

void rs_thread_join(rs_thread_t *thread) {

    if (thread && thread->handle) {
        pthread_join((pthread_t) thread->handle, 0);
    }
}

void rs_thread_close(rs_thread_t *thread) {

    if (thread && thread->handle) {
        pthread_detach((pthread_t) thread->handle);
        free(thread);
    }
}

#endif //__PTHREADS__
