#ifndef DLLIST_H
#define DLLIST_H
#include "sllist.h"
typedef struct LNode
{
    struct LNode *prev;
    struct LNode *next;
} LNode;

typedef struct DNode
{
    LNode link;
    int data;
} DNode;

typedef struct linkedList
{
    LNode preHead;
    LNode postTail;
    int length;
} linkedList;

DNode *createDNode(int d);

void initDList(linkedList *list);
linkedList *createDList();
void displayDList(linkedList *list);
void displayDListWithCursor(linkedList *list, LNode *cur);

LNode *dgetByPos(linkedList *list, int pos);
LNode *dgetByKeyFromNode(linkedList *list, int key, LNode *start);
LNode *dgetByKey(linkedList *list, int key);

void insertDNode(linkedList *list, LNode *priorNode, LNode *inserteeNode);
void deleteDNode(linkedList *list, LNode *deadNode);

void appendDList(linkedList *list, int d);
void dinsertAtHead(linkedList *list, int d);
int dinsertAtPos(linkedList *list, int d, int pos);
int dinsertAfterKey(linkedList *list, int d, int key);
int dinsertBeforeKey(linkedList *list, int d, int key);

void ddeleteHead(linkedList *list);
void ddeleteTail(linkedList *list);
int ddeleteAtPos(linkedList *list, int pos);
int ddeleteTheKey(linkedList *list, int key);
int ddeleteAllKey(linkedList *list, int key);

void clearList(linkedList *list);
void freeDList(linkedList **listadd);

linkedList *s2d(SNode *head);
#endif