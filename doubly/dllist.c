#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"
#include "sllist.h"

opStatus createIntDNode(int d, intDNode **newIntDNodeadd)
{
    *newIntDNodeadd = malloc(sizeof(intDNode));
    if (!(*newIntDNodeadd))
    {
        return OP_ALLOC_FAILED;
    }
    (*newIntDNodeadd)->intData = d;
    (*newIntDNodeadd)->link.prev = (*newIntDNodeadd)->link.next = NULL;
    return OP_SUCCESS;
}

opStatus createNADNode(LNode *d, NADNode **newNADNodeadd)
{
    *newNADNodeadd = malloc(sizeof(NADNode));
    if (!(*newNADNodeadd))
    {
        return OP_ALLOC_FAILED;
    }
    (*newNADNodeadd)->nodeAddressData = d;
    (*newNADNodeadd)->link.prev = (*newNADNodeadd)->link.next = NULL;
    return OP_SUCCESS;
}

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

static void unlinkDNode(LNode *nodeToUnlink)
{
    if (nodeToUnlink->prev != NULL)
    {
        nodeToUnlink->prev->next = nodeToUnlink->next;
    }
    if (nodeToUnlink->next != NULL)
    {
        nodeToUnlink->next->prev = nodeToUnlink->prev;
    }
    nodeToUnlink->prev = nodeToUnlink->next = NULL;
}

static void linkBeforeDNode(LNode *nodeToInject, LNode *injectBefore) //link a disconnected node before any node
{
    if (injectBefore->prev != NULL)
    {
        injectBefore->prev->next = nodeToInject;
    }
    nodeToInject->prev = injectBefore->prev;
    injectBefore->prev = nodeToInject;
    nodeToInject->next = injectBefore;
}

static void linkAfterDNode(LNode *nodeToInject, LNode *injectAfter) //link a disconnected node after any node
{
    if (injectAfter->next != NULL)
    {
        injectAfter->next->prev = nodeToInject;
    }
    nodeToInject->next = injectAfter->next;
    injectAfter->next = nodeToInject;
    nodeToInject->prev = injectAfter;
}

void moveBefore(LNode *nodeToMove, LNode *target)
{
    unlinkDNode(nodeToMove);
    linkBeforeDNode(nodeToMove, target);
}

void moveAfter(LNode *nodeToMove, LNode *target)
{
    unlinkDNode(nodeToMove);
    linkAfterDNode(nodeToMove, target);
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
    } // negative position are invalid anyways, but for empty list non negative position are invalid too. maybe I can add a check for list->length == 0 right at the top to pass OP_ERROR_EMPTY_LIST
    *posNodeadd = list->preHead.next;
    for (int i = 0; (i < pos); i++)
    {
        (*posNodeadd) = (*posNodeadd)->next;
    }
    return OP_SUCCESS;
}

opStatus dgetByIntKeyFrom(linkedList *list, int key, LNode *start, LNode **keyNodeadd)
{
    *keyNodeadd = NULL;
    LNode *cur = start;
    while ((cur != &(list->postTail)) && (((intDNode *)(cur))->intData != key))
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
    free(deadNode);
    --(list->length);
}

