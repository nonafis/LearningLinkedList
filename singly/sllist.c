#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sllist.h"
SNode *createSNode(int d)
{
    SNode *newSNode = malloc(sizeof(SNode));
    if (newSNode == NULL)
    {
        printf("SNode creation failed. Memory Allocation Unsuccessful.\r\n");
        exit(1);
    }
    else
    {
        newSNode->data = d;
        newSNode->next = NULL;
        return newSNode;
    }
}

void printSNode(SNode *head)
{
    SNode *current = head;
    if (current == NULL)
    {
        printf("NULL\r\n");
    }
    else
    {
        while (current->next != NULL)
        {
            printf("%d->", current->data);
            current = current->next;
        }
        printf("%d->NULL\r\n", current->data);
    }
}

void insertSNode(SNode **headadd, SNode *precederSNode, SNode *newSNode)
{
    assert((precederSNode == NULL) || ((*headadd) != NULL));
    if (precederSNode == NULL)
    {
        newSNode->next = *headadd;
        *headadd = newSNode;
    }
    else
    {
        newSNode->next = precederSNode->next;
        precederSNode->next = newSNode;
    }
}

SNode *getTail(SNode *head)
{
    if (head == NULL)
        return NULL;
    SNode *tail = head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }
    return tail;
}

SNode *getByPos(SNode *head, int pos)
{
    if (head == NULL)
        return NULL;
    SNode *current = head;
    for (int i = 0; (((current->next) != NULL) && (i < pos)); i++)
    {
        current = current->next;
    }
    return current;
}

SNode *getByKey(SNode *head, int key)
{
    if (head == NULL)
        return NULL;
    SNode *current = head;
    while ((current->next != NULL) && ((current->data) != key))
    {
        current = current->next;
    }
    return current;
}

SNode *getPriorToTail(SNode *head)
{
    if (head == NULL)
        return NULL;
    if (head->next == NULL)
        return NULL;
    SNode *priorToTail = head;
    while (priorToTail->next->next != NULL)
    {
        priorToTail = priorToTail->next;
    }
    return priorToTail;
}

SNode *getPriorToKey(SNode *head, int key)
{
    if (head == NULL)
        return NULL;
    if (head->next == NULL)
        return NULL;
    SNode *current = head;
    while ((current->next != NULL) && ((current->next->data) != key))
    {
        current = current->next;
    }
    return current;
}

int deleteSNode(SNode **headadd, SNode *precSNode)
{
    if (*headadd != NULL)
    {
        if (precSNode == NULL)
        {
            SNode *deadSNode = *headadd;
            *headadd = (*headadd)->next;
            free(deadSNode);
        }
        else
        {
            assert(precSNode->next != NULL);
            SNode *deadSNode = precSNode->next;
            precSNode->next = precSNode->next->next;
            free(deadSNode);
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

void appendSNode(SNode **headadd, int d)
{
    SNode *tail = getTail(*headadd);
    insertSNode(headadd, tail, createSNode(d));
}

void sinsertAtHead(SNode **headadd, int d)
{
    insertSNode(headadd, NULL, createSNode(d));
}

int sinsertAtPos(SNode **headadd, int d, int pos)
{
    if (pos < 0)  //if the pos is >= length of the list, the node silently gets appended not errored
    {
        printf("Invalid position! Position can't be negative.\r\n");
        return 0;
    }
    if (*headadd == NULL || pos == 0)
    {
        insertSNode(headadd, NULL, createSNode(d)); // if empty list then newSNode becomes head and its next is previous *headadd which was NULL, if it not empty but pos is 0 then newSNode becomes head and previous list gets attached to the next of newSNode.
    }
    else
    {
        SNode *precederSNode = getByPos(*headadd, pos - 1);
        insertSNode(headadd, precederSNode, createSNode(d));
    }
    return 1;
}

int sinsertAfterKey(SNode **headadd, int d, int key)
{
    if (*headadd == NULL)
    {
        printf("Insertion using key cannot be done in an empty list.\r\n");
        return 0;
    }
    else
    {
        SNode *keySNode = getByKey(*headadd, key);
        if ((keySNode->next == NULL) && (keySNode->data != key))
        {
            printf("Key not found!\r\n");
            return 0;
        }
        else
        {
            insertSNode(headadd, keySNode, createSNode(d));
            return 1;
        }
    }
}

int sinsertBeforeKey(SNode **headadd, int d, int key)
{
    if (*headadd == NULL)
    {
        printf("Insertion using key cannot be done in an empty list.\r\n");
        return 0;
    }
    SNode *priorToKey = getPriorToKey(*headadd, key);
    if (priorToKey == NULL) //can be rearranged? ig
    {
        if (((*headadd)==NULL)||((*headadd)!=NULL&&(*headadd)->data==key))
        {            
            sinsertAtHead(headadd, d);
            return 1;
        }
        else 
        {
            printf("Key not found!\r\n");
            return 0;
        }
    }
    else if (priorToKey->next == NULL)
    {
        printf("Key not found!\r\n");
        return 0;
    }
    else
    {
        insertSNode(headadd, priorToKey, createSNode(d));
        return 1;
    }
}

int sdeleteAtHead(SNode **headadd)
{
    if (deleteSNode(headadd, NULL))
    {
        printf("List is empty. Nothing to delete.\r\n");
        return 0;
    }
    return 1;
}

int sdeleteAtTail(SNode **headadd)
{
    SNode *priorToTail = getPriorToTail(*headadd);
    if (priorToTail == NULL)
    {

        return sdeleteAtHead(headadd);
    }
    else
    {
        deleteSNode(headadd, priorToTail);
        return 1;
    }
}

int sdeleteAtPos(SNode **headadd, int pos)
{
    if (pos < 0)
    {
        printf("Invalid position! Position can't be negative.\r\n");
        return 0;
    }
    if (pos == 0)
    {
        return sdeleteAtHead(headadd);
    }
    SNode *precederSNode = getByPos(*headadd, pos - 1);
    if (precederSNode == NULL)
    {
        printf("List is empty. Nothing to delele!\r\n");
        return 0;
    }
    else if (precederSNode->next == NULL)
    {
        printf("Invalid Position! Position is out of range.\r\n");
        return 0;
    }
    deleteSNode(headadd, precederSNode);
    return 1;
}

int sdeleteTheKey(SNode **headadd, int key)
{
    SNode *priorToKey = getPriorToKey(*headadd, key);
    if (((*headadd)==NULL)||((*headadd)!=NULL&&(*headadd)->data==key))
    {
        return sdeleteAtHead(headadd);
    }
    else if (priorToKey->next == NULL)
    {
        printf("Key not found!\r\n");
        return 0;
    }
    else
    {
        deleteSNode(headadd, priorToKey);
        return 1;
    }
}

void sdeleteAllKey(SNode **headadd, int key)
{
    SNode *priorToKey = getPriorToKey(*headadd, key);
    while ((((*headadd)==NULL)||((*headadd)!=NULL&&(*headadd)->data==key)))
    {
        sdeleteAtHead(headadd);
        if (*headadd == NULL)
            return;
        priorToKey = getPriorToKey(*headadd, key);
    }
    while (priorToKey->next != NULL)
    {
        deleteSNode(headadd, priorToKey);
        priorToKey = getPriorToKey(priorToKey, key);
    }
}

void freeSNode(SNode **headadd)
{
    SNode *current = *headadd;
    SNode *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    *headadd = NULL;
}