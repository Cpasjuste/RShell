//
// Created by cpasjuste on 16/06/19.
//

#include <stdio.h>
#include <unistd.h>
#include <rs_sys.h>
#include "rs_shell.h"
#include "rs_thread.h"

int main() {

    if (rs_shell_init() != 0) {
        return -1;
    }

    while (1) {
        rs_sleep(500);
    }

    return 0;
}