opStatus appendIntDList(linkedList *list, int d)
{
    intDNode *newNode;
    opStatus s = createIntDNode(d, &newNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, list->postTail.prev, &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertNAAtTail(linkedList *list, LNode *d)
{
    NADNode *newNADNode;
    opStatus s = createNADNode(d, &newNADNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, list->postTail.prev, &(newNADNode->link));
    return OP_SUCCESS;
}

opStatus dinsertIntAtHead(linkedList *list, int d)
{
    intDNode *newNode;
    opStatus s = createIntDNode(d, &newNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, &(list->preHead), &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertIntAtPos(linkedList *list, int d, int pos)
{
    if (pos == list->length)
    {
        return appendIntDList(list, d);
    }
    LNode *posNode;
    intDNode *newNode;
    opStatus s = dgetByPos(list, pos, &posNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createIntDNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, posNode->prev, &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertIntAfterKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    intDNode *newNode;
    opStatus s = dgetByIntKey(list, key, &keyNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createIntDNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    insertDNode(list, keyNode, &(newNode->link));
    return OP_SUCCESS;
}

opStatus dinsertIntBeforeKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    intDNode *newNode;
    opStatus s = dgetByIntKey(list, key, &keyNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createIntDNode(d, &newNode)) != OP_SUCCESS)
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

opStatus ddeleteTheIntKey(linkedList *list, int key)
{
    LNode *deadNode;
    opStatus s = dgetByIntKey(list, key, &deadNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    deleteDNode(list, deadNode);
    return OP_SUCCESS;
}

opStatus ddeleteAllIntKey(linkedList *list, int key)
{
    LNode *cur;
    opStatus s = dgetByIntKey(list, key, &cur);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    LNode *deadNode; // this uses two pointer to keep track of matching node so that it can continue scanning from the last deleted node
    while (s == OP_SUCCESS)
    {
        deadNode = cur;                                   // save the current matching node in the deadNode
        s = dgetByIntKeyFrom(list, key, cur->next, &cur); // this can only work(continue from after matching node) coz the matching node has not been deleted yet //, although there could have been another that will not need two LNode ptrs that is setting the cur to cur->prev before deleting then delete the cur->next and then continue from cur->next.
        deleteDNode(list, deadNode);
    }
    return OP_SUCCESS;
}

opStatus sortIntDList(linkedList *list)
{
    if (!list)
    {
        return OP_ERROR_INVALID_ARG;
    }
    LNode *head = list->preHead.next;
    if (head == &(list->postTail) || head->next == &(list->postTail))
    {
        return OP_SUCCESS;
    }
    int runWidth = 1;
    while (runWidth < list->length)
    {
        LNode *first, *second, *slimit;
        first = list->preHead.next;
        while (first != &(list->postTail))
        {
            second = first;
            for (int i = 0; second != &(list->postTail) && i < runWidth; i++)
            {
                second = second->next;
            }
            if (second == &(list->postTail))
            {
                break;
            }
            slimit = second;
            for (int i = 0; slimit != &(list->postTail) && i < runWidth; i++)
            {
                slimit = slimit->next;
            }
            while (first != second && second != slimit)
            {
                if (((intDNode *)first)->intData > ((intDNode *)second)->intData)
                {
                    second = second->next;
                    moveBefore(second->prev, first);
                }
                else
                {
                    first = first->next;
                }
            }
            first = slimit;
        }
        runWidth *= 2;
    }
    return OP_SUCCESS;
}

opStatus reverseList(linkedList *list)
{
    if (!list)
    {
        return OP_ERROR_INVALID_ARG;
    }
    LNode *current, *oldHead;
    oldHead = current = list->preHead.next;
    if (oldHead == &(list->postTail) || oldHead->next == &(list->postTail))
    {
        return OP_SUCCESS;
    }
    while (current != &(list->postTail))
    {
        LNode *next = current->next;
        current->next = current->prev;
        current->prev = next;
        current = next;
    }
    list->preHead.next = list->postTail.prev;
    list->preHead.next->prev = &(list->preHead);
    list->postTail.prev = oldHead;
    oldHead->next = &(list->postTail);
    return OP_SUCCESS;
}

void clearList(linkedList *list)
{
    if (!list)
    {
        return;
    }
    while (ddeleteHead(list) == OP_SUCCESS) // keep deleting at head until the list becomes empty which will the condition false itself
        ;
}

void freeList(linkedList **listadd)
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
        if ((s = appendIntDList(list, current->data)) != OP_SUCCESS)
        {
            freeList(&list);
            return s;
        }
        current = current->next;
    }
    *listadd = list;
    return OP_SUCCESS;
}