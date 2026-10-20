#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "dllist.h"
#include "rawMode.h"
#include "terminalconfig.h"
#include "terminalcontrol.h"
#include "timer.h"
#include "cli.h"
#define TITLETEXT "Linked List"
#define MAX_DEPTH 5

static menu menuHistory[MAX_DEPTH];
static int currentDepth = 0;

static opStatus push(menu inMenu)
{
    if (currentDepth >= MAX_DEPTH)
    {
        return OP_ERROR_STACK_OVERFLOW;
    }
    menuHistory[currentDepth++] = inMenu;
    return OP_SUCCESS;
}

static opStatus pop(menu *outMenu)
{
    if (currentDepth <= 0)
    {
        return OP_ERROR_STACK_UNDERFLOW;
    }
    --currentDepth;
    if (outMenu)
    {
        *outMenu = menuHistory[currentDepth];
    }
    return OP_SUCCESS;
}
//--------------------------------------------------------------------------------

//----------------------------------declarations----------------------------------
typedef struct state
{
    linkedList *list;
    menu currentMenu;
    int skipchar;
    int nodesInserted;
    LNode *insertedWhere;
    int terminalWidth;
    bool quit;
    LNode *cursoredNode;
    linkedList *HNList;
} state;

typedef struct button
{
    const char *character;
    const char *label;
    void (*action)(state *Stateadd);
    menu nextMenu;
    bool (*buttonConditon)(state State);
} button;

static int printTitle(state State);
static int printSepLine(state State);
static int reportOpError(opStatus s, state State);
static int printButtons(state State);
static int printList(state State);
static void quitAction(state *Stateadd);
static void createListAction(state *Stateadd);
static void addFirstNodeAction(state *Stateadd);
static void deleteListAction(state *Stateadd);
static void headInsAction(state *Stateadd);
static void tailInsAction(state *Stateadd);
static void posInsAction(state *Stateadd);
static void prefixAction(state *Stateadd);
static void afterKeyInsAction(state *Stateadd);
static void beforeKeyInsAction(state *Stateadd);
static void headDelAction(state *Stateadd);
static void tailDelAction(state *Stateadd);
static void posDelAction(state *Stateadd);
static void oneKeyDelAction(state *Stateadd);
static void allKeyDelAction(state *Stateadd);
static void delAllNodeAction(state *Stateadd);
static void moveCursorLeftAction(state *Stateadd);
static void moveCursorRightAction(state *Stateadd);
static void navInsertAction(state *Stateadd);
static void navDeleteAction(state *Stateadd);
static void buildHNListAction(state *Stateadd);
static void insertBeforeAction(state *Stateadd);
static void insertAfterAction(state *Stateadd);
static void searchDelBeforeAction(state *Stateadd);
static void searchDelItselfAction(state *Stateadd);
static void searchDelAfterAction(state *Stateadd);
static void massHeadInsAction(state *Stateadd);
static void massTailInsAction(state *Stateadd);
static void massPosInsAction(state *Stateadd);
static void reverseListAction(state *Stateadd);
static void sortListAction(state *Stateadd);

static bool notOnLeftEdgeCondition(state State);
static bool notOnRightEdgeCondition(state State);
static bool thereIsANodeToLeft(state State);
static bool thereIsANodeToRight(state State);
static bool isNotSingleNodeList(state State);
//--------------------------------------------------------------------------------

//-----------------------------------i/o func-------------------------------------
static opStatus getInput(const char *string, int *intadd)
{
    int c;
    char inputBuffer[22];
    disableRawMode();
    printf("\0337");
    printf("\033[?25h");
    while (1)
    {
        printf("%s", string);
        fflush(stdout);
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL)
        {
            clearerr(stdin);
            printf("\033[?25l");
            enableRawMode();
            return OP_ERROR;
        }
        int tooLong = 0;
        if (strchr(inputBuffer, '\n') == NULL)
        {
            tooLong = 1;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            if (c == EOF)
            {
                clearerr(stdin);
            }
        }
        errno = 0;
        char *firstNonInteger = inputBuffer;
        long input = strtol(inputBuffer, &firstNonInteger, 10);
        if (tooLong || firstNonInteger == inputBuffer || (*firstNonInteger != '\n' && *firstNonInteger != '\0') || (errno == ERANGE || input < INT_MIN || input > INT_MAX))
        {
            printf("Invalid Input! Try again.\n");
            fflush(stdout);
            timer(10);
            printf("\0338\x1b[J");
            // fflush(stdout);
        }
        else
        {
            *intadd = (int)input;
            printf("\033[?25l");
            enableRawMode();
            return OP_SUCCESS;
        }
    }
}

static void invalidInputAction(state *Stateadd)
{
    (void)Stateadd;
    printf("Invalid input!\n");
    fflush(stdout);
    timeoutC(5);
    clearLine();
}

