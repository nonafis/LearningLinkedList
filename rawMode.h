#ifndef RAWMODE_H
#define RAWMODE_H

#ifdef _WIN32
#include <conio.h>
static int rm = 0;
static int c = '\0';
static int k = '\0';

static void disableRawMode()
{
    rm = 0;
}
static void enableRawMode()
{
    rm = 1;
    atexit(disableRawMode);
}
static int c2k(int c)
{
    if (c == 224)
    {
        c = _getch();
        if (c == 75)
        {
            return 300;
        }
        else if (c == 77)
        {
            return 400;
        }
        else
        {
            return c;
        }
    }
    else
    {
        return c;
    }
}
static void readK()
{
    c = _getch();
    k = c2k(c);
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
static int k = '\0';
static int arrowkeyinterpreter(char c)
{
    if (c == '\x1b')
    {
        if (read(STDIN_FILENO, &c, 1) != 1)
            return '\x1b';

        if (c == '[')
        {
            if (read(STDIN_FILENO, &c, 1) != 1)
                return '\x1b';
            if (c == 'D')
            {
                return 300;
            }
            else if (c == 'C')
            {
                return 400;
            }
            else
            {
                return (unsigned char)c;
            }
        }
        else
        {
            return (unsigned char)c;
        }
    }
    else
    {
        return (unsigned char)c;
    }
}
static void readK()
{
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        die("read");
    k = c2k(c);
}

#endif
#endif