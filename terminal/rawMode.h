#ifndef RAWMODE_H
#define RAWMODE_H
extern int k;
typedef enum { LEFT_ARROW_KEY = 300, UP_ARROW_KEY, RIGHT_ARROW_KEY, DOWN_ARROW_KEY } arrowKey;
void disableRawMode();
void enableRawMode();
void readK();
#endif