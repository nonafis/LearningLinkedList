#ifndef SLLIST_H
#define SLLIST_H
typedef struct SNode
{
    int data;
    struct SNode *next;
} SNode;

SNode *createSNode(int d);
void printSNode(SNode *head);

void insertSNode(SNode **headadd, SNode *precederSNode, SNode *newSNode);
int deleteSNode(SNode **headadd, SNode *precSNode);

SNode *getTail(SNode *head);
SNode *getByPos(SNode *head, int pos);
SNode *getByKey(SNode *head, int key);
SNode *getPriorToTail(SNode *head);
SNode *getPriorToKey(SNode *head, int key);

void appendSNode(SNode **headadd, int d);
void sinsertAtHead(SNode **headadd, int d);
void sinsertAtPos(SNode **headadd, int d, int pos);
void sinsertAfterKey(SNode **headadd, int d, int key);
void sinsertBeforeKey(SNode **headadd, int d, int key);

void sdeleteAtHead(SNode **headadd);
void sdeleteAtTail(SNode **headadd);
void sdeleteAtPos(SNode **headadd, int pos);
void sdeleteTheKey(SNode **headadd, int key);
void sdeleteAllKey(SNode **headadd, int key);

void freeSNode(SNode **headadd);
#endif