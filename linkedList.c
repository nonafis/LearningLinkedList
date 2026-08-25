#include <stdio.h>
#include <stdlib.h>

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

void append(Node **headadd, int d)
{
    Node *newNode = createNode(d);
    if (*headadd == NULL)
    {
        *headadd = newNode;
    }
    else
    {
        Node *current = *headadd;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
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

void insertNode(Node **headadd, Node *N)
{
    if (*headadd == NULL)
    {
        *headadd = N;
    }
    else
    {
        Node *current = *headadd;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = N;
    }
}

void appendNode(Node **headadd, int d)
{
    Node *newNode = createNode(d);
    insertNode(headadd, newNode);
}

void insertAtHead(Node **headadd, int d)
{
    Node *newNode = createNode(d);
    insertNode(&newNode, *headadd);
    *headadd = newNode;
}

void insert_at_head(Node **headadd, int d)
{
    Node *newNode = createNode(d);
    newNode->next = *headadd;
    *headadd = newNode;
}

void insert_at_pos(Node **headadd, int d, int pos)
{
    Node *newNode = createNode(d);
    if (*headadd == NULL)
    {
        *headadd = newNode;
    }
    else
    {
        Node *current = *headadd;
        Node *nextToCurrent = *headadd;
        if (pos == 0)
        {
            newNode->next = nextToCurrent;
            *headadd = newNode;
        }
        else
        {
            for (int i = 0; (nextToCurrent != NULL) && (i < pos); i++)
            {
                nextToCurrent = nextToCurrent->next;
                if (i != 0)
                {
                    current = current->next;
                }
            }
            newNode->next = nextToCurrent;
            current->next = newNode;
        }
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

    append(&head, 10);
    append(&head, 20);
    append(&head, 30);
    append(&head, 40);
    append(&head, 50);
    printNode(head);

    insert_at_pos(&head, 70, 3);
    printNode(head);
    
    insert_at_pos(&head, 88, 7);
    printNode(head);

    insert_at_pos(&head, 65, 0);
    printNode(head);

    insert_at_pos(&head, 43, 8);
    printNode(head);
    
    freeList(&head);
    return 0;
}