#ifndef TIMER_H
#define TIMER_H
#ifdef _WIN32
#include<conio.h>
#include<windows.h>
static void timer(unsigned int deciseconds)
{
    Sleep(deciseconds*100);
}
static int timeoutC(unsigned int deciseconds)
{
    DWORD ms = deciseconds * 100;
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
#include<unistd.h>
#include <sys/select.h>
static void timer(unsigned int deciseconds)
{
    usleep(deciseconds*100000);
}
static int timeoutC(unsigned int deciseconds)
{
    fd_set read_fds;
    struct timeval timeout;
    
    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds); // 0 is the file descriptor for standard input (keyboard)

    timeout.tv_sec  = deciseconds / 10;
    timeout.tv_usec = (deciseconds % 10) * 100000;

    int ready = select(1, &read_fds, NULL, NULL, &timeout);
    return ready;
}

#endif
#endif