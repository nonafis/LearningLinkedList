#ifndef CLI_H
#define CLI_H
#include "dllist.h"
int emptylistmenu(linkedList *list);
int nonemptylistmenu(linkedList *list);
int navigationmenu(linkedList *list);
int insertionmenu(linkedList *list);
int deletionmenu(linkedList *list);

int getInput(const char *);
void printlnreo();
void printleo();
void printreo();
void printo();
void invalidInput();
#endif