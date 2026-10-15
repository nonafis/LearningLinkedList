#include <stdio.h>
#include <stdlib.h>
#include "terminalconfig.h"
#ifdef _WIN32
#include <windows.h> //for SetConsoleOutputCP, GetStdHandle, GetConsoleScreenBufferInfo
int getTerminalWidth(void)
{
    int width = 0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        if ((width = csbi.srWindow.Right - csbi.srWindow.Left + 1) >= 20)
        {
            return width;
        }
    }
    return 20;
}
#else
#include <sys/ioctl.h>
#include <unistd.h>
int getTerminalWidth(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        if (w.ws_col>=20)
        {
            return w.ws_col;
        }
    }
    return 20;
}
#endif
void resetTerminal() {
    // Exit alternate screen buffer & restore the cursor when closing
    printf("\033[?1049l\033[?25h");
    fflush(stdout);
}
int initTerminal(void)
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    printf("\033[?1049h\033[?25l");
    fflush(stdout);
    atexit(resetTerminal);
    return getTerminalWidth();
}