#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sllist.h"
opStatus createSNode(int d, SNode **newSNodeadd)
{
    *newSNodeadd = malloc(sizeof(SNode));
    if ((*newSNodeadd) == NULL)
    {
        return OP_ALLOC_FAILED;
    }
    (*newSNodeadd)->data = d;
    (*newSNodeadd)->next = NULL;
    return OP_SUCCESS;
}

// void printSNode(SNode *head)
// {
//     SNode *current = head;
//     if (current == NULL)
//     {
//         printf("NULL\r\n");
//     }
//     else
//     {
//         while (current->next != NULL)
//         {
//             printf("%d->", current->data);
//             current = current->next;
//         }
//         printf("%d->NULL\r\n", current->data);
//     }
// }

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

opStatus getByPos(SNode *head, int pos, SNode **posNodeadd)
{
    *posNodeadd = NULL;
    if (head == NULL)
    {
        return OP_ERROR_EMPTY_LIST;
    }
    if (pos < 0) // in this order so that if it is empty list any position said is invalid anyway
    {
        return OP_ERROR_NEGATIVE_POSITION;
    }
    SNode *current = head;
    int i = 0;
    for (i = 0; (((current->next) != NULL) && (i < pos)); i++)
    {
        current = current->next;
    }
    if (i != pos)
    {
        return OP_ERROR_OUT_OF_RANGE_POSITION;
    }
    *posNodeadd = current;
    return OP_SUCCESS;
}

opStatus getByKey(SNode *head, int key, SNode **keyNodeadd)
{
    *keyNodeadd = NULL;
    if (head == NULL)
        return OP_ERROR_EMPTY_LIST;
    SNode *current = head;
    while ((current->next != NULL) && ((current->data) != key))
    {
        current = current->next;
    }
    if (current->data != key)
    {
        return OP_ERROR_KEY_NOT_FOUND;
    }
    *keyNodeadd = current;
    return OP_SUCCESS;
}

opStatus getPriorToTail(SNode *head, SNode **priorToTailadd)
{
    *priorToTailadd = NULL;
    if (head == NULL)
        return OP_ERROR_EMPTY_LIST;
    if (head->next == NULL)
        return OP_SINGLE_NODE_LIST;
    *priorToTailadd = head;
    while ((*priorToTailadd)->next->next != NULL)
    {
        (*priorToTailadd) = (*priorToTailadd)->next;
    }
    return OP_SUCCESS;
}

opStatus getPriorToKey(SNode *head, int key, SNode **priorToKeyadd)
{
    *priorToKeyadd = NULL;
    if (head == NULL)
        return OP_ERROR_EMPTY_LIST;
    if (head->data == key)
        return OP_KEY_AT_HEAD;
    SNode *current = head;
    while ((current->next != NULL) && ((current->next->data) != key))
    {
        current = current->next;
    }
    if (current->next == NULL)
    {
        return OP_ERROR_KEY_NOT_FOUND;
    }
    *priorToKeyadd = current;
    return OP_SUCCESS;
}

opStatus deleteSNode(SNode **headadd, SNode *precSNode)
{
    if (*headadd == NULL)
    {
        return OP_ERROR_EMPTY_LIST;
    }
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
    return OP_SUCCESS;
}

opStatus appendSNode(SNode **headadd, int d)
{
    SNode *tail = getTail(*headadd), *newNode;
    opStatus s = createSNode(d, &newNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    insertSNode(headadd, tail, newNode);
    return OP_SUCCESS;
}

opStatus sinsertAtHead(SNode **headadd, int d)
{
    SNode *newNode;
    opStatus s = createSNode(d, &newNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    insertSNode(headadd, NULL, newNode);
    return OP_SUCCESS;
}

opStatus sinsertAtPos(SNode **headadd, int d, int pos)
{
    SNode *precederSNode, *newNode;
    if (pos == 0) //going through once more before committing
    {
        return sinsertAtHead(headadd, d);
    }
    opStatus s = getByPos(*headadd, pos - 1, &precederSNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createSNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    insertSNode(headadd, precederSNode, newNode);
    return OP_SUCCESS;
}

opStatus sinsertAfterKey(SNode **headadd, int d, int key)
{
    SNode *keySNode, *newNode;
    opStatus s = getByKey(*headadd, key, &keySNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createSNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    insertSNode(headadd, keySNode, newNode);
    return OP_SUCCESS;
}

opStatus sinsertBeforeKey(SNode **headadd, int d, int key)
{
    SNode *priorToKey, *newNode;
    opStatus s = getPriorToKey(*headadd, key, &priorToKey);
    if (s == OP_KEY_AT_HEAD)
    {
        return sinsertAtHead(headadd, d);
    }
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if ((s = createSNode(d, &newNode)) != OP_SUCCESS)
    {
        return s;
    }
    insertSNode(headadd, priorToKey, newNode);
    return OP_SUCCESS;
}

opStatus sdeleteAtTail(SNode **headadd)
{
    SNode *priorToTail;
    opStatus s = getPriorToTail(*headadd, &priorToTail);
    if (s != OP_SUCCESS) //relies on gPTT failing implying tail is at head or it's empty list which sdAH can handle on its own too
    {
        return sdeleteAtHead(headadd);
    }
    deleteSNode(headadd, priorToTail);
    return OP_SUCCESS;
}

opStatus sdeleteAtPos(SNode **headadd, int pos)
{
    if (pos == 0)
    {
        return sdeleteAtHead(headadd);
    }
    SNode *precederSNode;
    opStatus s = getByPos(*headadd, pos - 1, &precederSNode);
    if (s != OP_SUCCESS)
    {
        return s;
    }
    if (precederSNode->next == NULL)
    {
        return OP_ERROR_OUT_OF_RANGE_POSITION;
    }
    deleteSNode(headadd, precederSNode);
    return OP_SUCCESS;
}

opStatus sdeleteTheKey(SNode **headadd, int key)
{
    SNode *priorToKey;
    opStatus s = getPriorToKey(*headadd, key, &priorToKey);
    if (s == OP_KEY_AT_HEAD)
    {
        return sdeleteAtHead(headadd);
    }
    if (s != OP_SUCCESS)
    {
        return s;
    }
    deleteSNode(headadd, priorToKey);
    return OP_SUCCESS;
}

opStatus sdeleteAllKey(SNode **headadd, int key)
{
    SNode *priorToKey;
    opStatus s = getPriorToKey(*headadd, key, &priorToKey);
    if (s != OP_KEY_AT_HEAD && s != OP_SUCCESS)
    {
        return s;
    }
    while (s == OP_KEY_AT_HEAD)
    {
        sdeleteAtHead(headadd); //cannnot fail
        s = getPriorToKey(*headadd, key, &priorToKey);
    }
    while (s == OP_SUCCESS)
    {
        deleteSNode(headadd, priorToKey);
        s = getPriorToKey(priorToKey, key, &priorToKey);
    }
    return OP_SUCCESS;
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