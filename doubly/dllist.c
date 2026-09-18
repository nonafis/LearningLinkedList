#include <stdio.h>
#include <stdlib.h>
#include "sllist.h"
#include "dllist.h"

DNode *createDNode(int d)
{
    DNode *newNode = malloc(sizeof(DNode));
    if (!newNode)
    {
        printf("Node creation failed. Memory Allocation Unsuccessful.\n");
        exit(1);
    }
    else
    {
        newNode->data = d;
        newNode->link.prev = newNode->link.next = NULL;
        return newNode;
    }
}

linkedList *createDList()
{
    linkedList *newList = malloc(sizeof(linkedList));
    if (!newList)
    {
        printf("Allocation failed!\n");
        exit(1);
    }
    initDList(newList);
    return newList;
}

void initDList(linkedList *list)
{
    list->preHead.prev = NULL;
    list->preHead.next = &(list->postTail);
    list->postTail.next = NULL;
    list->postTail.prev = &(list->preHead);
    list->length = 0;
}

LNode *dgetByPos(linkedList *list, int pos)
{
    if (pos < 0)
    {
        printf("Position can't be negative.\n");
        fflush(stdout);
        return NULL;
    }
    if (pos >= list->length)
    {
        printf("Position out of range.\n");
        fflush(stdout);
        return NULL;
    }
    LNode *posNode = list->preHead.next;
    for (int i = 0; (i < pos); i++)
    {
        posNode = posNode->next;
    }
    return posNode;
}

LNode *dgetByKey(linkedList *list, int key)
{
    LNode *keyNode = dgetByKeyFromNode(list, key, list->preHead.next);
    if (!keyNode)
        printf("Key not found.\n");
    return keyNode;
}

LNode *dgetByKeyFromNode(linkedList *list, int key, LNode *start)
{
    LNode *cur = start;
    while ((cur != &(list->postTail)) && (((DNode *)(cur))->data != key))
    {
        cur = cur->next;
    }
    return cur == &(list->postTail) ? NULL : cur;
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

void appendDList(linkedList *list, int d)
{
    DNode *newNode = createDNode(d);
    insertDNode(list, list->postTail.prev, &(newNode->link));
}

void dinsertAtHead(linkedList *list, int d)
{
    DNode *newNode = createDNode(d);
    insertDNode(list, &(list->preHead), &(newNode->link));
}

int dinsertAtPos(linkedList *list, int d, int pos)
{
    if (pos == list->length)
    {
        appendDList(list, d);
        return 1;
    }
    LNode *posNode;
    if ((posNode = dgetByPos(list, pos)))
    {
        LNode *priorNode = posNode->prev;
        insertDNode(list, priorNode, &(createDNode(d)->link));
        return 1;
    }
    else
    {
        return 0;
    }
}

int dinsertAfterKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    if ((keyNode = dgetByKey(list, key)))
    {
        insertDNode(list, keyNode, &(createDNode(d)->link));
        return 1;
    }
    else
    {
        return 0;
    }
}

int dinsertBeforeKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    if ((keyNode = dgetByKey(list, key)))
    {
        LNode *priorNode = keyNode->prev;
        insertDNode(list, priorNode, &(createDNode(d)->link));
        return 1;
    }
    else
    {
        return 0;
    }
}

void ddeleteHead(linkedList *list)
{
    if ((list->preHead.next) != &(list->postTail))
        deleteDNode(list, list->preHead.next);
}

void ddeleteTail(linkedList *list)
{
    if (list->postTail.prev != &(list->preHead))
    {
        deleteDNode(list, list->postTail.prev);
    }
}

int ddeleteAtPos(linkedList *list, int pos)
{
    LNode *deadNode = dgetByPos(list, pos);
    if (deadNode)
    {
        deleteDNode(list, deadNode);
        return 1;
    }
    else
    {
        return 0;
    }
}

int ddeleteTheKey(linkedList *list, int key)
{
    LNode *deadNode = dgetByKey(list, key);
    if (deadNode)
    {
        deleteDNode(list, deadNode);
        return 1;
    }
    else
    {
        return 0;
    }
}

int ddeleteAllKey(linkedList *list, int key)
{
    LNode *cur;
    if (!(cur = dgetByKeyFromNode(list, key, list->preHead.next)))
    {
        printf("Key not found!\n");
        return 0;
    }
    LNode *deadNode;
    while (cur)
    {
        deadNode = cur;
        cur = dgetByKeyFromNode(list, key, cur->next);
        deleteDNode(list, deadNode);
    }
    return 1;
}

void displayDList(linkedList *list)
{
    printf("========================Linked List========================\n");
    LNode *dcurrent = list->preHead.next;
    while ((dcurrent != &(list->postTail)))
    {
        printf("%d<->", ((DNode *)(dcurrent))->data);
        dcurrent = dcurrent->next;
    }
    printf("NULL\n");
}

void displayDListWithCursor(linkedList *list, LNode *cur)
{
    int cursoroffset = 0;
    printf("========================Linked List========================\n");
    LNode *dcurrent = list->preHead.next;
    while (dcurrent != &(list->postTail) && (dcurrent != cur))
    {
        cursoroffset += printf("%d<->", ((DNode *)(dcurrent))->data);
        dcurrent = dcurrent->next;
    }
    if (!(dcurrent == cur))
    {
        printf("NULL\ntf you had given as cur parameter huh! -_-\n");
        return;
    }
    while (dcurrent != &(list->postTail))
    {
        printf("%d<->", ((DNode *)(dcurrent))->data);
        dcurrent = dcurrent->next;
    }
    printf("NULL\n");
    for (int i = 0; i < cursoroffset; i++)
    {
        printf(" ");
    }
    printf("^\n");
}

void clearList(linkedList *list)
{
    if (!list)
    {
        return;
    }
    LNode *dcurrent = (list)->preHead.next;
    while (dcurrent != &((list)->postTail))
    {
        dcurrent = dcurrent->next;
        dcurrent->prev = dcurrent->prev->prev;
        free((DNode *)dcurrent->prev->next);
        dcurrent->prev->next = dcurrent;
        --(list->length);
    }
}

void freeDList(linkedList **listadd)
{
    clearList(*listadd);
    free((*listadd));
    *listadd = NULL;
}

linkedList *s2d(SNode *head)
{
    SNode *current = head;
    linkedList *list = createDList();
    while (current != NULL)
    {
        appendDList(list, current->data);
        current = current->next;
    }
    return list;
}