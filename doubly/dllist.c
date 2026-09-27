#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"
#include "sllist.h"
#define START_OF(l) ((l)->preHead.next)

opStatus createDNode(int d, DNode **newNodeadd)
{
    *newNodeadd = malloc(sizeof(DNode));
    if (!(*newNodeadd))
    {
        return OP_ALLOC_FAILED;
    }
    (*newNodeadd)->data = d;
    (*newNodeadd)->link.prev = (*newNodeadd)->link.next = NULL;
    return OP_SUCCESS;
}

// MNode *createMNode(int d, LNode *nodeadd)
// {
//     MNode *newNode = malloc(sizeof(MNode));
//     if (!newNode)
//     {
//         printf("Node creation failed. Memory Allocation Unsuccessful.\r\n");
//         exit(1);
//     }
//     else
//     {
//         newNode->intData = d;
//         newNode->addressData = nodeadd;
//         newNode->link.prev = newNode->link.next = NULL;
//         return newNode;
//     }
// }

opStatus createList(linkedList **listadd)
{
    *listadd = malloc(sizeof(linkedList));
    if (!(*listadd))
    {
        return OP_ALLOC_FAILED;
    }
    initList(*listadd);
    return OP_SUCCESS;
}

void initList(linkedList *list)
{
    list->preHead.prev = NULL;
    list->preHead.next = &(list->postTail);
    list->postTail.next = NULL;
    list->postTail.prev = &(list->preHead);
    list->length = 0;
}

opStatus dgetByPos(linkedList *list, int pos, LNode **posNodeadd)
{
    *posNodeadd = NULL;
    if (pos < 0)
    {
        return OP_ERROR_NEGATIVE_POSITION;
    }
    if (pos >= list->length)
    {
        return OP_ERROR_OUT_OF_RANGE_POSITION;
    }
    *posNodeadd = list->preHead.next;
    for (int i = 0; (i < pos); i++)
    {
        (*posNodeadd) = (*posNodeadd)->next;
    }
    return OP_SUCCESS;
}

// opStatus dgetByKey(linkedList *list, int key, LNode **keyNodeadd) // redundant now
// {
//     return dgetByKeyFrom(keyNodeadd, list, key, list->preHead.next);
// }

opStatus dgetByKeyFrom(linkedList *list, int key, LNode *start, LNode **keyNodeadd)
{
    *keyNodeadd = NULL;
    LNode *cur = start;
    while ((cur != &(list->postTail)) && (((DNode *)(cur))->data != key))
    {
        cur = cur->next;
    }
    if (cur == &(list->postTail))
    {
        return OP_ERROR_KEY_NOT_FOUND;
    }
    *keyNodeadd = cur;
    return OP_SUCCESS;
}

void insertDNode(linkedList *list, LNode *priorNode, LNode *inserteeNode)
{
    inserteeNode->next = priorNode->next;
    inserteeNode->prev = priorNode;
    priorNode->next->prev = inserteeNode;
    priorNode->next = inserteeNode;
    ++(list->length);
}

void deleteDNode(linkedList *list, LNode *deadNode)
{
    deadNode->prev->next = deadNode->next;
    deadNode->next->prev = deadNode->prev;
    free((DNode *)deadNode);
    --(list->length);
}

opStatus appendDList(linkedList *list, int d)
{
    DNode *newNode;
    opStatus s = createDNode(d, &newNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, list->postTail.prev, &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertAtHead(linkedList *list, int d)
{
    DNode *newNode;
    opStatus s = createDNode(d, &newNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, &(list->preHead), &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertAtPos(linkedList *list, int d, int pos)
{
    if (pos == list->length)
    {
        return appendDList(list, d);
    }
    LNode *posNode;
    DNode *newNode;
    opStatus s = dgetByPos(list, pos, &posNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createDNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, posNode->prev, &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertAfterKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    DNode *newNode;
    opStatus s = dgetByKeyFrom(list, key, START_OF(list), &keyNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createDNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, keyNode, &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertBeforeKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    DNode *newNode;
    opStatus s = dgetByKeyFrom(list, key, START_OF(list), &keyNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createDNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    LNode *priorNode = keyNode->prev;
    insertDNode(list, priorNode, &(newNode->link));
    return OP_SUCCESS;
}

opStatus ddeleteHead(linkedList *list)
{
    if ((list->preHead.next) == &(list->postTail))
    {
        return OP_ERROR_EMPTY_LIST;
    }
    deleteDNode(list, list->preHead.next);
    return OP_SUCCESS;
}

opStatus ddeleteTail(linkedList *list)
{
    if (list->postTail.prev == &(list->preHead))
    {
        return OP_ERROR_EMPTY_LIST;
    }
    deleteDNode(list, list->postTail.prev);
    return OP_SUCCESS;
}

opStatus ddeleteAtPos(linkedList *list, int pos)
{
    LNode *deadNode;
    opStatus s = dgetByPos(list, pos, &deadNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    deleteDNode(list, deadNode);
    return OP_SUCCESS;
}

opStatus ddeleteTheKey(linkedList *list, int key)
{
    LNode *deadNode;
    opStatus s = dgetByKeyFrom(list, key, START_OF(list), &deadNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    deleteDNode(list, deadNode);
    return OP_SUCCESS;
}

opStatus ddeleteAllKey(linkedList *list, int key)
{
    LNode *cur;
    opStatus s = dgetByKeyFrom(list, key, START_OF(list), &cur);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    LNode *deadNode; // this uses two pointer to keep track of matching node so that it can continue scanning from the last deleted node
    while (s == OP_SUCCESS)
    {
        deadNode = cur;                                // save the current matching node in the deadNode
        s = dgetByKeyFrom(list, key, cur->next, &cur); // this can only work(continue from after matching node) coz the matching node has not been deleted yet //, although there could have been another that will not need two LNode ptrs that is setting the cur to cur->prev before deleting then delete the cur->next and then continue from cur->next.
        deleteDNode(list, deadNode);
    }
    return OP_SUCCESS;
}

void clearList(linkedList *list)
{
    if (!list)
    {
        return;
    }
    while (ddeleteHead(list) == OP_SUCCESS) //keep deleting at head until the list becomes empty which will the condition false itself
        ;
}

void freeDList(linkedList **listadd)
{
    clearList(*listadd);
    free((*listadd));
    *listadd = NULL;
}

opStatus s2d(SNode *head, linkedList **listadd)
{
    SNode *current = head;
    linkedList *list;
    opStatus s = createList(&list);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    while (current != NULL)
    {
        if ((s = appendDList(list, current->data)) != OP_SUCCESS)
        {
            freeDList(&list);
            return s;
        }
        current = current->next;
    }
    *listadd = list;
    return OP_SUCCESS;
}