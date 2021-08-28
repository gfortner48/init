/*
 * Copyright (c) 2013-2020 Rich Felker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char **argv)
{
    sigset_t set, old;
    sigfillset(&set);
    sigprocmask(SIG_BLOCK, &set, &old);

    posix_spawnattr_t attr;
    posix_spawnattr_init(&attr);
    posix_spawnattr_setsigmask(&attr, &old);
    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID|POSIX_SPAWN_SETSIGMASK);

    for (;;) {
        extern char **environ;
        pid_t pid;
        if (posix_spawn(&pid, argv[1], 0, &attr, argv+1, environ))
            nanosleep(&(struct timespec){.tv_nsec=100000000}, 0);
        waitpid(pid, 0, 0);
    }
}
