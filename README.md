# 🔗 LearningLinkedList

**LearningLinkedList is a C project implementing both a singly linked list and a doubly linked list, with an interactive terminal CLI built on top of the doubly linked list**, , built as a learning project to explore linked-list data structures, pointers, dynamic memory allocation, node manipulation, and low-level terminal input.

![Language: C](https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white)
![Build: Make](https://img.shields.io/badge/build-make-6D9B37)
![Platforms](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)

This project started as a way to *really* understand linked lists, and grew into a small interactive terminal app. Instead of writing a `main()` that runs a fixed set of test calls, you create a list and drive it with single keypresses: insert at the head, delete every node matching a key, or walk a cursor along the list with the arrow keys and edit as you go. The screen redraws after each action so you can watch the pointers do their work.

```
========================Linked List========================
5<->10<->20<->NULL
    ^
============================================================
← = move to left → = move to right i = insert here d = delete this
b = back to previous menu q = quit program
```

## Contents

- [Features](#features)
- [Getting started](#getting-started)
- [Using the app](#using-the-app)
- [Project structure](#project-structure)
- [API reference](#api-reference)
- [Design notes](#design-notes)
- [Roadmap](#roadmap)

## Features

- **Interactive terminal UI**: keys are read in raw mode, so there's no Enter to press for menu choices. Only numeric prompts, like *Enter Data:*, wait for Enter.
- **Cursor navigation**: move along the list with `←` / `→`. A `^` marker shows where you are, and you can insert or delete right at the cursor.
- **Rich insert and delete operations**: head, tail, by position, before or after a key, first match, all matches, or clear everything.
- **Forgiving input**: invalid keys and non-numeric input are caught and you're asked again. Out-of-range positions and missing keys report an error instead of crashing.
- **Cross-platform**: `termios` on Linux and macOS, `conio.h` and the Win32 API on Windows. A single `Makefile` handles both.
- **Two list implementations**: a doubly linked list (used by the app) and a singly linked list library.

## Getting started

### Requirements

- `gcc`
- `make`
- On Windows: a GCC toolchain such as [MinGW-w64](https://www.mingw-w64.org/) or [MSYS2](https://www.msys2.org/). [Windows Terminal](https://apps.microsoft.com/detail/9n0dx20hk701) is recommended, because the UI draws with ANSI escape codes.

### Build and run

```bash
git clone https://github.com/nonafis/LearningLinkedList.git
cd LearningLinkedList

make          # build
make run      # build (if needed) and launch
make clean    # remove the binary and object files
```

The binary is `./linkedList` (`linkedList.exe` on Windows).

To build by hand without `make`:

```bash
gcc -Wall -Idoubly -Isingly -Iterminal \
    linkedList.c cli.c doubly/dllist.c singly/sllist.c \
    terminal/rawMode.c terminal/timer.c terminal/terminalcontrol.c \
    -o linkedList
```

## Using the app

Launch the program and press `c` to create a list. Every screen shows the current list and a footer listing the keys available right now.

Two things worth knowing up front:

- **Positions are 0-indexed.** Position `0` is the head. Giving a position equal to the list's length appends to the end.
- **Two-key commands have a short window.** For `k` + `a` / `b` / `o`, press the second key within **half a second** of `k`, or the prompt times out and returns to the menu.

`q` quits from any menu, and `b` goes back one level.

### Main menu

| Key | Action |
| --- | ------ |
| `c` | Create a list |
| `q` | Quit |

### Empty list

| Key | Action |
| --- | ------ |
| `a` | Add the first node |
| `f` | Delete the list (back to the main menu) |
| `q` | Quit |

### List with nodes

| Key | Action |
| --- | ------ |
| `n` | Navigate through the list |
| `i` | Open the insert menu |
| `d` | Open the delete menu |
| `f` | Delete the list (back to the main menu) |
| `q` | Quit |

### Insert menu

| Key | Action |
| --- | ------ |
| `h` | Insert at head |
| `t` | Insert at tail |
| `p` | Insert at a given position |
| `k` then `a` | Insert **after** the node holding a given key |
| `k` then `b` | Insert **before** the node holding a given key |
| `b` | Back |

### Delete menu

| Key | Action |
| --- | ------ |
| `h` | Delete the head |
| `t` | Delete the tail |
| `p` | Delete the node at a given position |
| `k` then `o` | Delete the **first** node matching a key |
| `k` then `a` | Delete **all** nodes matching a key |
| `a` | Delete all nodes (the list stays, but empty) |
| `f` | Delete the list (back to the main menu) |
| `b` | Back |

### Navigation mode

The cursor starts on the head. It can also move one step past the last node onto `NULL`, and inserting there appends to the end.

| Key | Action |
| --- | ------ |
| `←` / `→` | Move the cursor left or right |
| `i` | Insert a new node **before** the cursor |
| `d` | Delete the node under the cursor (not available on `NULL`) |
| `b` | Back |

## Project structure

```
LearningLinkedList/
├── linkedList.c        # Entry point: main menu and program loop
├── cli.c / cli.h       # Menus, prompts, and input handling
├── doubly/
│   ├── dllist.c        # Doubly linked list implementation
│   └── dllist.h
├── singly/
│   ├── sllist.c        # Singly linked list implementation
│   └── sllist.h
├── terminal/
│   ├── rawMode.c/.h          # Raw keyboard input, arrow keys (Windows + POSIX)
│   ├── timer.c/.h            # Sleep and "key pressed within N ms?" helpers
│   └── terminalcontrol.c/.h  # Clear screen / clear line via ANSI escapes
├── Makefile            # Cross-platform build
└── .gitignore
```

## API reference

The list logic is separate from the UI, so `doubly/` and `singly/` can be reused on their own.

### Doubly linked list (`doubly/dllist.h`)

**Types**

| Type | Description |
| ---- | ----------- |
| `LNode` | The `prev` / `next` links, kept separate from the data |
| `DNode` | A node: an `LNode link` plus an `int data` |
| `linkedList` | The list: `preHead` and `postTail` sentinels plus a `length` |

**Lifecycle and display**

| Function | Description | Parameters | Return Type |
| -------- | ----------- | ---------- |----------- |
| `createDList()` | Allocate and initialise a list | - | returns a pointer to the list |
| `initDList(list)` | Initialise one you already have | pointer to the list (`linkedList *list`) that is to be initialized | - |
| `createDNode(d)` | Allocate a detached node holding `d` | `int d` | returns a pointer to the DNode (`DNode *`) |
| `displayDList(list)` | Print the list as `1<->2<->3<->NULL` | `linkedList *list` | - |
| `displayDListWithCursor(list, cur)` | Same, with a `^` marker under `cur` | 1. `linkedList *list`<br>2. pointer to a node that is to be marked with a cursor in the `list` (address has to be passed in the `LNode *` format) | - |
| `clearList(list)` | Free every node but keep the list | a pointer to a valid, allocated list (`linkedList *`) | - |
| `freeDList(&list)` | Free every node and the list itself, then set the pointer to the list `NULL` | address of pointer to the list (`linkedList **listadd`) | - |
| `s2d(head)` | Convert a singly linked list into a new doubly linked list | head pointer of a singly-linked list (`SNode *head`) | returns a pointer to the list converted from the singly-linked list |

**Lookup**

| Function | Description | Parameters | Return Type |
| -------- | ----------- | ---------- | ----------- |
| `dgetByPos(list, pos)` | traverse through the `list` to reach the node at 0-indexed given position | 1. `linkedList *list`<br>2. `int pos` | returns a pointer to the node (`LNode *`) at the given `pos`ition in the `list`, or `NULL` if the entered `pos`ition was invalid |
| `dgetByKey(list, key)` | traverse through the `list` to reach the node containing `data` matching with the `key` or print "key not found" otherwise | 1. `linkedList *list`<br>2. `int key` | returns a pointer to the first node (`LNode *`) with `data` matching the given `key` in the `list`, or `NULL` if the entered `key` was invalid |
| `dgetByKeyFromNode(list, key, start)` | traverse through the `list` to reach the node containing `data` matching with the `key`, starting the traversal from `start` node | 1. `linkedList *list`<br>2. `int key` | returns a pointer to the first node (`LNode *`) with `data` matching the given `key` in the `list` if key was found in the `list` or `NULL` otherwise |

**Insertion**

| Function | Returns | Description |
| -------- | ------- | ----------- |
| `appendDList(list, d)` | – | Inserts a new node at the end of the list |
| `dinsertAtHead(list, d)` | – | Inserts a new node at the start of the list |
| `dinsertAtPos(list, d, pos)` | `int`(0 on failure(if the `pos` was invalid), 1 on success) | Insert at `pos` (pos has to be 0-indexed(valid position = [0, `length`])) |
| `dinsertAfterKey(list, d, key)` | `int`(0 on failure, 1 on success) | Insert after the first node holding `key` |
| `dinsertBeforeKey(list, d, key)` | `int`(0 on failure, 1 on success) | Insert before the first node holding `key` |
| `insertDNode(list, priorNode, inserteeNode)` | – | Low-level: insert a precreated `DNode` in after `priorNode` |

**Deletion**

| Function | Returns | Description |
| -------- | ------- | ----------- |
| `ddeleteHead(list)` | – | Remove the first `DNode` from the `list` |
| `ddeleteTail(list)` | – | Remove the last `DNode` from the `list` |
| `ddeleteAtPos(list, pos)` | `int`(0 on failure, 1 on success) | Remove the `DNode` at `pos` |
| `ddeleteTheKey(list, key)` | `int`(0 on failure, 1 on success) | Remove the first `DNode` holding `key` in the `list` |
| `ddeleteAllKey(list, key)` | `int`(0 on failure, 1 on success) | Remove every `DNode` holding `key` in the `list` |
| `deleteDNode(list, node)` | – | Low-level: unlink and free one node |

### Singly linked list (`singly/sllist.h`)

A pointer-to-head API (`SNode **headadd`) so operations on the head can update the caller's pointer. Core functions include `createSNode`, `printSNode`, `insertSNode`, `deleteSNode` , `getTail`, `getByPos`, `getByKey`, `getPriorToTail`,  `getPriorToKey`, `freeSNode`. Other functions `sinsertAtHead`, `appendSNode`, `sinsertAtPos`, `sinsertAfterKey`, `sinsertBeforeKey`, `sdeleteAtHead`, `sdeleteAtTail`, `sdeleteAtPos`, `sdeleteTheKey` and `sdeleteAllKey`. (`insertSNode` expects `NULL` as the second argument (SNode *precedorNode) for head insertion).
(`getByPos`, `getByKey` returns a pointer to `NULL` if a empty SList is passed to it).
(`getPriorToTail` and `getPriorToKey` returns a pointer to `NULL` for a empty SList and single-node SList).
(Return type of `deleteSNode` is `int`. It returns 0 when the deletion doesn't occurs(if the list passed was empty) and 1 on successful deletion).
(`sinsertAtPos` returns 0 for negative `pos`, returns 1 on success. It appends the `newNode` at the end of the list silently if the `pos` passed was more equal or more than the length).
(`sinsertAfterKey` and `sinsertBeforeKey` returns 0 if the list passed was empty or if the key was found, and returns 1 on successful insertion).
(`sdeleteAtHead`, `sdeleteAtTail`, `sdeleteAtPos`, `sdeleteTheKey` returns `int` 1 on success and 0 on failure).
## Design notes

- **Sentinel nodes.** The doubly linked list keeps `preHead` and `postTail` inside the list struct. Because those two nodes always exist, inserting at the head or tail and deleting the first or last node need no special cases for an empty list or a `NULL` neighbour.
- **Constant-time head and tail inserts.** With sentinels, `appendDList` is just "insert after `postTail.prev`", so there is no walk to the end. The singly linked list has no tail pointer, so its `appendSNode` has to traverse the whole list first (`getTail`).
- **Links embedded in the node.** `DNode` starts with an `LNode`, so a `DNode *` and an `LNode *` point at the same address and the link-manipulating code (`insertDNode`, `deleteDNode`) works on plain `LNode`s without knowing about the data.
- **Terminal input in raw mode.** `rawMode.c` switches the terminal out of line-buffered mode to read single keys, and translates arrow-key escape sequences into the codes `300` (left) and `400` (right). The Windows and POSIX versions live side by side behind `#ifdef _WIN32`.
