#include <stdio.h>

void clearScreen()
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

void clearLine()
{
    printf("\033[A\033[2K");
    fflush(stdout);
}