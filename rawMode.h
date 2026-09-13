#ifndef RAWMODE_H
#define RAWMODE_H

#ifdef _WIN32
#include <conio.h>
static int rm = 0;
static char c = '\0';
static void disableRawMode()
{
    rm = 0;
}
static void enableRawMode()
{
    rm = 1;
    atexit(disableRawMode);
}
static void readK()
{
    c = _getch();
}
#else
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
static struct termios orig_termios;
static int rm = 0;
static void die(const char *s)
{
    perror(s);
    exit(1);
}
static void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
    else
        rm = 0;
}
static void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
    else
        rm = 1;
}
static char c = '\0';
static void readK()
{
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        die("read");
}

#endif
#endif