static int reportOpError(opStatus s, state State)
{
    if (s == OP_SUCCESS)
        return 0;
    clearScreen();
    switch (s)
    {
    case OP_ALLOC_FAILED:
        printf("Allocation Failed.\n");
        break;
    case OP_ERROR_INVALID_ARG:
        printf("Invalid argument passed to the function.\n");
        break;
    case OP_ERROR_INVALID_INPUT:
        printf("Invalid Input.\n");
        break;
    case OP_ERROR_NEGATIVE_POSITION:
        printf("Position can't be negative!\n");
        break;
    case OP_ERROR_OUT_OF_RANGE_POSITION:
        printf("Given position is out of range. Length of List: %d\n", State.list->length);
        break;
    case OP_ERROR_KEY_NOT_FOUND:
        printf("Key not found.\n");
        break;
    case OP_ERROR_EMPTY_LIST:
        printf("This operation cannot be done on an empty list.\n");
        break;
    case OP_ERROR_STACK_OVERFLOW:
        printf("Stack overflowed.\n");
        break;
    case OP_ERROR_STACK_UNDERFLOW:
        printf("Stack underflowed.\n");
        break;
    case OP_ERROR:
        printf("Something went wrong :/\n");
        break;
    default:
        printf("Something went wrong :|\n");
    }
    fflush(stdout);
    timer(15);
    return 1;
}

//--------------------------------------------------------------------------------

//------------------------------------strwidth------------------------------------
static int strwidth(const char *str) // only ascii and four specific characters (←, →, ↑, ↓) are handled
{
    int width = 0;

    for (int i = 0; str[i] != '\0';)
    {
        if ((unsigned char)str[i] == 0xE2 &&
            (unsigned char)str[i + 1] == 0x86 && ((unsigned char)str[i + 2] == 0x90 || (unsigned char)str[i + 2] == 0x91 || (unsigned char)str[i + 2] == 0x92 || (unsigned char)str[i + 2] == 0x93))
        {
            i += 3;
            width++;
        }
        else
        {
            if ((unsigned char)str[i] >= 0x20 && (unsigned char)str[i] != 0x7F)
            {
                width++;
            }
            i++;
        }
    }

    return width;
}

//--------------------------------------------------------------------------------

//======================================================================================================

static button mainMenuButtons[] = {
    {.character = "c", .label = "create list", .action = createListAction, .nextMenu = IS_LIST_MENU},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};
static button emptyListMenuButtons[] = {
    {.character = "a", .label = "add first node", .action = addFirstNodeAction, .nextMenu = BACK},
    {.character = "f", .label = "delete list", .action = deleteListAction},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};
static button isListMenuButtons[] = {
    {.character = "n", .label = "navigation menu", .nextMenu = NAVIGATION_MENU},
    {.character = "i", .label = "insertion menu", .nextMenu = INSERTION_MENU},
    {.character = "d", .label = "deletion menu", .nextMenu = DELETION_MENU},
    {.character = "s", .label = "search menu", .action = buildHNListAction, .nextMenu = SEARCH_MENU},
    {.character = "m", .label = "mass insertion menu", .nextMenu = MASS_INSERTION_MENU},
    {.character = "r", .label = "reverse the list", .action = reverseListAction, .buttonConditon = isNotSingleNodeList},
    {.character = "o", .label = "sort the list", .action = sortListAction, .buttonConditon = isNotSingleNodeList},
    {.character = "f", .label = "delete list", .action = deleteListAction},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};
static button keyInsertSubmenuButtons[] = {
    {.character = "ka", .label = "insert after key", .action = afterKeyInsAction},
    {.character = "kb", .label = "insert before key", .action = beforeKeyInsAction},
    {.character = NULL, .label = NULL}};
static button insertionMenuButtons[] = {
    {.character = "h", .label = "insert at head", .action = headInsAction},
    {.character = "t", .label = "insert at tail", .action = tailInsAction},
    {.character = "p", .label = "insert at pos", .action = posInsAction},
    {.character = "ka", .label = "insert after key", .action = prefixAction, .nextMenu = KEY_INSERT_SUBMENU},
    {.character = "kb", .label = "insert before key", .action = prefixAction, .nextMenu = KEY_INSERT_SUBMENU},
    {.character = "b", .label = "back to previous menu", .nextMenu = BACK},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};
static button keyDeleteSubmenuButtons[] = {
    {.character = "ko", .label = "delete the first node matching the given key", .action = oneKeyDelAction},
    {.character = "ka", .label = "delete all nodes matching the given key", .action = allKeyDelAction},
    {.character = NULL, .label = NULL}};
static button deletionMenuButtons[] = {
    {.character = "h", .label = "delete head node", .action = headDelAction},
    {.character = "t", .label = "delete tail node", .action = tailDelAction},
    {.character = "p", .label = "delete at pos", .action = posDelAction},
    {.character = "ko", .label = "delete the first node matching the given key", .action = prefixAction, .nextMenu = KEY_DELETE_SUBMENU},
    {.character = "ka", .label = "delete all nodes matching the given key", .action = prefixAction, .nextMenu = KEY_DELETE_SUBMENU},
    {.character = "a", .label = "delete all nodes", .action = delAllNodeAction},
    {.character = "f", .label = "delete list", .action = deleteListAction},
    {.character = "b", .label = "back to previous menu", .nextMenu = BACK},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};
