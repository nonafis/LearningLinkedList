#ifndef DLLIST_H
#define DLLIST_H
#include "../convention.h"
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

// typedef struct MNode
// {
//     LNode link;
//     int intData;
//     LNode *addressData;
// } MNode;

typedef struct linkedList
{
    LNode preHead;
    LNode postTail;
    int length;
} linkedList;

opStatus createDNode(int d, DNode **newNodeadd);
// MNode *createMNode(int d, LNode *nodeadd);

void initList(linkedList *list);
opStatus createList(linkedList **listadd);

opStatus dgetByPos(linkedList *list, int pos, LNode **posNodeadd);
opStatus dgetByKeyFrom(linkedList *list, int key, LNode *start, LNode **keyNodeadd);
// LNode *dgetByKey(linkedList *list, int key);

void insertDNode(linkedList *list, LNode *priorNode, LNode *inserteeNode);
void deleteDNode(linkedList *list, LNode *deadNode);

opStatus appendDList(linkedList *list, int d);
opStatus dinsertAtHead(linkedList *list, int d);
opStatus dinsertAtPos(linkedList *list, int d, int pos);
opStatus dinsertAfterKey(linkedList *list, int d, int key);
opStatus dinsertBeforeKey(linkedList *list, int d, int key);

opStatus ddeleteHead(linkedList *list);
opStatus ddeleteTail(linkedList *list);
opStatus ddeleteAtPos(linkedList *list, int pos);
opStatus ddeleteTheKey(linkedList *list, int key);
opStatus ddeleteAllKey(linkedList *list, int key);

void clearList(linkedList *list);
void freeDList(linkedList **listadd);

opStatus s2d(SNode *head, linkedList **listadd);
#endif