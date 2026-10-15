#include <stdlib.h>
#include <string.h>
#include "rawMode.h"
#include "convention.h"
#ifdef _WIN32
#include <conio.h>
static int rm = 0;

void disableRawMode()
{
    rm = 0;
}
void enableRawMode()
{
    rm = 1;
}
void readB(char *dest)
{
    int c1 = '\0', c2 = '\0';
    c1 = _getch();
    if (c1 != 224 && c1 != 0)
    {
        dest[0] = c1;
        dest[1] = '\0';
        return;
    }
    c2 = _getch();
    if (c1 == 224 && c2 == 75)
    {
        strcpy(dest, LEFT_ARROW_CHAR);
    }
    else if (c1 == 224 && c2 == 77)
    {
        strcpy(dest, RIGHT_ARROW_CHAR);
    }
    else if (c1 == 224)
    {
        dest[0] = 224;
        dest[1] = '\0';
    }
    else
    {
        dest[0] = 255;
        dest[1] = '\0';
    }
}
#else
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "timer.h"
static int enabledOnce = 0;
static struct termios orig_termios;
static void die(const char *s)
{
    perror(s);
    exit(1);
}
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
void enableRawMode()
{
    if (!enabledOnce)
    {
        if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
        atexit(disableRawMode);
        enabledOnce = 1;
    }
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}
static void readK(char *ch)
{
    ssize_t n;
    while ((n = read(STDIN_FILENO, ch, 1)) == -1 && errno == EAGAIN)
        ;

    if (n == -1)
        die("read");
}
void readB(char *dest)
{
    char c1 = '\0';
    readK(&c1);
    if (c1 != '\x1b')
    {
        dest[0] = c1;
        dest[1] = '\0';
        return;
    }
    //means the character read was ESC
    if (msTimeoutC(50) == 0) // if it timedout then it was not a sequence, instead solely ESC was pressed deliberately
    {
        strcpy(dest, "\x1b");
        return;
    }
    //means it didnt timedout (it was an escape sequence)
    char c2 = '\0';
    readK(&c2);
    if (c2 != '[' && c2 != 'O') //means it is 2-character legacy sequence
    {
        strcpy(dest, "\x1b");
        return;
    }
    //otherwise, if it was arrow_char
    char c3 = '\0';
    readK(&c3);
    if (c3 == 'D')
    {
        strcpy(dest, LEFT_ARROW_CHAR);
    }
    else if (c3 == 'C')
    {
        strcpy(dest, RIGHT_ARROW_CHAR);
    }
    //or if it was some other other escape sequence
    else
    {
        while (!((c3 >= 'A' && c3 <= 'Z') || 
    (c3 >= 'a' && c3 <= 'z') || 
    c3 == '~'))
        {
            readK(&c3);
        }
        strcpy(dest, "\x1b");
    }
}

#endif