static button navigationMenuButtons[] = {
    {.character = LEFT_ARROW_CHAR, .label = "move to left", .action = moveCursorLeftAction, .buttonConditon = notOnLeftEdgeCondition},
    {.character = RIGHT_ARROW_CHAR, .label = "move to right", .action = moveCursorRightAction, .buttonConditon = notOnRightEdgeCondition},
    {.character = "i", .label = "insert here", .action = navInsertAction},
    {.character = "d", .label = "delete this", .action = navDeleteAction, .buttonConditon = notOnRightEdgeCondition},
    {.character = "b", .label = "back to previous menu", .nextMenu = BACK},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};
static button searchInsertSubmenuButtons[] = {
    {.character = "ib", .label = "insert before this", .action = insertBeforeAction},
    {.character = "ia", .label = "insert after this", .action = insertAfterAction},
    {.character = NULL, .label = NULL}};
static button searchDeleteSubmenuButtons[] = {
    {.character = "db", .label = "delete the node before this", .action = searchDelBeforeAction, .buttonConditon = thereIsANodeToLeft},
    {.character = "dt", .label = "delete this node", .action = searchDelItselfAction},
    {.character = "da", .label = "delete the node after this", .action = searchDelAfterAction, .buttonConditon = thereIsANodeToRight},
    {.character = NULL, .label = NULL}};
static button searchMenuButtons[] = {
    {.character = LEFT_ARROW_CHAR, .label = "move to the previous matching node", .action = moveCursorLeftAction, .buttonConditon = notOnLeftEdgeCondition},
    {.character = RIGHT_ARROW_CHAR, .label = "move to the next matching node", .action = moveCursorRightAction, .buttonConditon = notOnRightEdgeCondition},
    {.character = "ib", .label = "insert before this node", .action = prefixAction, .nextMenu = SEARCH_INSERT_SUBMENU},
    {.character = "ia", .label = "insert after this node", .action = prefixAction, .nextMenu = SEARCH_INSERT_SUBMENU},
    {.character = "db", .label = "delete the node before this", .action = prefixAction, .nextMenu = SEARCH_DELETE_SUBMENU, .buttonConditon = thereIsANodeToLeft},
    {.character = "dt", .label = "delete this node", .action = prefixAction, .nextMenu = SEARCH_DELETE_SUBMENU},
    {.character = "da", .label = "delete the node after this", .action = prefixAction, .nextMenu = SEARCH_DELETE_SUBMENU, .buttonConditon = thereIsANodeToRight},
    {.character = "b", .label = "back to previous menu", .nextMenu = BACK},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};
static button massInsertionMenuButtons[] = {
    {.character = "h", .label = "insert many nodes at head", .action = massHeadInsAction},
    {.character = "t", .label = "insert many nodes at tail", .action = massTailInsAction},
    {.character = "p", .label = "insert many nodes at pos", .action = massPosInsAction},
    {.character = "b", .label = "back to previous menu", .nextMenu = BACK},
    {.character = "q", .label = "quit program", .action = quitAction},
    {.character = NULL, .action = invalidInputAction}};

static button *menus[] = {
    [MAIN_MENU] = mainMenuButtons,
    [EMPTY_LIST_MENU] = emptyListMenuButtons,
    [IS_LIST_MENU] = isListMenuButtons,
    [INSERTION_MENU] = insertionMenuButtons,
    [KEY_INSERT_SUBMENU] = keyInsertSubmenuButtons,
    [DELETION_MENU] = deletionMenuButtons,
    [KEY_DELETE_SUBMENU] = keyDeleteSubmenuButtons,
    [NAVIGATION_MENU] = navigationMenuButtons,
    [SEARCH_MENU] = searchMenuButtons,
    [SEARCH_INSERT_SUBMENU] = searchInsertSubmenuButtons,
    [SEARCH_DELETE_SUBMENU] = searchDeleteSubmenuButtons,
    [MASS_INSERTION_MENU] = massInsertionMenuButtons};

//======================================================================================================

//-----------------------------------displaying helpers-------------------------------------

static int printTitle(state State)
{
    int semilinelength = ((State.terminalWidth - 1) - (int)(sizeof(TITLETEXT) - 1)) / 2;
    for (int i = 0; i < semilinelength; i++)
    {
        printf("=");
    }
    printf("%s", TITLETEXT);
    for (int i = 0; i < semilinelength; i++)
    {
        printf("=");
    }
    printf("\n");
    return 1;
}

static int printSepLine(state State)
{
    int linelength = 2 * ((State.terminalWidth - 1) / 2);
    for (int i = 0; i < linelength; i++)
    {
        printf("=");
    }
    printf("\n");
    return 1;
}

