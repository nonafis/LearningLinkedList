#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sllist.h"
SNode *createSNode(int d)
{
    SNode *newSNode = malloc(sizeof(SNode));
    if (newSNode == NULL)
    {
        printf("SNode creation failed. Memory Allocation Unsuccessful.\n");
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
        printf("NULL\n");
    }
    else
    {
        while (current->next != NULL)
        {
            printf("%d->", current->data);
            current = current->next;
        }
        printf("%d->NULL\n", current->data);
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
    if (head->data == key)
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

void insert_at_head(SNode **headadd, int d)
{
    insertSNode(headadd, NULL, createSNode(d));
}

void insert_at_pos(SNode **headadd, int d, int pos)
{
    if (pos < 0)
    {
        printf("Invalid position! Position can't be negative.\n");
        return;
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
}

void insert_after_key(SNode **headadd, int d, int key)
{
    if (*headadd == NULL)
    {
        printf("Insertion using key cannot be done in an empty list.\n");
    }
    else
    {
        SNode *keySNode = getByKey(*headadd, key);
        if ((keySNode->next == NULL) && (keySNode->data != key))
        {
            printf("Key not found!\n");
        }
        else
        {
            insertSNode(headadd, keySNode, createSNode(d));
        }
    }
}

void insert_before_key(SNode **headadd, int d, int key)
{
    if (*headadd == NULL)
    {
        printf("Insertion using key cannot be done in an empty list.\n");
        return;
    }
    SNode *priorToKey = getPriorToKey(*headadd, key);
    if (priorToKey == NULL)
    {
        insert_at_head(headadd, d);
    }
    else if (priorToKey->next == NULL)
    {
        printf("Key not found!\n");
    }
    else
    {
        insertSNode(headadd, priorToKey, createSNode(d));
    }
}

void delete_at_head(SNode **headadd)
{
    if (deleteSNode(headadd, NULL))
    {
        printf("List is empty. Nothing to delete.\n");
    }
}

void delete_at_tail(SNode **headadd)
{
    SNode *priorToTail = getPriorToTail(*headadd);
    if (priorToTail == NULL)
    {

        delete_at_head(headadd);
    }
    else
    {
        deleteSNode(headadd, priorToTail);
    }
}

void delete_at_pos(SNode **headadd, int pos)
{
    if (pos < 0)
    {
        printf("Invalid position! Position can't be negative.\n");
        return;
    }
    if (pos == 0)
    {
        delete_at_head(headadd);
        return;
    }
    SNode *precederSNode = getByPos(*headadd, pos - 1);
    if (precederSNode == NULL)
    {
        printf("List is empty. Nothing to delele!\n");
        return;
    }
    else if (precederSNode->next == NULL)
    {
        printf("Invalid Position! Position is out of range.\n");
        return;
    }
    deleteSNode(headadd, precederSNode);
}

void delete_the_key(SNode **headadd, int key)
{

    SNode *priorToKey = getPriorToKey(*headadd, key);
    if (priorToKey == NULL)
    {
        delete_at_head(headadd);
    }
    else if (priorToKey->next == NULL)
    {
        printf("Key not found!\n");
    }
    else
    {
        deleteSNode(headadd, priorToKey);
    }
}

void delete_all_key(SNode **headadd, int key)
{
    SNode *priorToKey = getPriorToKey(*headadd, key);
    while (priorToKey == NULL)
    {
        delete_at_head(headadd);
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

void freeList(SNode **headadd)
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