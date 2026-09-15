#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rawMode.h"
#include "timer.h"

typedef struct node
{
    int data;
    struct node *next;
} Node;

typedef struct LNode
{
    struct LNode *prev;
    struct LNode *next;
} LNode;

typedef struct DNode
{
    LNode link;
    int data;
} DNode;

typedef struct linkedList
{
    LNode preHead;
    LNode postTail;
    int length;
} linkedList;

Node *createNode(int d);
void printNode(Node *head);

void insertNode(Node **headadd, Node *precederNode, Node *newNode);
int deleteNode(Node **headadd, Node *precNode);

Node *getTail(Node *head);
Node *getByPos(Node *head, int pos);
Node *getByKey(Node *head, int key);
Node *getPriorToTail(Node *head);
Node *getPriorToKey(Node *head, int key);

void appendNode(Node **headadd, int d);
void insert_at_head(Node **headadd, int d);
void insert_at_pos(Node **headadd, int d, int pos);
void insert_after_key(Node **headadd, int d, int key);
void insert_before_key(Node **headadd, int d, int key);

void delete_at_head(Node **headadd);
void delete_at_tail(Node **headadd);
void delete_at_pos(Node **headadd, int pos);
void delete_the_key(Node **headadd, int key);
void delete_all_key(Node **headadd, int key);

void freeList(Node **headadd);

DNode *createDNode(int d);

void initDList(linkedList *list);
linkedList *createDList();
void displayDList(linkedList *list);
void displayDListWithCursor(linkedList *list, LNode *cur);
void clearScreen();
void clearLine();

LNode *dgetByPos(linkedList *list, int pos);
LNode *dgetByKeyFromNode(linkedList *list, int key, LNode *start);
LNode *dgetByKey(linkedList *list, int key);
void insertDNode(linkedList *list, LNode *priorNode, LNode *inserteeNode);
void deleteDNode(linkedList *list, LNode *deadNode);

void appendDList(linkedList *list, int d);
void dinsertAtHead(linkedList *list, int d);
void dinsertAtPos(linkedList *list, int d, int pos);
void dinsertAfterKey(linkedList *list, int d, int key);
void dinsertBeforeKey(linkedList *list, int d, int key);

void ddeleteHead(linkedList *list);
void ddeleteTail(linkedList *list);
void ddeleteAtPos(linkedList *list, int pos);
void ddeleteTheKey(linkedList *list, int key);
void ddeleteAllKey(linkedList *list, int key);

linkedList *s2d(Node *head);

void clearList(linkedList *list);
void freeDList(linkedList **listadd);

int printemptylistmenu(linkedList *list);
int navigationmenu(linkedList *list);

int getInput(const char *);
void printlnreo();
void printleo();
void printreo();
void printo();
void invalidInput();

int main()
{
    linkedList *list;
    enableRawMode();
    list = createDList();
    appendDList(list, 10);
    // displayDList(list);
    navigationmenu(list);
    return 0;
}

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

Node *getTail(Node *head)
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

Node *getByPos(Node *head, int pos)
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

Node *getByKey(Node *head, int key)
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

Node *getPriorToTail(Node *head)
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

Node *getPriorToKey(Node *head, int key)
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
    Node *tail = getTail(*headadd);
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
        Node *precederNode = getByPos(*headadd, pos - 1);
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
        Node *keyNode = getByKey(*headadd, key);
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
    Node *priorToKey = getPriorToKey(*headadd, key);
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
    Node *priorToTail = getPriorToTail(*headadd);
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
    Node *precederNode = getByPos(*headadd, pos - 1);
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

    Node *priorToKey = getPriorToKey(*headadd, key);
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
    Node *priorToKey = getPriorToKey(*headadd, key);
    while (priorToKey == NULL)
    {
        delete_at_head(headadd);
        if (*headadd == NULL)
            return;
        priorToKey = getPriorToKey(*headadd, key);
    }
    while (priorToKey->next != NULL)
    {
        deleteNode(headadd, priorToKey);
        priorToKey = getPriorToKey(priorToKey, key);
    }
}

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
        return NULL;
    }
    if (pos >= list->length)
    {
        printf("Position out of range.\n");
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

void dinsertAtPos(linkedList *list, int d, int pos)
{
    if (pos == list->length)
    {
        appendDList(list, d);
        return;
    }
    LNode *posNode;
    if (posNode = dgetByPos(list, pos))
    {
        LNode *priorNode = posNode->prev;
        insertDNode(list, priorNode, &(createDNode(d)->link));
    }
}

void dinsertAfterKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    if (keyNode = dgetByKey(list, key))
    {
        insertDNode(list, keyNode, &(createDNode(d)->link));
    }
}

