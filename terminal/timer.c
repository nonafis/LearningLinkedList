#include "timer.h"
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
void timer(unsigned int deciseconds)
{
    Sleep(deciseconds * 100);
}
int msTimeoutC(unsigned int milliseconds)
{
    DWORD ms = milliseconds;
    DWORD start = GetTickCount();

    while (GetTickCount() - start < ms)
    {
        if (_kbhit())
            return 1;
        Sleep(1);
    }
    return 0;
}
#else
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
void timer(unsigned int deciseconds)
{
    usleep(deciseconds * 100000);
}
int msTimeoutC(unsigned int milliseconds)
{
    while (1)
    {
        fd_set read_fds;
        struct timeval timeout;

        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds); // 0 is the file descriptor for stdin

        timeout.tv_sec = milliseconds / 1000;
        timeout.tv_usec = (milliseconds % 1000) * 1000;

        int ready = select(1, &read_fds, NULL, NULL, &timeout);
        if (ready >= 0)
        {
            return ready;
        }
        if (errno == EINTR)
        {
            continue;
        }
        perror("select");
        exit(1);
    }
}

#endif