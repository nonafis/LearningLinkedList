#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

void freeDList(linkedList **listadd);

int main()
{
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
    // printNode(list);

    linkedList *LL = createDList();
    LL = s2d(list);
    displayDList(LL);
    ddeleteAllKey(LL, 10);
    displayDList(LL);

    // linkedList *list = createDList();
    // dinsertAtHead(list, 33);
    // dinsertAtHead(list, 25);
    // appendDList(list, 10);
    // appendDList(list, 20);
    // appendDList(list, 30);
    // dinsertAtPos(list, 40, 5);
    // appendDList(list, 50);
    // dinsertAfterKey(list, 60, 50);
    // appendDList(list, 70);
    // dinsertBeforeKey(list, 65, 70);
    // displayDList(list);

    // freeDList(&list);

    // Node *head = NULL;

    // appendNode(&head, 10);
    // appendNode(&head, 20);
    // appendNode(&head, 30);
    // appendNode(&head, 40);
    // appendNode(&head, 50);
    // printNode(head);

    // insert_at_head(&head, 35);
    // printNode(head);

    // insert_at_head(&head, 90);
    // printNode(head);

    // insert_at_pos(&head, 70, 5);
    // printNode(head);

    // insert_at_pos(&head, 88, 8);
    // printNode(head);

    // insert_at_pos(&head, 65, 0);
    // printNode(head);

    // insert_at_pos(&head, 43, 15);
    // printNode(head);

    // insert_after_key(&head, 37, 10);
    // printNode(head);

    // insert_after_key(&head, 86, 37);
    // printNode(head);

    // insert_after_key(&head, 45, 43);
    // printNode(head);

    // insert_after_key(&head, 23, 89);
    // printNode(head);

    // insert_before_key(&head, 10, 20);
    // printNode(head);

    // insert_before_key(&head, 0, 56);
    // printNode(head);

    // insert_before_key(&head, 0, 65);
    // printNode(head);

    // delete_at_head(&head);
    // printNode(head);

    // delete_at_tail(&head);
    // printNode(head);

    // delete_at_pos(&head, 10);
    // printNode(head);

    // delete_at_pos(&head, 11);
    // printNode(head);

    // delete_at_pos(&head, -46);
    // printNode(head);

    // delete_the_key(&head, 86);
    // printNode(head);

    // delete_the_key(&head, 22);
    // printNode(head);

    // freeList(&head);

    // delete_all_key(&list, 10);
    // printNode(list);

    // freeList(&list);
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
    LNode *dcurrent = list->preHead.next;
    while ((dcurrent != &(list->postTail)))
    {
        printf("%d->", ((DNode *)(dcurrent))->data);
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

void freeDList(linkedList **listadd)
{
    if (!(*listadd))
    {
        return;
    }
    LNode *dcurrent = (*listadd)->preHead.next;
    LNode *next;
    while (dcurrent != &((*listadd)->postTail))
    {
        next = dcurrent->next;
        free((DNode *)(dcurrent));
        dcurrent = next;
    }
    free((*listadd));
    *listadd = NULL;
}