void dinsertBeforeKey(linkedList *list, int d, int key)
{
    LNode *keyNode;
    if (keyNode = dgetByKey(list, key))
    {
        LNode *priorNode = keyNode->prev;
        insertDNode(list, priorNode, &(createDNode(d)->link));
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

void ddeleteAtPos(linkedList *list, int pos)
{
    LNode *deadNode = dgetByPos(list, pos);
    if (deadNode)
        deleteDNode(list, deadNode);
}

void ddeleteTheKey(linkedList *list, int key)
{
    LNode *deadNode = dgetByKey(list, key);
    if (deadNode)
        deleteDNode(list, deadNode);
}

void ddeleteAllKey(linkedList *list, int key)
{
    LNode *cur;
    if (!(cur = dgetByKeyFromNode(list, key, list->preHead.next)))
    {
        printf("Key not found!\n");
        return;
    }
    LNode *deadNode;
    while (cur)
    {
        deadNode = cur;
        cur = dgetByKeyFromNode(list, key, cur->next);
        deleteDNode(list, deadNode);
    }
    printf("Deletion done.\n");
}

linkedList *s2d(Node *head)
{
    Node *current = head;
    linkedList *list = createDList();
    while (current != NULL)
    {
        appendDList(list, current->data);
        current = current->next;
    }
    return list;
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

void clearScreen()
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

void clearLine()
{
    printf("\033[A\033[2K");
    fflush(stdout);
}

void clearList(linkedList *list)
{
    if (!(list))
    {
        return;
    }
    LNode *dcurrent = (list)->preHead.next;
    LNode *next;
    while (dcurrent != &((list)->postTail))
    {
        next = dcurrent->next;
        free((DNode *)(dcurrent));
        dcurrent = next;
    }
}

void freeDList(linkedList **listadd)
{
    clearList(*listadd);
    free((*listadd));
    *listadd = NULL;
}

int getInput(const char *string)
{
    int input;
    disableRawMode();
    printf("%s", string);
    scanf("%d", &input);
    getchar();
    enableRawMode();
    return input;
}

// menu 0 = home menu (no list menu) {option c = menu 1, option q =return}
// menu 11=empty list menu {option a = menu 12, option f = menu 0, option q =return}
// menu 12=non empty list menu {option n = menu 121, option i = menu 122, option d = menu 123, option f = menu 0, option q = return}
// menu 121= navigation menu {option </>/i,d = menu 121, option b=menu 1}
// menu 31 = cur at left most menu
// menu 32 = cur at middle menu
// menu 33 = cur at right most menu
// menu 341 = menu 11
// menu 122= insertion menu {option h/t/p/ka/kb = menu 122, option b = menu 1}
// menu 123= deletion menu {option h/t/p/ko/ka = menu 123, option b = menu 1}

// void printMenu(int menu)
// {
//     switch (menu)
//     {
//     case 0:
//         printNoListMenu();
//         break;
//     case 11:

//     default:
//         break;
//     }
// }

// void printnolistmenu()
// {
//     printf("========================Linked List========================\n");
//     printf("No list to see :/\n");
//     printf("============================================================\n");
//     printf("c = create list  q = quit program\n");
// }

int printemptylistmenu(linkedList *list)
{
    clearScreen();
    displayDList(list);
    printf("============================================================\n");
    printf("a = add node  f = delete list   q = quit program\n");
    while (1)
    {
        readK();
        if (k == 'q')
        {
            return 0;
        }
        else if (k == 'f')
        {
            freeDList(&list);
            return 5;
        }
        else if (k == 'a')
        {
            appendDList(list, getInput("Enter Data:"));
            return 1;
        }
        else
        {
            invalidInput();
        }
    }
}

// int print12menu(linkedList *list)
// {
//     clearScreen();
//     displayDList(list);
//     printf("============================================================\n");
//     printf("n = navigate through the list  i = insert node  d = delete node\nf = delete list   q = quit program\n");
//     // more to come
// }

// int insertionmenu(linkedList *list)
// {
//     while (1)
//     {
//         clearScreen();
//         displayDList(list);
//         printf("============================================================\n");
//         printf("h = insert at head  t = insert at tail  p = insert at given position\nka = insert after given key  kb = insert before key  b = back to previous menu\nf = delete list   q = quit program\n");
//         while (1)
//         {
//             readK();
//             if (k == 'h')
//             {
//                 dinsertAtHead(list, getInput("Enter Data:"));
//                 break;
//             }
//             else if (k == 't')
//             {
//                 appendDList(list, getInput("Enter Data:"));
//                 break;
//             }
//             else if (k == 'p')
//             {
//                 dinsertAtPos(list, getInput("Enter Data:"), getInput("Enter the position:"));
//                 break;
//             }
//             else if (k == 'k')
//             {
//                 clearLine();
//                 clearLine();
//                 printf("============================================================\n");
//                 printf("ka = insert after given key  kb = insert before key\n");
//                 int r = timeoutC(5);
//                 if (r > 0)
//                 {
//                     readK();
//                     if (k == 'a')
//                     {
//                         dinsertAfterKey(list, getInput("Enter Data:"), getInput("Enter Key:"));
//                         break;
//                     }
//                     if (k == 'b')
//                     {
//                         dinsertAfterKey(list, getInput("Enter Data:"), getInput("Enter Key:"));
//                         break;
//                     }
//                 }
//                 else if (r == 0)
//                 {
//                     clearLine();
//                     clearLine();
//                     printf("============================================================\n");
//                     printf("h = insert at head  t = insert at tail  p = insert at given position\nka = insert after given key  kb = insert before key  b = back to previous menu\nq = quit program\n");
//                 }
//                 else
//                 {
//                     exit(1);
//                 }
//             }
//             else if (k == 'b')
//             {
//                 return 1;
//             }
//             else if (k == 'q')
//             {
//                 return 0;
//             }
//             else
//             {
//                 invalidInput();
//             }
//         }
//     }
// }

void invalidInput()
{
    printf("Invalid input!\n");
    fflush(stdout);
    timer(2);
    clearLine();
}

// int deletionmenu(linkedList *list)
// {
//     int menu;
//     while (1)
//     {
//         clearScreen();
//         displayDList(list);
//         printf("============================================================\n");
//         printf("h = delete head node  t = insert tail node  p = delete node at given position\nko = delete node matching the given key (first occurence)\nka = delete node matching the given key (all occurences)\na = delete all nodes  f = delete list\nb = back to previous menu  q = quit program\n");
//         while (1)
//         {
//             readK();
//             if (k == 'h')
//             {
//                 dinsertAtHead(list, getInput("Enter Data:"));
//                 if (list->length == 0)
//                 {
//                     menu = printemptylistmenu(list);
//                     if (!menu)
//                     {
//                         return 0;
//                     }
//                     else if (menu == 5)
//                     {
//                         return 5;
//                     }
//                 }
//                 break;
//             }
//             else if (k == 't')
//             {
//                 appendDList(list, getInput("Enter Data:"));
//                 if (list->length == 0)
//                 {
//                     menu = printemptylistmenu(list);
//                     if (!menu)
//                     {
//                         return 0;
//                     }
//                     else if (menu == 5)
//                     {
//                         return 5;
//                     }
//                 }
//                 break;
//             }
//             else if (k == 'p')
//             {
//                 dinsertAtPos(list, getInput("Enter Data:"), getInput("Enter the position:"));
//                 if (list->length == 0)
//                 {
//                     menu = printemptylistmenu(list);
//                     if (!menu)
//                     {
//                         return 0;
//                     }
//                     else if (menu == 5)
//                     {
//                         return 5;
//                     }
//                 }
//                 break;
//             }
//             else if (k == 'k')
//             {
//                 clearLine();
//                 clearLine();
//                 printf("============================================================\n");
//                 printf("ko = delete the first occurence  kb = delete all the occurences\n");
//                 int r = timeoutC(5);
//                 if (r > 0)
//                 {
//                     readK();
//                     if (k == 'a')
//                     {
//                         dinsertAfterKey(list, getInput("Enter Data:"), getInput("Enter Key:"));
//                         if (list->length == 0)
//                         {
//                             menu = printemptylistmenu(list);
//                             if (!menu)
//                             {
//                                 return 0;
//                             }
//                             else if (menu == 5)
//                             {
//                                 return 5;
//                             }
//                         }
//                         break;
//                     }
//                     else if (k == 'b')
//                     {
//                         dinsertBeforeKey(list, getInput("Enter Data:"), getInput("Enter Key:"));
//                         if (list->length == 0)
//                         {
//                             menu = printemptylistmenu(list);
//                             if (!menu)
//                             {
//                                 return 0;
//                             }
//                             else if (menu == 5)
//                             {
//                                 return 5;
//                             }
//                         }
//                         break;
//                     }
//                 }
//                 else if (r == 0)
//                 {
//                     clearLine();
//                     clearLine();
//                     printf("============================================================\n");
//                     printf("h = delete head node  t = insert tail node  p = delete node at given position\nko = delete node matching the given key (first occurence)\nka = delete node matching the given key (all occurences)\na = delete all nodes  f = delete list\nb = back to previous menu  q = quit program\n");
//                 }
//                 else
//                 {
//                     exit(1);
//                 }
//             }
//             else if (k == 'a')
//             {
//                 clearList(list);
//                 menu = printemptylistmenu(list);
//                 if (!menu)
//                 {
//                     return 0;
//                 }
//                 else if (menu == 5)
//                 {
//                     return 5;
//                 }
//                 break;
//             }
//             else if (k == 'f')
//             {
//                 freeDList(&list);
//                 return 5;
//             }
//             else if (k == 'b')
//             {
//                 return 1;
//             }
//             else if (k == 'q')
//             {
//                 return 0;
//             }
//             else
//             {
//                 invalidInput();
//             }
//         }
//     }
// }

int navigationmenu(linkedList *list)
{
    int menu;
    LNode *cur = list->preHead.next;
    while (1)
    {
        clearScreen();
        displayDListWithCursor(list, cur);
        if (cur->prev == &(list->preHead) && cur->next == &(list->postTail))
                {
                    printlnreo();
                }
                else if (cur->prev == &(list->preHead))
                {
                    printleo();
                }
                else if (cur->next == &(list->postTail))
                {
                    printreo();
                }
                else
                {
                    printo();
                }
        while (1)
        {
            readK();
            if ((!(cur->prev == &(list->preHead) && cur->next == &(list->postTail))) && (k == 300 || k == 400))
            {
                if (cur->prev == &(list->preHead))
                {
                    if (k == 300)
                    {
                        invalidInput();
                    }
                    else
                    {
                        cur = cur->next;
                        break;
                    }
                }
                else if (cur->next == &(list->postTail))
                {
                    if (k == 400)
                    {
                        invalidInput();
                    }
                    else
                    {
                        cur = cur->prev;
                        break;
                    }
                }
                else
                {
                    if (k == 300)
                    {
                        cur = cur->prev;
                        break;
                    }
                    else
                    {
                        cur = cur->next;
                        break;
                    }
                }
            }
            else if (k == 'i')
            {
                insertDNode(list, cur->prev, &(createDNode(getInput("Enter Data:"))->link));
                cur = cur->prev;
                break;
            }
            else if (k == 'd')
            {
                cur = cur->prev;
                deleteDNode(list, cur->next);
                if (cur->next != &(list->postTail))
                {
                    cur = cur->next;
                }
                if (list->length == 0)
                {
                    menu = printemptylistmenu(list);
                    if (!menu)
                    {
                        return 0;
                    }
                    else if (menu == 5)
                    {
                        return 5;
                    }
                    else if (menu == 1)
                    {
                        cur = list->preHead.next;
                        break;
                    }
                    
                }
                else
                {
                    break;
                }
            }
            else if (k == 'b')
            {
                return 1;
            }
            else if (k == 'q')
            {
                return 0;
            }
            else
            {
                invalidInput();
            }
        }
    }
}

void printleo()
{
    printf("============================================================\n");
    printf("\xe2\x86\x92 = move to right  i = insert here  d = delete this\nb = back to previous menu  q = quit program\n");
}

void printreo()
{
    printf("============================================================\n");
    printf("← = move to left  i = insert here  d = delete this\nb = back to previous menu  q = quit program\n");
}

void printo()
{
    printf("============================================================\n");
    printf("← = move to left  → = move to right  i = insert here  d = delete this\nb = back to previous menu  q = quit program\n");
}

void printlnreo()
{
    printf("============================================================\n");
    printf("i = insert here  d = delete this\nb = back to previous menu  q = quit program\n");
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
    if (dcurrent==&(list->postTail))
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