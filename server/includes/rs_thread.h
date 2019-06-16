//
// Created by cpasjuste on 16/06/19.
//

#ifdef __THREADS__
#ifndef LIBRSHELL_RS_THREAD_H
#define LIBRSHELL_RS_THREAD_H

#include "SDL2/SDL_thread.h"

struct rs_thread {
    void *handle;
};

#endif //LIBRSHELL_RS_THREAD_H
#endif //__THREADS__
