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
int sinsertAtPos(SNode **headadd, int d, int pos);
int sinsertAfterKey(SNode **headadd, int d, int key);
int sinsertBeforeKey(SNode **headadd, int d, int key);

int sdeleteAtHead(SNode **headadd);
int sdeleteAtTail(SNode **headadd);
int sdeleteAtPos(SNode **headadd, int pos);
int sdeleteTheKey(SNode **headadd, int key);
void sdeleteAllKey(SNode **headadd, int key);

void freeSNode(SNode **headadd);
#endif