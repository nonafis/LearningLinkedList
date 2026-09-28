#ifndef SLLIST_H
#define SLLIST_H
#include "../convention.h"
typedef struct SNode
{
    int data;
    struct SNode *next;
} SNode;

opStatus createSNode(int d, SNode **newSNodeadd);
// void printSNode(SNode *head);

void insertSNode(SNode **headadd, SNode *precederSNode, SNode *newSNode);
opStatus deleteSNode(SNode **headadd, SNode *precSNode);

SNode *getTail(SNode *head);
opStatus getByPos(SNode *head, int pos, SNode **posNodeadd);
opStatus getByKey(SNode *head, int key, SNode **keyNodeadd);
opStatus getPriorToTail(SNode *head, SNode **priorToTailadd);
opStatus getPriorToKey(SNode *head, int key, SNode **priorToKeyadd);

opStatus appendSNode(SNode **headadd, int d);
opStatus sinsertAtHead(SNode **headadd, int d);
opStatus sinsertAtPos(SNode **headadd, int d, int pos);
opStatus sinsertAfterKey(SNode **headadd, int d, int key);
opStatus sinsertBeforeKey(SNode **headadd, int d, int key);

static inline opStatus sdeleteAtHead(SNode **headadd)
{
    return deleteSNode(headadd, NULL);
}
opStatus sdeleteAtTail(SNode **headadd);
opStatus sdeleteAtPos(SNode **headadd, int pos);
opStatus sdeleteTheKey(SNode **headadd, int key);
opStatus sdeleteAllKey(SNode **headadd, int key);

void freeSNode(SNode **headadd);
#endif