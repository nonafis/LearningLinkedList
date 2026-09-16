# LearningLinkedList

An interactive, terminal-based C program **doubly linked list implementation**, built as a learning project to explore linked-list data structures, pointers, dynamic memory allocation, node manipulation, and low-level terminal input.

The program provides an interactive terminal interface where nodes can be inserted, deleted, searched, and navigated using the keyboard. It also contains a separate singly linked-list implementation inside `linkedList.c`, making the repository useful as a workspace for experimenting with both singly and doubly linked lists.

---

## Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Project Structure](#project-structure)
* [Data Structures](#data-structures)
* [How the Program Works](#how-the-program-works)
* [Interactive Interface](#interactive-interface)

  * [Main Menu](#main-menu)
  * [Insertion](#insertion)
  * [Deletion](#deletion)
  * [Navigation](#navigation)
* [Linked List Implementation](#linked-list-implementation)

  * [Singly Linked List](#singly-linked-list)
  * [Doubly Linked List](#doubly-linked-list)
* [Building and Running](#building-and-running)
* [Platform Support](#platform-support)
* [Memory Management](#memory-management)
* [Implementation Details](#implementation-details)
* [Function Reference](#function-reference)
* [Learning Points](#learning-points)
* [Limitations and Notes](#limitations-and-notes)
* [Future Improvements](#future-improvements)

---

## Overview

`LearningLinkedList` is a C programming project centered around learning how linked lists work at a low level.

Rather than representing a list using an array, the project dynamically allocates nodes and connects them through pointers.

The main interactive program uses a **doubly linked list**. Each node stores an integer and has links to both its previous and next nodes.

The program supports operations such as:

* inserting at the head;
* inserting at the tail;
* inserting at a specified position;
* inserting before or after a specified key;
* deleting the head;
* deleting the tail;
* deleting a node at a specified position;
* deleting the first occurrence of a key;
* deleting all occurrences of a key;
* clearing the entire list;
* searching for nodes;
* navigating through the list using the arrow keys;
* inserting and deleting directly while navigating.

The repository also contains a complete set of helper functions for a **singly linked list**, although the interactive application built by `main()` operates on the doubly linked list.

---

## Features

### Data structure

* Singly linked-list implementation
* Doubly linked-list implementation
* Dynamic node allocation using `malloc()`
* Explicit memory deallocation using `free()`
* Doubly linked list with sentinel nodes
* List length tracking
* Conversion from singly linked list to doubly linked list

### List operations

* Insert at head
* Insert at tail
* Insert at position
* Insert before a key
* Insert after a key
* Delete head
* Delete tail
* Delete at position
* Delete first occurrence of a key
* Delete all occurrences of a key
* Clear the list
* Free the entire list

### Interactive terminal interface

* Keyboard-driven menus
* Single-key commands
* Arrow-key navigation
* Visual cursor showing the currently selected node
* Separate menus for insertion and deletion
* Interactive numeric input
* Invalid-input handling
* Timed menu transitions

### Portability

The project contains separate implementations for Windows and POSIX-like systems for:

* raw keyboard input;
* arrow-key detection;
* sleeping/timing;
* detecting keyboard input during a timeout.

---

# Project Structure

```text
LearningLinkedList/
│
├── linkedList.c
├── rawMode.h
├── timer.h
├── .gitignore
└── README.md
```

### `linkedList.c`

The main source file.

It contains:

* data-structure definitions;
* singly linked-list functions;
* doubly linked-list functions;
* memory-management functions;
* user-input functions;
* menu systems;
* list navigation;
* terminal display functions;
* `main()`.

The file is approximately 1,200 lines long and contains the actual application logic.

### `rawMode.h`

Provides platform-specific keyboard input handling.

On Windows it uses:

```c
_getch()
```

from `<conio.h>`.

On POSIX systems it uses:

```c
termios
read()
```

to put the terminal into raw/non-canonical mode.

It also converts left and right arrow keys into internal integer codes used by the navigation system.

### `timer.h`

Provides platform-specific timing functions.

The project defines:

```c
timer()
```

for delays and:

```c
timeoutC()
```

for waiting for keyboard input for a specified amount of time.

### `.gitignore`

Ignores generated files such as:

* object files;
* executables;
* libraries;
* linker output;
* debug information;
* platform-specific build artifacts.

---

# Data Structures

The project defines several related structures.

## Singly linked-list node

```c
typedef struct node
{
    int data;
    struct node *next;
} Node;
```

A singly linked-list node contains:

```text
+--------+--------+
|  data  |  next  |
+--------+--------+
```

`next` points to the following node.

The last node has:

```text
next = NULL
```

---

## Doubly linked-list link

```c
typedef struct LNode
{
    struct LNode *prev;
    struct LNode *next;
} LNode;
```

`LNode` contains only the links.

```text
+--------+--------+
|  prev  |  next  |
+--------+--------+
```

This structure is separated from the actual data so that the list can use a common link representation.

---

## Doubly linked-list node

```c
typedef struct DNode
{
    LNode link;
    int data;
} DNode;
```

A `DNode` combines:

* a `prev` pointer;
* a `next` pointer;
* an integer `data` value.

Conceptually:

```text
+--------+--------+--------+
|  prev  |  data  |  next  |
+--------+--------+--------+
```

---

## List container

```c
typedef struct linkedList
{
    LNode preHead;
    LNode postTail;
    int length;
} linkedList;
```

The doubly linked list itself stores:

* `preHead` — a sentinel node before the first actual node;
* `postTail` — a sentinel node after the last actual node;
* `length` — the number of actual data nodes.

An empty list therefore looks conceptually like:

```text
preHead <-> postTail
```

A list containing three values looks like:

```text
preHead <-> 10 <-> 20 <-> 30 <-> postTail
```

The sentinel nodes simplify insertion and deletion because actual nodes do not need special handling for many boundary cases.

---

# How the Program Works

The program starts in `main()` by enabling raw keyboard input:

```c
enableRawMode();
```

It then repeatedly displays the initial screen and waits for a command.

The initial interface provides:

```text
c = create list
q = quit program
```

When `c` is pressed, a new doubly linked list is allocated and initialized.

The program then enters the list interface.

The menu shown depends on whether the list currently contains any nodes.

---

# Interactive Interface

## Main Menu

When no list exists:

```text
c = create list
q = quit program
```

After creating a list, the program distinguishes between an empty and non-empty list.

---

## Empty List Menu

When the list contains zero nodes:

```text
a = add node
f = delete list
q = quit program
```

### `a` — Add node

Prompts for an integer and appends it to the list.

For example:

```text
Enter Data: 42
```

The resulting list is:

```text
42 <-> NULL
```

### `f` — Delete list

Clears the list and releases the list structure.

### `q` — Quit

Exits the program.

---

# Non-Empty List Menu

Once the list contains at least one node:

```text
n = navigate through the list
i = insert node
d = delete node
f = delete list
q = quit program
```

---

# Insertion

Pressing `i` opens the insertion menu.

```text
h  = insert at head
t  = insert at tail
p  = insert at given position
ka = insert after given key
kb = insert before given key
b  = back
q  = quit
```

## Insert at head

Adds a new node before the current first node.

```text
Before:

preHead <-> 10 <-> 20 <-> postTail

Insert 5:

preHead <-> 5 <-> 10 <-> 20 <-> postTail
```

Implemented by:

```c
dinsertAtHead()
```

---

## Insert at tail

Adds a node immediately before the `postTail` sentinel.

```c
appendDList()
```

is responsible for this operation.

---

## Insert at position

```c
dinsertAtPos()
```

uses a zero-based position.

For example:

```text
Position: 0  1  2
          ↓  ↓  ↓
List:    10 20 30
```

Inserting `15` at position `1` produces:

```text
10 <-> 15 <-> 20 <-> 30
```

---

## Insert after a key

```c
dinsertAfterKey()
```

searches for the first node containing the requested key and inserts a new node immediately after it.

Example:

```text
10 <-> 20 <-> 30
```

Insert `25` after key `20`:

```text
10 <-> 20 <-> 25 <-> 30
```

---

## Insert before a key

```c
dinsertBeforeKey()
```

performs the corresponding operation before the first node matching the requested key.

Example:

```text
10 <-> 20 <-> 30
```

Insert `15` before key `20`:

```text
10 <-> 15 <-> 20 <-> 30
```

---

# Deletion

Pressing `d` opens the deletion menu.

The available operations are:

```text
h  = delete head node
t  = delete tail node
p  = delete node at given position
ko = delete first occurrence of a key
ka = delete all occurrences of a key
a  = delete all nodes
f  = delete list
b  = back
q  = quit
```

---

## Delete head

```c
ddeleteHead()
```

removes the first actual node.

---

## Delete tail

```c
ddeleteTail()
```

removes the last actual node.

Because the list has a `postTail` sentinel, the last node can be found through:

```c
list->postTail.prev
```

---

## Delete at position

```c
ddeleteAtPos()
```

locates the node at the specified zero-based position and removes it.

---

## Delete first occurrence

```c
ddeleteTheKey()
```

searches for the requested key and removes the first matching node.

For:

```text
10 <-> 20 <-> 20 <-> 30
```

deleting key `20` results in:

```text
10 <-> 20 <-> 30
```

---

## Delete all occurrences

```c
ddeleteAllKey()
```

continues searching after each deletion and removes every node containing the requested key.

For:

```text
10 <-> 20 <-> 30 <-> 20 <-> 40 <-> 20
```

deleting all occurrences of `20` results in:

```text
10 <-> 30 <-> 40
```

---

## Delete all nodes

```c
clearList()
```

removes every actual node while keeping the `linkedList` container itself available.

This is different from deleting the entire list structure.

---

# Navigation

The most distinctive part of the program is its interactive list navigation.

Pressing:

```text
n
```

opens navigation mode.

The program maintains a pointer:

```c
LNode *cur
```

representing the currently selected location.

The screen displays a caret beneath the selected node:

```text
========================Linked List========================
10<->20<->30<->NULL
    ^
```

The user can move through the list with the left and right arrow keys.

The program prevents moving beyond the sentinel boundaries.

---

## Navigation commands

### `←`

Move to the previous node.

### `→`

Move to the next node.

### `i`

Insert a new node at the current position.

### `d`

Delete the currently selected node.

### `b`

Return to the previous menu.

### `q`

Quit the program.

The available navigation commands change depending on whether the cursor is at the beginning, middle, or end of the list.

---

# Linked List Implementation

## Singly Linked List

The file contains a complete set of singly linked-list operations.

The singly linked-list node is:

```c
typedef struct node
{
    int data;
    struct node *next;
} Node;
```

The implementation includes functions for:

* creating nodes;
* printing a list;
* inserting after a node;
* appending;
* inserting at the head;
* inserting at a position;
* inserting before/after a key;
* finding the tail;
* finding a node by position;
* finding a node by key;
* finding the node before the tail;
* finding the node before a key;
* deleting the head;
* deleting the tail;
* deleting at a position;
* deleting by key;
* deleting all occurrences of a key;
* freeing the entire list.

The singly linked-list implementation appears to function primarily as a learning/implementation workspace; the interactive `main()` program uses the doubly linked-list implementation instead.

---

# Doubly Linked List

The main application uses the doubly linked list.

Its structure is:

```text
                 next
preHead ──────────────────► Node
   ▲                         │
   │                         ▼
   └────────────────────── prev
```

More generally:

```text
          prev                 prev
           │                    │
           ▼                    ▼
preHead <-> Node <-> Node <-> Node <-> postTail
                     ▲
                     │
                   cur
```

The sentinel nodes make the list behave as though there is always a node before the first element and after the last element.

---

## Inserting a node

The central insertion primitive is:

```c
insertDNode()
```

It performs the four pointer updates required to place a node between two existing nodes:

```c
inserteeNode->next = priorNode->next;
inserteeNode->prev = priorNode;
priorNode->next->prev = inserteeNode;
priorNode->next = inserteeNode;
```

The list length is then incremented.

Conceptually:

```text
Before:

A <-> B

Insert X after A:

A <-> X <-> B
```

---

## Deleting a node

The central deletion primitive is:

```c
deleteDNode()
```

It reconnects the surrounding nodes:

```c
deadNode->prev->next = deadNode->next;
deadNode->next->prev = deadNode->prev;
```

The removed node is then released:

```c
free((DNode *)deadNode);
```

and the list length is decremented.

This means most higher-level deletion functions can be implemented simply by locating the appropriate node and passing it to `deleteDNode()`.

---

# Sentinel Nodes

One of the important implementation ideas in this project is the use of **sentinel nodes**.

An empty list is initialized as:

```text
preHead <-> postTail
```

with:

```c
list->preHead.prev = NULL;
list->preHead.next = &(list->postTail);

list->postTail.next = NULL;
list->postTail.prev = &(list->preHead);
```

The actual data nodes are always positioned between these two sentinels.

For example:

```text
preHead
   │
   ▼
+------+     +------+     +------+
|  10  | <-> |  20  | <-> |  30  |
+------+     +------+     +------+
                                 │
                                 ▼
                              postTail
```

This reduces the amount of special-case pointer manipulation required for operations at the boundaries of the list.

---

# Singly-to-Doubly Conversion

The project also includes:

```c
linkedList *s2d(Node *head)
```

This function accepts the head of a singly linked list and creates a new doubly linked list containing the same data.

The original singly linked list is traversed:

```text
Singly list:

10 -> 20 -> 30 -> NULL
```

and each value is appended to a new doubly linked list:

```text
preHead <-> 10 <-> 20 <-> 30 <-> postTail
```

The conversion creates new doubly linked-list nodes rather than reusing the original singly linked-list nodes.

---

# Building and Running

## Requirements

You need:

* a C compiler;
* a terminal capable of handling the program's keyboard input;
* a system supported by the platform-specific code in `rawMode.h` and `timer.h`.

A commonly used compiler is GCC.

---

## Linux / macOS / other POSIX systems

Clone the repository:

```bash
git clone https://github.com/nonafis/LearningLinkedList.git
cd LearningLinkedList
```

Compile:

```bash
gcc linkedList.c -o linkedList
```

Run:

```bash
./linkedList
```

For additional compiler warnings:

```bash
gcc -Wall -Wextra linkedList.c -o linkedList
```

---

## Windows with MinGW/GCC

Clone the repository:

```bash
git clone https://github.com/nonafis/LearningLinkedList.git
cd LearningLinkedList
```

Compile:

```bash
gcc linkedList.c -o linkedList.exe
```

Run:

```bash
linkedList.exe
```

The Windows-specific implementation uses `<conio.h>` and `<windows.h>` for keyboard input and timing.

---

# Platform Support

The project uses conditional compilation:

```c
#ifdef _WIN32
```

to distinguish Windows from POSIX-like systems.

## Windows

Keyboard input uses:

```c
_getch()
```

and arrow keys are decoded from Windows console key codes.

Timing uses:

```c
Sleep()
```

and:

```c
GetTickCount()
```

---

## POSIX

The POSIX implementation uses:

```c
termios
```

to configure the terminal.

Raw mode disables features such as:

* canonical input;
* terminal echo;
* signal generation from normal keyboard input.

Arrow-key escape sequences are read manually from standard input.

Timing uses:

```c
usleep()
```

and:

```c
select()
```

for timed keyboard detection.

---

# Input Handling

Normal numeric input and menu input are deliberately handled differently.

## Menu input

The program uses:

```c
readK()
```

to read a single key without waiting for the user to press Enter.

This makes commands such as:

```text
n
i
d
q
```

feel like an interactive application rather than a traditional line-oriented C program.

---

## Numeric input

When an operation needs an integer, raw mode is temporarily disabled.

`getInput()` then uses:

```c
scanf("%d", &input)
```

and validates the input.

After the number has been read, raw mode is enabled again.

This switching is necessary because `scanf()` and raw character-by-character keyboard input operate under different terminal input modes.

---

# Memory Management

The project explicitly allocates and releases its nodes.

## Allocation

Singly linked-list nodes are created with:

```c
malloc(sizeof(Node))
```

Doubly linked-list nodes are created with:

```c
malloc(sizeof(DNode))
```

The list container itself is also dynamically allocated:

```c
malloc(sizeof(linkedList))
```

---

## Deallocation

Individual doubly linked-list nodes are released through:

```c
free((DNode *)deadNode);
```

The entire doubly linked list can be released using:

```c
freeDList()
```

which first clears the nodes and then frees the list container.

The singly linked-list implementation provides:

```c
freeList()
```

for releasing every node in a singly linked list.

---

# Function Reference

## Singly linked list

| Function              | Purpose                                           |
| --------------------- | ------------------------------------------------- |
| `createNode()`        | Allocate and initialize a singly linked-list node |
| `printNode()`         | Print a singly linked list                        |
| `insertNode()`        | Insert a node after a specified predecessor       |
| `deleteNode()`        | Delete the head or a node after a predecessor     |
| `getTail()`           | Find the final node                               |
| `getByPos()`          | Find a node by position                           |
| `getByKey()`          | Find a node by value                              |
| `getPriorToTail()`    | Find the node immediately before the tail         |
| `getPriorToKey()`     | Find the node immediately before a matching key   |
| `appendNode()`        | Append a node                                     |
| `insert_at_head()`    | Insert at the beginning                           |
| `insert_at_pos()`     | Insert at a position                              |
| `insert_after_key()`  | Insert after a key                                |
| `insert_before_key()` | Insert before a key                               |
| `delete_at_head()`    | Delete the first node                             |
| `delete_at_tail()`    | Delete the last node                              |
| `delete_at_pos()`     | Delete at a position                              |
| `delete_the_key()`    | Delete the first matching key                     |
| `delete_all_key()`    | Delete all matching keys                          |
| `freeList()`          | Free every singly linked-list node                |

---

## Doubly linked list

| Function                   | Purpose                                              |
| -------------------------- | ---------------------------------------------------- |
| `createDNode()`            | Allocate a doubly linked-list node                   |
| `createDList()`            | Allocate and initialize a list                       |
| `initDList()`              | Initialize the sentinel nodes                        |
| `dgetByPos()`              | Find a node by position                              |
| `dgetByKey()`              | Find the first node matching a key                   |
| `dgetByKeyFromNode()`      | Search for a key starting at a specified link        |
| `insertDNode()`            | Core doubly linked-list insertion                    |
| `deleteDNode()`            | Core doubly linked-list deletion                     |
| `appendDList()`            | Append to the tail                                   |
| `dinsertAtHead()`          | Insert at the head                                   |
| `dinsertAtPos()`           | Insert at a position                                 |
| `dinsertAfterKey()`        | Insert after a key                                   |
| `dinsertBeforeKey()`       | Insert before a key                                  |
| `ddeleteHead()`            | Delete the head                                      |
| `ddeleteTail()`            | Delete the tail                                      |
| `ddeleteAtPos()`           | Delete at a position                                 |
| `ddeleteTheKey()`          | Delete the first matching key                        |
| `ddeleteAllKey()`          | Delete all matching keys                             |
| `s2d()`                    | Convert a singly linked list to a doubly linked list |
| `clearList()`              | Delete all data nodes                                |
| `freeDList()`              | Clear and free the entire list                       |
| `displayDList()`           | Display the list                                     |
| `displayDListWithCursor()` | Display the list and current cursor                  |

---

# Menu Functions

The user interface is separated into several menu functions.

| Function             | Purpose                                |
| -------------------- | -------------------------------------- |
| `emptylistmenu()`    | Interface shown when the list is empty |
| `nonemptylistmenu()` | Main interface for a non-empty list    |
| `insertionmenu()`    | Insertion operations                   |
| `deletionmenu()`     | Deletion operations                    |
| `navigationmenu()`   | Interactive cursor navigation          |
| `invalidInput()`     | Handles invalid menu commands          |

This separation keeps the main program relatively simple while each menu handles its own interaction loop.

---

# Terminal Helper Functions

| Function           | Purpose                                     |
| ------------------ | ------------------------------------------- |
| `enableRawMode()`  | Enable immediate keyboard input             |
| `disableRawMode()` | Restore normal terminal input               |
| `readK()`          | Read a keyboard command                     |
| `c2k()`            | Convert raw key codes to internal key codes |
| `timer()`          | Pause execution                             |
| `timeoutC()`       | Wait for keyboard input with a timeout      |
| `clearScreen()`    | Clear the terminal                          |
| `clearLine()`      | Move upward and clear a terminal line       |
| `getInput()`       | Read and validate an integer                |

---

# Complexity

The project is primarily intended for learning rather than optimization.

For a linked list with `n` nodes:

| Operation               |                                    Typical complexity |
| ----------------------- | ----------------------------------------------------: |
| Insert at head          |                                                `O(1)` |
| Delete head             |                                                `O(1)` |
| Insert at tail          | `O(1)` with the doubly linked-list sentinel structure |
| Delete tail             |                                                `O(1)` |
| Search by key           |                                                `O(n)` |
| Search by position      |                                                `O(n)` |
| Insert by position      |                                                `O(n)` |
| Delete by position      |                                                `O(n)` |
| Insert before/after key |         `O(n)` because the key must first be searched |
| Delete by key           |                                                `O(n)` |
| Delete all occurrences  |                                                `O(n)` |
| Display list            |                                                `O(n)` |
| Clear list              |                                                `O(n)` |

Once the target node is already known, the actual pointer manipulation performed by `insertDNode()` and `deleteDNode()` is `O(1)`.

---

# Learning Points

This repository demonstrates several important C concepts.

### 1. Structures

The linked-list nodes are represented using `struct`.

### 2. Pointers

Pointers connect nodes together and allow the list to exist independently of contiguous memory.

### 3. Pointer-to-pointer parameters

Functions such as:

```c
void appendNode(Node **headadd, int d);
```

use a pointer to the head pointer so that operations can modify the caller's head pointer.

### 4. Dynamic memory allocation

Nodes are created at runtime using `malloc()` and destroyed using `free()`.

### 5. Doubly linked lists

Every node maintains both:

```text
previous
   ↑
node
   ↓
next
```

allowing traversal in both directions.

### 6. Sentinel nodes

The `preHead` and `postTail` nodes simplify boundary operations.

### 7. Platform-specific programming

The project demonstrates how the same application can use different APIs on Windows and POSIX systems.

### 8. Terminal programming

Raw terminal input, escape sequences, cursor positioning, and screen manipulation are used to create the interactive interface.

### 9. Modular design

Although the project is contained in a small number of files, terminal input and timing functionality are separated into header files rather than being implemented entirely inside `linkedList.c`.

---

# License

No license is currently included in the repository.

If this project is intended to be shared or reused by others, consider adding an appropriate open-source license.

---

# Repository

**GitHub:**
https://github.com/nonafis/LearningLinkedList

---

## Author

Created by **nonafis** as a workspace for learning and experimenting with linked-list implementations in C.
