#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node
{
    int data;
    struct node *next;
} Node;

Node *createNode(int d)
{
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Node creation failed. Memory Allocation Unsuccessful.\n");
        exit(1);
    }
    else
    {
        newNode->data = d;
        newNode->next = NULL;
        return newNode;
    }
}

void printNode(Node *head)
{
    Node *current = head;
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

void insertNode(Node **headadd, Node *precederNode, Node *newNode)
{
    assert((precederNode == NULL) || ((*headadd) != NULL));
    if (precederNode == NULL)
    {
        newNode->next = *headadd;
        *headadd = newNode;
    }
    else
    {
        newNode->next = precederNode->next;
        precederNode->next = newNode;
    }
}

Node *traverseToTail(Node *head)
{
    if (head == NULL)
        return NULL;
    Node *tail = head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }
    return tail;
}

Node *traverseToPos(Node *head, int pos)
{
    if (head == NULL)
        return NULL;
    Node *current = head;
    for (int i = 0; (((current->next) != NULL) && (i < pos)); i++)
    {
        current = current->next;
    }
    return current;
}

Node *traverseToKey(Node *head, int key)
{
    if (head == NULL)
        return NULL;
    Node *current = head;
    while ((current->next != NULL) && ((current->data) != key))
    {
        current = current->next;
    }
    return current;
}

Node *traverseToPriorToTail(Node *head)
{
    if (head == NULL)
        return NULL;
    if (head->next == NULL)
        return NULL;
    Node *priorToTail = head;
    while (priorToTail->next->next != NULL)
    {
        priorToTail = priorToTail->next;
    }
    return priorToTail;
}

Node *traverseToPriorToKey(Node *head, int key)
{
    if (head == NULL)
        return NULL;
    if (head->data == key)
        return NULL;
    Node *current = head;
    while ((current->next != NULL) && ((current->next->data) != key))
    {
        current = current->next;
    }
    return current;
}

int deleteNode(Node **headadd, Node *precNode)
{
    if (*headadd != NULL)
    {
        if (precNode == NULL)
        {
            Node *deadNode = *headadd;
            *headadd = (*headadd)->next;
            free(deadNode);
        }
        else
        {
            assert(precNode->next != NULL);
            Node *deadNode = precNode->next;
            precNode->next = precNode->next->next;
            free(deadNode);
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

void appendNode(Node **headadd, int d)
{
    Node *tail = traverseToTail(*headadd);
    insertNode(headadd, tail, createNode(d));
}

void insert_at_head(Node **headadd, int d)
{
    insertNode(headadd, NULL, createNode(d));
}

void insert_at_pos(Node **headadd, int d, int pos)
{
    if (pos < 0)
    {
        printf("Invalid position! Position can't be negative.\n");
        return;
    }
    if (*headadd == NULL || pos == 0)
    {
        insertNode(headadd, NULL, createNode(d)); // if empty list then newNode becomes head and its next is previous *headadd which was NULL, if it not empty but pos is 0 then newNode becomes head and previous list gets attached to the next of newNode.
    }
    else
    {
        Node *precederNode = traverseToPos(*headadd, pos - 1);
        insertNode(headadd, precederNode, createNode(d));
    }
}

void insert_after_key(Node **headadd, int d, int key)
{
    if (*headadd == NULL)
    {
        printf("Insertion using key cannot be done in an empty list.\n");
    }
    else
    {
        Node *keyNode = traverseToKey(*headadd, key);
        if ((keyNode->next == NULL) && (keyNode->data != key))
        {
            printf("Key not found!\n");
        }
        else
        {
            insertNode(headadd, keyNode, createNode(d));
        }
    }
}

void insert_before_key(Node **headadd, int d, int key)
{
    if (*headadd == NULL)
    {
        printf("Insertion using key cannot be done in an empty list.\n");
        return;
    }
    Node *priorToKey = traverseToPriorToKey(*headadd, key);
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
        insertNode(headadd, priorToKey, createNode(d));
    }
}

void delete_at_head(Node **headadd)
{
    if (deleteNode(headadd, NULL))
    {
        printf("List is empty. Nothing to delete.\n");
    }
}

void delete_at_tail(Node **headadd)
{
    Node *priorToTail = traverseToPriorToTail(*headadd);
    if (priorToTail == NULL)
    {

        delete_at_head(headadd);
    }
    else
    {
        deleteNode(headadd, priorToTail);
    }
}

void delete_at_pos(Node **headadd, int pos)
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
    Node *precederNode = traverseToPos(*headadd, pos - 1);
    if (precederNode == NULL)
    {
        printf("List is empty. Nothing to delele!\n");
        return;
    }
    else if (precederNode->next == NULL)
    {
        printf("Invalid Position! Position is out of range.\n");
        return;
    }
    deleteNode(headadd, precederNode);
}

void delete_the_key(Node **headadd, int key)
{

    Node *priorToKey = traverseToPriorToKey(*headadd, key);
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
        deleteNode(headadd, priorToKey);
    }
}