static int printButtons(state State)
{
    const char equal[] = " = ";
    const char padding[] = "  ";
    int eqwidth = strwidth(equal), padwidth = strwidth(padding);
    int lineCount = 0, currentLineWidth = 0;
    int buttonIndex = 0;
    for (buttonIndex = 0; menus[State.currentMenu][buttonIndex].character != NULL; buttonIndex++)
    {
        if (menus[State.currentMenu][buttonIndex].buttonConditon != NULL && menus[State.currentMenu][buttonIndex].buttonConditon(State) == false)
        {
            continue;
        }
        int currentButtonWidth = strwidth(menus[State.currentMenu][buttonIndex].character) + eqwidth + strwidth(menus[State.currentMenu][buttonIndex].label); // IMP: setLocale must be called once in main() before using this.
        if ((currentLineWidth > 0) && (currentLineWidth + padwidth + currentButtonWidth < State.terminalWidth))
        {
            printf("%s", padding);
            currentLineWidth += padwidth;
        }
        else if (currentLineWidth > 0)
        {
            printf("\n");
            currentLineWidth = 0;
            ++lineCount;
        }
        printf("%s%s%s", menus[State.currentMenu][buttonIndex].character, equal, menus[State.currentMenu][buttonIndex].label);
        if (currentButtonWidth >= State.terminalWidth)
        {
            lineCount += ((currentButtonWidth + State.terminalWidth - 1) / State.terminalWidth) - 1;
        }
        currentLineWidth += currentButtonWidth;
    }
    if (buttonIndex)
    {
        printf("\n");
        ++lineCount;
    }
    return lineCount;
}

static int printList(state State)
{
    if (!(State.list))
    {
        printf("No list to see :/\n");
        return 1;
    }
    int lineCount = 0, offset = 0, cursoroffset = 0, cursorFound = 0;
    const char arrowseperator[] = "<->", opensqbracket[] = "[", closesqbracket[] = "]", endingspaces[] = "   ";
    const int arrowseperatorwidth = sizeof(arrowseperator) - 1, opensqbracketwidth = sizeof(opensqbracket) - 1, closesqbracketwidth = sizeof(closesqbracket) - 1, endingspaceswidth = sizeof(endingspaces) - 1;
    LNode *current = State.list->preHead.next;
    LNode *HNcurrent = State.HNList ? State.HNList->preHead.next : NULL;
    while (current != &(State.list->postTail))
    {
        int datawidth = snprintf(NULL, 0, "%d", ((intDNode *)(current))->intData);
        int sqbracketbool = (HNcurrent) && (HNcurrent != &(State.HNList->postTail)) && (current == ((NADNode *)HNcurrent)->nodeAddressData);
        if (offset > 0)
        {
            offset += printf("%s", arrowseperator);
        }
        if (offset > 0 && ((current->next != &(State.list->postTail) ? arrowseperatorwidth : endingspaceswidth) + (sqbracketbool ? opensqbracketwidth + closesqbracketwidth : 0) + datawidth + offset >= State.terminalWidth))
        {
            printf("\n");
            ++lineCount;
            offset = 0;
            if (State.cursoredNode && cursorFound)
            {
                for (int i = 0; i < cursoroffset; i++)
                {
                    printf(" ");
                }
                printf("^\n");
                ++lineCount;
                cursorFound = 0;
            }
        }
        else
        {
            if (sqbracketbool)
            {
                offset += printf("%s", opensqbracket);
            }
            if (current == (State.HNList ? ((NADNode *)(State.cursoredNode))->nodeAddressData : State.cursoredNode))
            {
                cursoroffset = offset;
                cursorFound = 1;
            }
            offset += printf("%d", ((intDNode *)(current))->intData);
            if (sqbracketbool)
            {
                offset += printf("%s", closesqbracket);
                HNcurrent = HNcurrent->next;
            }
            current = current->next;
        }
    }
    offset += printf("%s", endingspaces);
    printf("\n");
    ++lineCount;
    if (State.cursoredNode == &(State.list->postTail))
    {
        cursoroffset = offset;
        cursorFound = 1;
    }
    if (State.cursoredNode && cursorFound)
    {
        for (int i = 0; i < cursoroffset; i++)
        {
            printf(" ");
        }
        printf("^\n");
        ++lineCount;
    }
    return lineCount;
}
//-----------------------------------------------------------------------------------------------------

