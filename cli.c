#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"
#include "rawMode.h"
#include "timer.h"
#include "terminalcontrol.h"
#include "cli.h"

int getInput(const char *string)
{
    int c;
    int input;
    disableRawMode();
    printf("%s", string);
    while (scanf("%d", &input) != 1)
    {
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        printf("Invalid Input! Try again.\r\n");
        fflush(stdout);
        timer(10);
        clearLine();
        clearLine();
        printf("%s", string);
    }
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    enableRawMode();
    return input;
}

void invalidInput()
{
    printf("Invalid input!\r\n");
    fflush(stdout);
    timer(2);
    clearLine();
}


void displayDList(linkedList *list)
{
    printf("========================Linked List========================\r\n");
    LNode *dcurrent = list->preHead.next;
    while ((dcurrent != &(list->postTail)))
    {
        printf("%d<->", ((DNode *)(dcurrent))->data);
        dcurrent = dcurrent->next;
    }
    printf("NULL\r\n");
}

void displayDListWithCursor(linkedList *list, LNode *cur)
{
    int cursoroffset = 0;
    printf("========================Linked List========================\r\n");
    LNode *dcurrent = list->preHead.next;
    while (dcurrent != &(list->postTail) && (dcurrent != cur))
    {
        cursoroffset += printf("%d<->", ((DNode *)(dcurrent))->data);
        dcurrent = dcurrent->next;
    }
    if (!(dcurrent == cur))
    {
        printf("NULL\r\ntf you had given as cur parameter huh! -_-\r\n");
        return;
    }
    while (dcurrent != &(list->postTail))
    {
        printf("%d<->", ((DNode *)(dcurrent))->data);
        dcurrent = dcurrent->next;
    }
    printf("NULL\r\n");
    for (int i = 0; i < cursoroffset; i++)
    {
        printf(" ");
    }
    printf("^\r\n");
}

