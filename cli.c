#include<stdio.h>
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
        printf("Invalid Input! Try again.\n");
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
    printf("Invalid input!\n");
    fflush(stdout);
    timer(2);
    clearLine();
}

int emptylistmenu(linkedList *list)
{
    clearScreen();
    displayDList(list);
    printf("============================================================\n");
    printf("a = add first node  f = delete list   q = quit program\n");
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
    printf("============================================================\n");
    printf("n = navigate through the list  i = insert node  d = delete node\nf = delete list   q = quit program\n");
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
        printf("============================================================\n");
        printf("h = insert at head  t = insert at tail  p = insert at given position\nka = insert after given key  kb = insert before key\nb = back to previous menu  q = quit program\n");
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
                printf("============================================================\n");
                printf("ka = insert after given key  kb = insert before key\n");
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
        clearScreen();
        displayDList(list);
        printf("============================================================\n");
        printf("h = delete head node  t = delete tail node  p = delete node at given position\nko = delete node matching the given key (first occurence)\nka = delete node matching the given key (all occurences)\na = delete all nodes  f = delete list\nb = back to previous menu  q = quit program\n");
        while (1)
        {
            readK();
            if (k == 'h')
            {
                ddeleteHead(list);
                if (list->length == 0)
                {
                    menu = emptylistmenu(list);
                    if (menu <= 0)
                    {
                        return menu;
                    }
                }
                break;
            }
            else if (k == 't')
            {
                ddeleteTail(list);
                if (list->length == 0)
                {
                    menu = emptylistmenu(list);
                    if (menu <= 0)
                    {
                        return menu;
                    }
                }
                break;
            }
            else if (k == 'p')
            {
                if (!(ddeleteAtPos(list, getInput("Enter the position:"))))
                {
                    timer(15);
                }
                if (list->length == 0)
                {
                    menu = emptylistmenu(list);
                    if (menu <= 0)
                    {
                        return menu;
                    }
                }
                break;
            }
            else if (k == 'k')
            {
                for (int i = 0; i < 6; i++)
                {
                    clearLine();
                }
                printf("============================================================\n");
                printf("ko = delete the first occurence  ka = delete all the occurences\n");
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
                        if (list->length == 0)
                        {
                            menu = emptylistmenu(list);
                            if (menu <= 0)
                            {
                                return menu;
                            }
                        }
                        break;
                    }
                    else if (k == 'a')
                    {
                        if (!(ddeleteAllKey(list, getInput("Enter Key:"))))
                        {
                            timer(15);
                        }
                        if (list->length == 0)
                        {
                            menu = emptylistmenu(list);
                            if (menu <= 0)
                            {
                                return menu;
                            }
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
                menu = emptylistmenu(list);
                if (menu <= 0)
                {
                    return menu;
                }
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
        if (cur->prev == &(list->preHead) && cur->next == &(list->postTail))
        {
            printlnreo();
        }
        else if (cur->prev == &(list->preHead))
        {
            printleo();
        }
        else if (cur == &(list->postTail))
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
            if ((!(cur->prev == &(list->preHead) && cur == &(list->postTail))) && (k == 300 || k == 400))
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
                else if (cur == &(list->postTail))
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

void printleo()
{
    printf("============================================================\n");
    printf("\xe2\x86\x92 = move to right  i = insert here  d = delete this\nb = back to previous menu  q = quit program\n");
}

void printreo()
{
    printf("============================================================\n");
    printf("← = move to left  i = insert here\nb = back to previous menu  q = quit program\n");
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