#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rawMode.h"
#include "terminalcontrol.h"
#include "timer.h"
#include "dllist.h"
#include "cli.h"

int main()
{
    linkedList *list;
    int menu = -5;
    enableRawMode();
    while (menu)
    {
        clearScreen();
        printf("========================Linked List========================\n");
        printf("No list to see :/\n");
        printf("============================================================\n");
        printf("c = create list  q = quit program\n");
        readK();
        if (k == 'c')
        {
            list = createDList();
            menu = 1;
            while (menu == 1)
            {
                if (list->length == 0)
                {
                    menu = emptylistmenu(list);
                }
                else
                {
                    menu = nonemptylistmenu(list);
                }
            }
            if (menu == -5)
            {
                freeDList(&list);
            }
        }
        else if (k == 'q')
        {
            menu = 0;
        }
        else
        {
            invalidInput();
        }
    }

    return 0;
}