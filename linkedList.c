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

void spliceNode(Node **headadd, Node *N)
{
    if (N->next == NULL)
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
    else
    {
        printf("Invalid argument! it is not a single node.\n");
    }
}

void appendNode(Node **headadd, int d)
{
    Node *newNode = createNode(d);
    spliceNode(headadd, newNode);
}

void insert_at_head(Node **headadd, int d)
{
    Node *newNode = createNode(d);
    newNode->next = *headadd;
    *headadd = newNode;
}

void insert_at_pos(Node **headadd, int d, int pos)
{
    if (pos < 0)
    {
        printf("Invalid position! Position can't be negative.\n");
        return;
    }
    Node *newNode = createNode(d);
    if (*headadd == NULL || pos == 0)
    {
        newNode->next = *headadd;
        *headadd = newNode;
    }
    else
    {
        Node *current = *headadd;
        for (int i = 1; (current->next != NULL) && (i < pos); i++)
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void insert_after_key(Node **headadd, int d, int key)
{
    if (*headadd == NULL)
    {
        printf("Key not found!\n");
    }
    else
    {
        Node *current = *headadd;
        while ((current->next != NULL) && ((current->data) != key))
        {
            current = current->next;
        }
        if ((current->next == NULL) && (current->data != key))
        {
            printf("Key not found!\n");
        }
        else
        {
            Node *newNode = createNode(d);
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

void delete_at_head(Node **headadd)
{
    if (*headadd == NULL)
    {
        printf("List is empty. Nothing to delete.\n");
    }
    else
    {
        Node *deadnode = *headadd;
        *headadd = (*headadd)->next;
        free(deadnode);
    }
}

void delete_at_tail(Node **headadd)
{
    if (*headadd == NULL)
    {
        printf("List is Empty. Nothing to delete.\n");
    }
    else
    {
        if ((*headadd)->next == NULL) // if it is a single node
        {
            free(*headadd);
            *headadd = NULL;
        }
        else
        {
            Node *current = *headadd;
            while (current->next->next != NULL)
            {
                current = current->next;
            }
            free(current->next);
            current->next = NULL;
        }
    }
}

void delete_at_pos(Node **headadd, int pos)
{
    if (*headadd == NULL)
    {
        printf("List is empty. Nothing to delete.\n");
    }
    else
    {
        if (pos < 0)
        {
            printf("Invalid position! Position can't be negative.\n");
        }
        else if (pos == 0)
        {
            delete_at_head(headadd);
        }
        else
        {
            if ((*headadd)->next == NULL)
            {
                printf("Invalid Position! Length of the list is only 1 and last valid position is 0\n");
            }
            else
            {
                Node *current = *headadd;
                int i;
                for (i = 1; (current->next->next) != NULL && i < pos; i++)
                {
                    current = current->next;
                }
                if ((current->next->next == NULL) && (i != pos))
                {
                    printf("Invalid Position! Length of the list is only %d and last valid position is %d\n", i + 1, i);
                }
                else
                {
                    Node *deadNode = current->next;
                    current->next = current->next->next;
                    free(deadNode);
                }
            }
        }
    }
}

void delete_the_key(Node **headadd, int key)
{
    if (*headadd == NULL)
    {
        printf("List is empty. Nothing to delete.\n");
    }
    else
    {
        if ((*headadd)->data == key)
        {
            delete_at_head(headadd);
        }
        else
        {
            if ((*headadd)->next == NULL)
            {
                printf("Key not found!\n");
            }
            else
            {
                Node *current = *headadd;
                while ((current->next->next != NULL) && (current->next->data != key))
                {
                    current = current->next;
                }
                if ((current->next->next == NULL) && (current->next->data != key))
                {
                    printf("Key not found!\n");
                }
                else
                {
                    Node *deadNode = current->next;
                    current->next = current->next->next;
                    free(deadNode);
                }
            }
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

    freeList(&head);
    return 0;
}