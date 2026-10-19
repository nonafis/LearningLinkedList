#ifndef DLLIST_H
#define DLLIST_H
#include "convention.h"
#include "sllist.h"
typedef struct LNode
{
    struct LNode *prev;
    struct LNode *next;
} LNode;

//Here, "DNode" means any node type whose first member is an LNode. The LNode provides the links used to traverse the node in a linkedList. Specific DNode types may use different payload types; payload-specific operations must only be used with the matching node type.

typedef struct intDNode
{
    LNode link;
    int intData;
} intDNode;

typedef struct NADNode
{
    LNode link;
    LNode *nodeAddressData;
} NADNode; // NA = node address = LNode * = the data type of the payload of the said DNode

typedef struct linkedList
{
    LNode preHead;
    LNode postTail;
    int length;
} linkedList;

opStatus createIntDNode(int d, intDNode **newIntNodeadd);
opStatus createNADNode(LNode *d, NADNode **newNADNodeadd);

void initList(linkedList *list);
opStatus createList(linkedList **listadd);

opStatus dgetByPos(linkedList *list, int pos, LNode **posNodeadd);
opStatus dgetByIntKeyFrom(linkedList *list, int key, LNode *start, LNode **keyNodeadd);
static inline opStatus dgetByIntKey(linkedList *list, int key, LNode **keyNodeadd)
{
    return dgetByIntKeyFrom(list, key, list->preHead.next, keyNodeadd);
}

void insertDNode(linkedList *list, LNode *priorNode, LNode *inserteeNode);
void deleteDNode(linkedList *list, LNode *deadNode);

opStatus appendIntDList(linkedList *list, int d);
opStatus dinsertIntAtHead(linkedList *list, int d);
opStatus dinsertIntAtPos(linkedList *list, int d, int pos);
opStatus dinsertIntAfterKey(linkedList *list, int d, int key);
opStatus dinsertIntBeforeKey(linkedList *list, int d, int key);

opStatus ddeleteHead(linkedList *list);
opStatus ddeleteTail(linkedList *list);
opStatus ddeleteAtPos(linkedList *list, int pos);
opStatus ddeleteTheIntKey(linkedList *list, int key);
opStatus ddeleteAllIntKey(linkedList *list, int key);

opStatus dinsertNAAtTail(linkedList *list, LNode *d);

opStatus sortIntDList(linkedList *list);
opStatus reverseList(linkedList *list);

void clearList(linkedList *list);
void freeList(linkedList **listadd);

opStatus s2d(SNode *head, linkedList **listadd);
#endif