//------------------action's logic----------------------
static void quitAction(state *Stateadd)
{
    Stateadd->quit = true;
}
static void createListAction(state *Stateadd)
{
    opStatus status = createList(&(Stateadd->list));
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void addFirstNodeAction(state *Stateadd)
{
    tailInsAction(Stateadd);
}
static void deleteListAction(state *Stateadd)
{
    freeList(&(Stateadd->HNList));
    Stateadd->cursoredNode = NULL;
    freeList(&(Stateadd->list));
}
static void headInsAction(state *Stateadd)
{
    int data;
    if (getInput("Enter Data:", &data) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = dinsertIntAtHead(Stateadd->list, data);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void tailInsAction(state *Stateadd)
{
    int data;
    if (getInput("Enter Data:", &data) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = appendIntDList(Stateadd->list, data);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void posInsAction(state *Stateadd)
{
    int data;
    if (getInput("Enter Data:", &data) != OP_SUCCESS)
    {
        return;
    }
    int pos;
    if (getInput("Enter the position:", &pos) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = dinsertIntAtPos(Stateadd->list, data, pos);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void prefixAction(state *Stateadd)
{
    ++Stateadd->skipchar; // coz only single character are meant to be prefixes.
}
static void afterKeyInsAction(state *Stateadd)
{
    int data;
    if (getInput("Enter Data:", &data) != OP_SUCCESS)
    {
        return;
    }
    int key;
    if (getInput("Enter key:", &key) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = dinsertIntAfterKey(Stateadd->list, data, key);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void beforeKeyInsAction(state *Stateadd)
{
    int data;
    if (getInput("Enter Data:", &data) != OP_SUCCESS)
    {
        return;
    }
    int key;
    if (getInput("Enter key:", &key) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = dinsertIntBeforeKey(Stateadd->list, data, key);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
//-------helper for massInsAction------
static opStatus askHowMany(state *Stateadd, int *howManyaddress)
{
    if (getInput("How many nodes to insert:", howManyaddress) != OP_SUCCESS)
    {
        return OP_ERROR;
    }
    if (*howManyaddress <= 0 || *howManyaddress > INT_MAX - Stateadd->list->length)
    {
        reportOpError(OP_ERROR_INVALID_INPUT, *Stateadd);
        return OP_ERROR;
    }
    return OP_SUCCESS;
}
static void massHeadInsAction(state *Stateadd)
{
    int data, howmany = 0;
    if (askHowMany(Stateadd, &howmany) != OP_SUCCESS || getInput("Enter data:", &data) != OP_SUCCESS)
    {
        return;
    }
    for (int i = 0; i < howmany; i++)
    {
        opStatus status = dinsertIntAtHead(Stateadd->list, data);
        if (reportOpError(status, *Stateadd))
        {
            break;
        }
        ++Stateadd->nodesInserted;
    }
    if (Stateadd->nodesInserted > 0)
    {
        Stateadd->insertedWhere = Stateadd->list->preHead.next;
    }
}
static void massTailInsAction(state *Stateadd)
{
    int data, howmany = 0;
    if (askHowMany(Stateadd, &howmany) != OP_SUCCESS || getInput("Enter data:", &data) != OP_SUCCESS)
    {
        return;
    }
    for (int i = 0; i < howmany; i++)
    {
        opStatus status = appendIntDList(Stateadd->list, data);
        if (reportOpError(status, *Stateadd))
        {
            break;
        }
        ++Stateadd->nodesInserted;
        if (Stateadd->nodesInserted == 1)
        {
            Stateadd->insertedWhere = Stateadd->list->postTail.prev;
        }
    }
}
static void massPosInsAction(state *Stateadd)
{
    int data, pos, howmany = 0;
    if (askHowMany(Stateadd, &howmany) != OP_SUCCESS || getInput("Enter data:", &data) != OP_SUCCESS || getInput("Enter the position:", &pos) != OP_SUCCESS)
    {
        return;
    }
    for (int i = 0; i < howmany; i++)
    {
        opStatus status = dinsertIntAtPos(Stateadd->list, data, pos);
        if (reportOpError(status, *Stateadd))
        {
            break;
        }
        ++Stateadd->nodesInserted;
    }
    if (Stateadd->nodesInserted > 0)
    {
        dgetByPos(Stateadd->list, pos, &(Stateadd->insertedWhere));
    }
}
static void headDelAction(state *Stateadd)
{
    opStatus status = ddeleteHead(Stateadd->list);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void tailDelAction(state *Stateadd)
{
    opStatus status = ddeleteTail(Stateadd->list);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void posDelAction(state *Stateadd)
{
    int pos;
    if (getInput("Enter the position:", &pos) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = ddeleteAtPos(Stateadd->list, pos);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void oneKeyDelAction(state *Stateadd)
{
    int key;
    if (getInput("Enter key:", &key) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = ddeleteTheIntKey(Stateadd->list, key);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void allKeyDelAction(state *Stateadd)
{
    int key;
    if (getInput("Enter key:", &key) != OP_SUCCESS)
    {
        return;
    }
    opStatus status = ddeleteAllIntKey(Stateadd->list, key);
    if (reportOpError(status, *Stateadd))
    {
        return;
    }
}
static void delAllNodeAction(state *Stateadd)
{
    clearList(Stateadd->list);
}
static void moveCursorLeftAction(state *Stateadd)
{
    Stateadd->cursoredNode = Stateadd->cursoredNode->prev;
}
static void moveCursorRightAction(state *Stateadd)
{
    Stateadd->cursoredNode = Stateadd->cursoredNode->next;
}
static void navInsertAction(state *Stateadd)
{
    intDNode *newNode;
    int data;
    if (getInput("Enter data:", &data) != OP_SUCCESS)
    {
        return;
    }
    opStatus s = createIntDNode(data, &newNode);
    if (reportOpError(s, *Stateadd))
    {
        return;
    }
    insertDNode(Stateadd->list, Stateadd->cursoredNode->prev, &(newNode->link));
    Stateadd->cursoredNode = Stateadd->cursoredNode->prev;
}
static void navDeleteAction(state *Stateadd)
{
    Stateadd->cursoredNode = Stateadd->cursoredNode->prev;
    deleteDNode(Stateadd->list, Stateadd->cursoredNode->next);
    if (Stateadd->cursoredNode->next != &(Stateadd->list->postTail))
    {
        Stateadd->cursoredNode = Stateadd->cursoredNode->next;
    }
    else if (Stateadd->cursoredNode == &(Stateadd->list->preHead))
    {
        Stateadd->cursoredNode = NULL;
    }
}
static void buildHNListAction(state *Stateadd)
{
    // prompt the user
    int searchfor;
    if (getInput("Search for:", &searchfor) != OP_SUCCESS)
    {
        return;
    }
    //==========================build HNList=========================
    //---if no match found---
    LNode *searchforNode = NULL;
    opStatus matchStatus;
    if ((matchStatus = dgetByIntKey(Stateadd->list, searchfor, &searchforNode)) == OP_ERROR_KEY_NOT_FOUND)
    {
        printf("No matching node was found.\n");
        fflush(stdout);
        timer(10);
        printf("\0338\x1b[J"); // this relies on getInput saving the cursor position where the search for prompt was printed
        return;
    }
    //---if match found---
    else
    {
        if (reportOpError(createList(&(Stateadd->HNList)), *Stateadd))
        {
            printf("\0338\x1b[J");
            return;
        }
        NADNode *matchedNode = NULL;
        while (matchStatus == OP_SUCCESS)
        {
            if (reportOpError(createNADNode(searchforNode, &matchedNode), *Stateadd))
            {
                printf("\0338\x1b[J");
                freeList(&(Stateadd->HNList));
                return;
            }
            insertDNode(Stateadd->HNList, Stateadd->HNList->postTail.prev, &(matchedNode->link));
            matchStatus = dgetByIntKeyFrom(Stateadd->list, searchfor, searchforNode->next, &searchforNode);
        }
    }
    //================================================================
}
// generic helper for insertion in search menu, to save from repeating similar logic
static void searchInsertAction(state *Stateadd, WRTC wrtc)
{
    LNode *priorIntDNode = NULL, *priorNADNode = NULL;
    if (wrtc == BEFORE)
    {
        priorIntDNode = ((NADNode *)(Stateadd->cursoredNode))->nodeAddressData->prev;
        priorNADNode = Stateadd->cursoredNode->prev;
    }
    if (wrtc == AFTER)
    {
        priorIntDNode = ((NADNode *)(Stateadd->cursoredNode))->nodeAddressData;
        priorNADNode = Stateadd->cursoredNode;
    }
    int data;
    if (getInput("Enter data:", &data) != OP_SUCCESS)
    {
        return;
    }
    intDNode *newIntDNode;
    if (reportOpError(createIntDNode(data, &newIntDNode), *Stateadd))
    {
        return;
    }
    insertDNode(Stateadd->list, priorIntDNode, &(newIntDNode->link));
    if (data == ((intDNode *)(((NADNode *)(Stateadd->cursoredNode))->nodeAddressData))->intData)
    {
        NADNode *newNADNode;
        if (reportOpError(createNADNode(&(newIntDNode->link), &newNADNode), *Stateadd))
        {
            deleteDNode(Stateadd->list, &(newIntDNode->link));
            return;
        }
        insertDNode(Stateadd->HNList, priorNADNode, &(newNADNode->link));
    }
}
//
static inline void insertBeforeAction(state *Stateadd)
{
    searchInsertAction(Stateadd, BEFORE);
}
static inline void insertAfterAction(state *Stateadd)
{
    searchInsertAction(Stateadd, AFTER);
}
// generic helper for deletion in search menu, to save from repeating similar logic
static void searchDeleteAction(state *Stateadd, WRTC wrtc)
{
    LNode *deadNode = NULL;
    if (wrtc == BEFORE)
    {
        deadNode = ((NADNode *)(Stateadd->cursoredNode))->nodeAddressData->prev;
        LNode *neighbouringNADNode = Stateadd->cursoredNode->prev;
        if (neighbouringNADNode != &(Stateadd->HNList->preHead) && ((NADNode *)neighbouringNADNode)->nodeAddressData == deadNode)
        {
            deleteDNode(Stateadd->HNList, neighbouringNADNode);
        }
    }
    if (wrtc == ITSELF)
    {
        deadNode = ((NADNode *)(Stateadd->cursoredNode))->nodeAddressData;
    }
    if (wrtc == AFTER)
    {
        deadNode = ((NADNode *)(Stateadd->cursoredNode))->nodeAddressData->next;
        LNode *neighbouringNADNode = Stateadd->cursoredNode->next;
        if (neighbouringNADNode != &(Stateadd->HNList->postTail) && ((NADNode *)neighbouringNADNode)->nodeAddressData == deadNode)
        {
            deleteDNode(Stateadd->HNList, neighbouringNADNode);
        }
    }
    deleteDNode(Stateadd->list, deadNode);
    if (wrtc == ITSELF)
    {
        Stateadd->cursoredNode = Stateadd->cursoredNode->prev;
        deleteDNode(Stateadd->HNList, Stateadd->cursoredNode->next);
        if (Stateadd->cursoredNode == &(Stateadd->HNList->preHead))
        {
            if (Stateadd->cursoredNode->next != &(Stateadd->HNList->postTail))
            {
                Stateadd->cursoredNode = Stateadd->cursoredNode->next;
            }
            else
            {
                Stateadd->cursoredNode = NULL;
                freeList(&(Stateadd->HNList));
            }
        }
    }
}
//
static inline void searchDelBeforeAction(state *Stateadd)
{
    searchDeleteAction(Stateadd, BEFORE);
}
static inline void searchDelItselfAction(state *Stateadd)
{
    searchDeleteAction(Stateadd, ITSELF);
}
static inline void searchDelAfterAction(state *Stateadd)
{
    searchDeleteAction(Stateadd, AFTER);
}
static void reverseListAction(state *Stateadd)
{
    reportOpError(reverseList(Stateadd->list), *Stateadd);
}
static void sortListAction(state *Stateadd)
{
    reportOpError(sortIntDList(Stateadd->list), *Stateadd);
}
//--------------------------------------------------------

//-------------conditional button's condition----------------
static bool notOnLeftEdgeCondition(state State)
{
    LNode *nodeToCheck = State.HNList ? State.HNList->preHead.next : State.list->preHead.next;
    return State.cursoredNode != nodeToCheck;
}
static bool notOnRightEdgeCondition(state State)
{
    LNode *nodeToCheck = State.HNList ? State.HNList->postTail.prev : &(State.list->postTail);
    return State.cursoredNode != nodeToCheck;
}
static bool thereIsANodeToLeft(state State) // must have a valid cursor (S.cN != NULL)
{
    LNode *relativeTo = State.HNList ? ((NADNode *)(State.cursoredNode))->nodeAddressData : State.cursoredNode;
    return relativeTo->prev != &(State.list->preHead);
}
static bool thereIsANodeToRight(state State)
{
    LNode *relativeTo = State.HNList ? ((NADNode *)(State.cursoredNode))->nodeAddressData : State.cursoredNode;
    return relativeTo->next != &(State.list->postTail);
}
static bool isNotSingleNodeList(state State) //call when list != NULL
{
    return State.list->length != 1;
}

//-------------------------------------------------------------

//--------------------------helper function-------------------------------
static button *buttonReader(state *Stateadd)
{
    if (Stateadd->skipchar > 0)
    {
        int r = timeoutC(5);
        if (r == 0)
        {
            int buttonIndex = 0;
            while (menus[Stateadd->currentMenu][buttonIndex].character != NULL)
            {
                ++buttonIndex;
            }
            return &(menus[Stateadd->currentMenu][buttonIndex]);
        }
    }
    char br[4];
    readB(br);
    button *currentButtons = menus[Stateadd->currentMenu];
    int buttonIndex = 0;
    for (buttonIndex = 0; currentButtons[buttonIndex].character != NULL; buttonIndex++)
    {
        if (menus[Stateadd->currentMenu][buttonIndex].buttonConditon != NULL && menus[Stateadd->currentMenu][buttonIndex].buttonConditon(*Stateadd) == false)
        {
            continue;
        }
        int j = 0;
        while (br[j] != '\0' && br[j] == currentButtons[buttonIndex].character[Stateadd->skipchar + j])
        {
            ++j;
        }
        if (j > 0 && br[j] == '\0')
        {
            break;
        }
    }
    return &(currentButtons[buttonIndex]);
}

static void backMenu(state *Stateadd)
{
    menu next;
    if (!reportOpError(pop(&next), *Stateadd))
    {
        Stateadd->currentMenu = next;
    }
}

static void onEnter(state *Stateadd)
{
    switch (Stateadd->currentMenu)
    {
    case NAVIGATION_MENU:
        Stateadd->cursoredNode = Stateadd->list->preHead.next;
        break;
    case SEARCH_MENU:
        Stateadd->cursoredNode = Stateadd->HNList->preHead.next;
        break;
    default:
        break;
    }
}

static void onExit(state *Stateadd)
{
    switch (Stateadd->currentMenu) // exiting to MM's clean up is already done at deleteListAction
    {
    case NAVIGATION_MENU:
        Stateadd->cursoredNode = NULL;
        break;
    case SEARCH_MENU:
        Stateadd->cursoredNode = NULL;
        freeList(&(Stateadd->HNList));
        break;
    default:
        break;
    }
}

//-------------------------------------------------------------------

void run()
{
    setvbuf(stdout, NULL, _IOFBF, 1 << 16);
    state State;
    // === initState ===
    State.list = NULL;
    State.HNList = NULL;
    State.insertedWhere = State.cursoredNode = NULL;
    State.currentMenu = MAIN_MENU;
    State.nodesInserted = State.skipchar = 0;
    State.terminalWidth = initTerminal();
    State.quit = false;
    // =================
    while (!State.quit)
    {
        //--------------------------------------------display-----------------------------------------------
        clearScreen();
        printTitle(State);
        printList(State);
        printSepLine(State);
        printButtons(State);
        fflush(stdout);
        /*------------------------------------------------------------------------------------------------*/

        //-------------------------------report nodesInserted and undo on [Z}-------------------------------
        if (State.nodesInserted > 0 && State.insertedWhere != NULL)
        {
            printf("%d %s inserted successfully. Press [z] to undo.\n", State.nodesInserted, State.nodesInserted > 1 ? "nodes" : "node");
            fflush(stdout);
            char undo[4];
            if (timeoutC(15) > 0)
            {
                readB(undo);
                if (strcmp(undo, "z") == 0)
                {
                    State.insertedWhere = State.insertedWhere->prev;
                    for (int i = 0; i < State.nodesInserted; i++)
                    {
                        deleteDNode(State.list, State.insertedWhere->next);
                    }
                }
            }
            State.nodesInserted = 0;
            State.insertedWhere = NULL;
            continue;
        }
        /*------------------------------------------------------------------------------------------------*/

        //-------------------------------------------readbutton---------------------------------------------
        button *matchedButton = buttonReader(&State);
        /*------------------------------------------------------------------------------------------------*/

        //---------------------------revertion from submenu, if it was in one-------------------------------

        if (State.skipchar > 0)
        {
            backMenu(&State);
            --(State.skipchar);
        }
        /*------------------------------------------------------------------------------------------------*/

        //-------------------------------------------takeaction---------------------------------------------
        if (matchedButton->action != NULL)
        {
            matchedButton->action(&State);
        }
        /*------------------------------------------------------------------------------------------------*/

        //-------------------------------------------changemenu---------------------------------------------
        if (State.list == NULL) // the very first piece, order matters. takes care of if the createListAction couldnt make a list, or if deleteListAction was run in takeAction(which unwinding to MAIN_MENU, must not do push mechanism as done with others .nextMenu)
        {
            while (currentDepth != 0)
            {
                reportOpError(pop(NULL), State);
            }
            State.currentMenu = MAIN_MENU;
        }
        else if (State.HNList == NULL) // takes care of if HNList became empty coz of some action in SEARCH_MENU(deletion of the last match), or if the HNList couldnt be made coz it found no matching node at the time of entering the SM it refuses the entry to SM, or else if it was not meant to enter SM to begin with, it will simply just do nothing.
        {
            if (State.currentMenu == SEARCH_MENU && matchedButton->nextMenu != BACK)
            {
                onExit(&State);
                backMenu(&State);
            }
            if (matchedButton->nextMenu == SEARCH_MENU)
            {
                continue;
            }
        }
        if (State.list != NULL && matchedButton->nextMenu != BASE) // makes sure a list exists first of all and .nextMenu has been set explicitly. it takes care of back mechanism and descending to some menu.
        {
            if (matchedButton->nextMenu == BACK) // if it was not ELM, runs menu specific cleanup then pops and sets the currentMenu. only if it was ELM and successful insertion (through 'a' button), it re-enter(back+onEnter setup) the parent menu.
            {
                int wasELM = State.currentMenu == EMPTY_LIST_MENU ? 1 : 0;
                if (!wasELM || State.list->length > 0)
                {
                    onExit(&State);
                    backMenu(&State);
                    if (wasELM)
                    {
                        onEnter(&State);
                    }
                }
            }
            else if (!reportOpError(push(State.currentMenu), State)) // pushes the parent menu to history onlt then sets the currentMenu and runs its onEnter setup
            {
                State.currentMenu = matchedButton->nextMenu;
                onEnter(&State);
            }
        }
        if (State.list != NULL && State.list->length == 0 && State.currentMenu != EMPTY_LIST_MENU) // makes sure there exists a list, it is empty and CM is not already ELM. last piece, order matters, so that it can push its right parent to the menuHistory.
        {
            if (!reportOpError(push(State.currentMenu), State))
            {
                State.currentMenu = EMPTY_LIST_MENU;
            }
        }
        /*------------------------------------------------------------------------------------------------*/
    }
    freeList(&(State.HNList));
    freeList(&(State.list));
}