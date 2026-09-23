#ifndef CLI_H
#define CLI_H
#include "dllist.h"
int emptylistmenu(linkedList *list);
int nonemptylistmenu(linkedList *list);
int navigationmenu(linkedList *list);
int insertionmenu(linkedList *list);
int deletionmenu(linkedList *list);

int getInput(const char *);
void invalidInput();
void displayDList(linkedList *list);
void displayDListWithCursor(linkedList *list, LNode *cur);
void printnavkeyhints(int leftedgeboolean, int rightedgeboolean, const char *leftarrowhintmessage, const char *rightarrowhintmessage, const char *ihintmessage, const char *dhintmessage);
#endif