void delete_all_key(Node **headadd, int key)
{
    Node *priorToKey = traverseToPriorToKey(*headadd, key);
    while (priorToKey == NULL)
    {
        delete_at_head(headadd);
        if (*headadd==NULL) return;
        priorToKey = traverseToPriorToKey(*headadd, key);
    }
    while (priorToKey->next!=NULL)
    {
        deleteNode(headadd, priorToKey);
        priorToKey=traverseToPriorToKey(priorToKey, key);
    }
}

void freeList(Node **headadd)
{
    Node *current = *headadd;
    Node *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    *headadd = NULL;
}

int main()
{
    Node *head = NULL;

    appendNode(&head, 10);
    appendNode(&head, 20);
    appendNode(&head, 30);
    appendNode(&head, 40);
    appendNode(&head, 50);
    printNode(head);

    insert_at_head(&head, 35);
    printNode(head);

    insert_at_head(&head, 90);
    printNode(head);

    insert_at_pos(&head, 70, 5);
    printNode(head);

    insert_at_pos(&head, 88, 8);
    printNode(head);

    insert_at_pos(&head, 65, 0);
    printNode(head);

    insert_at_pos(&head, 43, 15);
    printNode(head);

    insert_after_key(&head, 37, 10);
    printNode(head);

    insert_after_key(&head, 86, 37);
    printNode(head);

    insert_after_key(&head, 45, 43);
    printNode(head);

    insert_after_key(&head, 23, 89);
    printNode(head);

    insert_before_key(&head, 10, 20);
    printNode(head);

    insert_before_key(&head, 0, 56);
    printNode(head);

    insert_before_key(&head, 0, 65);
    printNode(head);

    delete_at_head(&head);
    printNode(head);

    delete_at_tail(&head);
    printNode(head);

    delete_at_pos(&head, 10);
    printNode(head);

    delete_at_pos(&head, 11);
    printNode(head);

    delete_at_pos(&head, -46);
    printNode(head);

    delete_the_key(&head, 86);
    printNode(head);

    delete_the_key(&head, 22);
    printNode(head);

    freeList(&head);

    Node *list = NULL;
    appendNode(&list, 10);
    appendNode(&list, 10);
    appendNode(&list, 10);
    appendNode(&list, 20);
    appendNode(&list, 30);
    appendNode(&list, 10);
    appendNode(&list, 10);
    appendNode(&list, 40);
    appendNode(&list, 50);
    appendNode(&list, 60);
    appendNode(&list, 10);
    appendNode(&list, 70);
    appendNode(&list, 80);
    appendNode(&list, 90);
    appendNode(&list, 10);
    appendNode(&list, 100);
    appendNode(&list, 110);
    appendNode(&list, 10);
    appendNode(&list, 120);
    appendNode(&list, 10);
    printNode(list);

    delete_all_key(&list, 10);
    printNode(list);

    freeList(&list);
    return 0;
}