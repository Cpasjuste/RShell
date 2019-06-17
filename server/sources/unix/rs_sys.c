//
// Created by cpasjuste on 17/06/19.
//

#include <time.h>
#include <errno.h>

#include "rs_sys.h"

void rs_sleep(unsigned int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