int emptylistmenu(linkedList *list)
{
    clearScreen();
    displayDList(list);
    printf("============================================================\r\n");
    printf("a = add first node  f = delete list   q = quit program\r\n");
    while (1)
    {
        readK();
        if (k == 'q')
        {
            return 0;
        }
        else if (k == 'f')
        {
            return -5;
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

int nonemptylistmenu(linkedList *list)
{
    clearScreen();
    displayDList(list);
    printf("============================================================\r\n");
    printf("n = navigate through the list  i = insert node  d = delete node\r\nf = delete list   q = quit program\r\n");
    while (1)
    {
        readK();
        if (k == 'n')
        {
            return navigationmenu(list);
        }
        else if (k == 'i')
        {
            return insertionmenu(list);
        }
        else if (k == 'd')
        {
            return deletionmenu(list);
        }
        else if (k == 'f')
        {
            return -5;
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

int insertionmenu(linkedList *list)
{
    while (1)
    {
        clearScreen();
        displayDList(list);
        printf("============================================================\r\n");
        printf("h = insert at head  t = insert at tail  p = insert at given position\r\nka = insert after given key  kb = insert before key\r\nb = back to previous menu  q = quit program\r\n");
        while (1)
        {
            readK();
            if (k == 'h')
            {
                dinsertAtHead(list, getInput("Enter Data:"));
                break;
            }
            else if (k == 't')
            {
                appendDList(list, getInput("Enter Data:"));
                break;
            }
            else if (k == 'p')
            {
                if (!(dinsertAtPos(list, getInput("Enter Data:"), getInput("Enter the position:"))))
                {
                    timer(15);
                }
                break;
            }
            else if (k == 'k')
            {
                for (int i = 0; i < 4; i++)
                {
                    clearLine();
                }
                printf("============================================================\r\n");
                printf("ka = insert after given key  kb = insert before key\r\n");
                int r = timeoutC(5);
                if (r > 0)
                {
                    readK();
                    if (k == 'a')
                    {
                        if (!(dinsertAfterKey(list, getInput("Enter Data:"), getInput("Enter Key:"))))
                        {
                            timer(15);
                        }
                        break;
                    }
                    else if (k == 'b')
                    {
                        if (!(dinsertBeforeKey(list, getInput("Enter Data:"), getInput("Enter Key:"))))
                        {
                            timer(15);
                        }
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (r == 0)
                {
                    break;
                }
                else
                {
                    exit(1);
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

int deletionmenu(linkedList *list)
{
    int menu;
    while (1)
    {
        if (list->length == 0)
        {
            menu = emptylistmenu(list);
            if (menu <= 0)
            {
                return menu;
            }
        }
        clearScreen();
        displayDList(list);
        printf("============================================================\r\n");
        printf("h = delete head node  t = delete tail node  p = delete node at given position\r\nko = delete node matching the given key (first occurence)\r\nka = delete node matching the given key (all occurences)\r\na = delete all nodes  f = delete list\r\nb = back to previous menu  q = quit program\r\n");
        while (1)
        {
            readK();
            if (k == 'h')
            {
                ddeleteHead(list);
                break;
            }
            else if (k == 't')
            {
                ddeleteTail(list);
                break;
            }
            else if (k == 'p')
            {
                if (!(ddeleteAtPos(list, getInput("Enter the position:"))))
                {
                    timer(15);
                }
                break;
            }
            else if (k == 'k')
            {
                for (int i = 0; i < 6; i++)
                {
                    clearLine();
                }
                printf("============================================================\r\n");
                printf("ko = delete the first occurence  ka = delete all the occurences\r\n");
                int r = timeoutC(5);
                if (r > 0)
                {
                    readK();
                    if (k == 'o')
                    {
                        if (!(ddeleteTheKey(list, getInput("Enter Key:"))))
                        {
                            timer(15);
                        }
                        break;
                    }
                    else if (k == 'a')
                    {
                        if (!(ddeleteAllKey(list, getInput("Enter Key:"))))
                        {
                            timer(15);
                        }
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (r == 0)
                {
                    break;
                }
                else
                {
                    exit(1);
                }
            }
            else if (k == 'a')
            {
                clearList(list);
                break;
            }
            else if (k == 'f')
            {
                return -5;
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

int navigationmenu(linkedList *list)
{
    int menu;
    LNode *cur = list->preHead.next;
    while (1)
    {
        clearScreen();
        displayDListWithCursor(list, cur);
        printnavkeyhints(cur->prev == &(list->preHead), cur == &(list->postTail), "move to left", "move to right", "insert here", "delete this");
        while (1)
        {
            readK();
            if ((!(cur->prev == &(list->preHead) && cur == &(list->postTail))) && (k == LEFT_ARROW_KEY || k == RIGHT_ARROW_KEY))
            {
                if (cur->prev == &(list->preHead))
                {
                    if (k == LEFT_ARROW_KEY)
                    {
                        invalidInput();
                    }
                    else
                    {
                        cur = cur->next;
                        break;
                    }
                }
                else if (cur == &(list->postTail))
                {
                    if (k == RIGHT_ARROW_KEY)
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
                    if (k == LEFT_ARROW_KEY)
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
            else if ((cur != &(list->postTail)) && (k == 'd'))
            {
                cur = cur->prev;
                deleteDNode(list, cur->next);
                if (cur->next != &(list->postTail))
                {
                    cur = cur->next;
                }
                if (list->length == 0)
                {
                    menu = emptylistmenu(list);
                    if (menu <= 0)
                    {
                        return menu;
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

void printnavkeyhints(int leftedgeboolean, int rightedgeboolean, const char *leftarrowhintmessage, const char *rightarrowhintmessage, const char *ihintmessage, const char *dhintmessage)
{
    printf("============================================================\r\n");
    if ((leftedgeboolean) && (rightedgeboolean))  // when it is an empty list. navmenu cant reach this  
    {
        printf("i = %s  d = %s  b = back to previous menu  q = quit program\r\n", ihintmessage, dhintmessage);
    }
    else if (leftedgeboolean)
    {
        printf("→ = %s  i = %s  d = %s\r\nb = back to previous menu  q = quit program\r\n", rightarrowhintmessage, ihintmessage, dhintmessage);
    }
    else if (rightedgeboolean)
    {
        printf("← = %s  i = %s\r\nb = back to previous menu  q = quit program\r\n", leftarrowhintmessage, ihintmessage);
    }
    else
    {
        printf("← = %s  → = %s  i = %s  d = %s\r\nb = back to previous menu  q = quit program\r\n", leftarrowhintmessage, rightarrowhintmessage, ihintmessage, dhintmessage);
    }
}

// int searchmenu(linkedList *list)
// {
//     linkedList *searchstoredlist = NULL;
//     LNode *matchedNode, *dcurrent, *sllcur;
//     int key, cursoroffset = 0, lastcurrelpos = 0;
//     key = getInput("Search:");
//     while (1)
//     {
//         matchedNode = dgetByKeyFromNode(list, key, list->preHead.next);
//         if (!(matchedNode))
//         {
//             printf("No matching node.\r\n");
//             timer(15);
//             return 1;
//         }
//         freeDList(&searchstoredlist);
//         searchstoredlist = createList();
//         cursoroffset = 0;
//         dcurrent = list->preHead.next;
//         clearScreen();
//         printf("========================Linked List========================\r\n");
//         while (matchedNode != NULL)
//         {
//             while (dcurrent != matchedNode)
//             {
//                 cursoroffset += printf("%d<->", ((DNode *)(dcurrent))->data);
//                 dcurrent = dcurrent->next;
//             }
//             insertDNode(searchstoredlist, searchstoredlist->postTail.prev, &(createMNode(cursoroffset, matchedNode))->link);
//             cursoroffset += printf("[%d]<->", ((DNode *)(dcurrent))->data);
//             dcurrent = dcurrent->next;
//             matchedNode = dgetByKeyFromNode(list, key, matchedNode->next);
//         }
//         while (dcurrent != &(list->postTail))
//         {
//             printf("%d<->", ((DNode *)(dcurrent))->data);
//             dcurrent = dcurrent->next;
//         }
//         printf("NULL\r\n");
//         sllcur = searchstoredlist->preHead.next;
//         for (int i = 0; (i < lastcurrelpos); i++)
//         {
//             sllcur = sllcur->next;
//         }
//         while (1)
//         {
//             for (int i = 0; i < (((MNode *)(sllcur))->intData) + 1; i++) //+1 for "["
//             {
//                 printf(" ");
//             }
//             printf("^\r\n");
//             printnavkeyhints(((sllcur->prev) == &(searchstoredlist)->preHead), ((sllcur->next) == &(searchstoredlist)->postTail), "move to previous matching node", "move to next matching node", "insert around here", "delete this");
//             readK();
//             if (((sllcur->next) != &(searchstoredlist)->postTail) && (k == RIGHT_ARROW_KEY))
//             {
//                 sllcur = sllcur->next;
//                 ++lastcurrelpos;
//             }
//             else if (((sllcur->prev) != &(searchstoredlist)->preHead) && (k == LEFT_ARROW_KEY))
//             {
//                 sllcur = sllcur->prev;
//                 --lastcurrelpos;
//             }
//             else if (k == 'i')
//             {
//                 for (int i = 0; i < 4 + 1; i++)
//                 {
//                     clearLine();
//                 }
//                 for (int i = 0; i < (((MNode *)(sllcur))->intData) + 1; i++) //+1 for "["
//                 {
//                     printf(" ");
//                 }
//                 printf("[%d]\r\n", ((DNode *)(((MNode *)(sllcur))->addressData))->data);
//                 printf("b = insert before this matching node\r\n");
//                 printf("a = insert after this matching node\r\n");
//                 printf("c = cancel insertion\r\n");
//                 while (1)
//                 {
//                     readK();
//                     if (k == 'c')
//                     {
//                         break;
//                     }
//                     else if (k == 'b')
//                     {
//                         clearScreen();
//                         insertDNode(list, ((MNode *)(sllcur))->addressData->prev, createDNode(getInput("Enter Data:")));
//                         if (((DNode *)(((MNode *)(sllcur))->addressData->prev))->data == key)
//                         {
//                             ++lastcurrelpos;
//                         }
//                         break;
//                     }
//                     else if (k == 'a')
//                     {
//                         clearScreen();
//                         insertDNode(list, ((MNode *)(sllcur))->addressData, createDNode(getInput("Enter Data:")));
//                         break;
//                     }
//                     else
//                     {
//                         invalidInput();
//                     }
//                 }
//                 break;
//             }
//             else if (k == 'd')
//             {
//                 deleteDNode(list, ((MNode *)(sllcur))->addressData);
//                 if (lastcurrelpos > 0)
//                 {
//                     --lastcurrelpos;
//                 }
//                 break;
//             }
//             else
//             {
//                 invalidInput();
//             }
//             for (int i = 0; i < 4; i++)
//             {
//                 clearLine();
//             }
//         }
//     }
// }