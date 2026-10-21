# 🔗 LearningLinkedList

**LearningLinkedList is a C project implementing both a singly linked list and a doubly linked list, with an interactive terminal CLI built on top of the doubly linked list.** It is a learning project for exploring linked-list data structures, pointers, dynamic memory allocation, node manipulation, and low-level terminal input.

![Language: C](https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white)
![Build: Make](https://img.shields.io/badge/build-make-6D9B37)
![Platforms](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)

This project started as a way to *really* understand linked lists, and grew into a small interactive terminal app. Instead of writing a `main()` that runs a fixed set of test calls, you create a list and drive it with single keypresses: insert at the head, search for matching values, or walk a cursor along the list with the arrow keys and edit as you go. The screen redraws after each action so you can watch the pointers do their work.

```
========================Linked List========================
5<->10<->20
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

## Features

- **Interactive terminal UI**: keys are read in raw mode, so there's no Enter to press for menu choices. Only numeric prompts, like *Enter Data:*, wait for Enter.
- **Cursor navigation**: move along the list with `←` / `→`. A `^` marker shows where you are, and you can insert or delete right at the cursor.
- **Search mode**: find every node matching a value, highlight the matches, and navigate between them to insert around or delete a selected match.
- **Rich insert and delete operations**: head, tail, by position, before or after a key, first match, all matches, or clear everything.
- **Forgiving input**: invalid keys and non-numeric input are caught and you're asked again. Out-of-range positions and missing keys report an error instead of crashing.
- **Cross-platform terminal support**: `termios` on Linux and macOS, and `conio.h` plus the Win32 API on Windows. The UI uses ANSI escape codes and restores the terminal screen and cursor on exit.
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
gcc -Wall -Wextra -I. -Idoubly -Isingly -Iterminal \
    linkedList.c cli.c doubly/dllist.c singly/sllist.c \
    terminal/rawMode.c terminal/timer.c terminal/terminalcontrol.c \
    terminal/terminalconfig.c \
    -o linkedList
```

## Using the app

Launch the program and press `c` to create a list. Every screen shows the current list and a footer listing the keys available right now.

Two things worth knowing up front:

- **Positions are 0-indexed.** Position `0` is the head. Giving a position equal to the list's length appends to the end.
- **Two-key commands have a short window.** For `k` + `a` / `b` / `o` and `i` + `a` / `b` in search mode, press the second key within **half a second** of the first, or the command times out.

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

### Search mode

Pressing `s` in the list menu prompts for a value. Matching nodes are enclosed in square brackets, and the cursor starts on the first match.

| Key | Action |
| --- | ------ |
| `←` / `→` | Move to the previous or next matching node |
| `i` then `b` | Insert a value before the selected match |
| `i` then `a` | Insert a value after the selected match |
| `d` | Delete the selected matching node |
| `b` | Leave search mode |

If there are no matching nodes, the app reports that and returns to the previous menu.

## Project structure

```
LearningLinkedList/
├── linkedList.c        # Entry point: raw-mode setup, then run()
├── cli.c / cli.h       # Menu state machine, rendering, prompts, and search mode
├── convention.h        # Shared opStatus codes, menu ids, and arrow-key strings
├── doubly/
│   ├── dllist.c        # Doubly linked list implementation
│   └── dllist.h
├── singly/
│   ├── sllist.c        # Singly linked list implementation
│   └── sllist.h
├── terminal/
│   ├── rawMode.c/.h          # Raw single-key input via readB, arrow keys (Windows + POSIX)
│   ├── timer.c/.h            # Sleep and "key pressed within N ms?" helpers
│   ├── terminalcontrol.c/.h  # Clear screen / clear line via ANSI escapes
│   └── terminalconfig.c/.h   # Terminal setup, width detection, and exit cleanup
├── Makefile            # Cross-platform build
└── .gitignore
```

## API reference

The list logic is separate from the UI, so the libraries in `doubly/` and `singly/` can be reused independently. The declarations below are in the respective headers; operations that can fail return an `opStatus` (defined in `convention.h`) and provide results through output parameters.

### Doubly linked list (`doubly/dllist.h`)

`linkedList` contains `preHead` and `postTail` sentinel links and a `length`. In this header, "`DNode`" means any node type whose first member is an `LNode link`; the link is embedded so traversal and link operations use `LNode *`. Two concrete node types exist:

- `intDNode`: an `LNode link` plus an `int intData` payload (the nodes the app stores)
- `NADNode`: an `LNode link` plus an `LNode *nodeAddressData` payload ("node address" data, used for search results that point at nodes in the main list)

Payload-specific functions (those with `Int` or `NAD` in the name) must only be used with the matching node type.

**Lifecycle and lookup**

| Function | Description |
| -------- | ----------- |
| `createList(&list)` | Allocate and initialize a list; writes it to `linkedList **` |
| `initList(list)` | Initialize an existing `linkedList` |
| `createIntDNode(data, &node)` | Allocate a detached `intDNode` |
| `createNADNode(node, &searchNode)` | Allocate a detached `NADNode` pointing to an existing list node |
| `dgetByPos(list, pos, &node)` | Find the node at a zero-based position |
| `dgetByIntKey(list, key, &node)` | Find the first node whose `intData` matches the key |
| `dgetByIntKeyFrom(list, key, start, &node)` | Find the next matching node starting at `start` |
| `clearList(list)` | Free all data nodes while keeping the list allocated |
| `freeList(&list)` | Free the list and set its pointer to `NULL` |
| `s2d(head, &list)` | Create a doubly linked list from a singly linked list |

**Insertion and deletion**

| Function | Description |
| -------- | ----------- |
| `appendIntDList(list, data)`, `dinsertIntAtHead(list, data)` | Insert at the tail or head |
| `dinsertIntAtPos(list, data, pos)` | Insert at a zero-based position from `0` through `length` |
| `dinsertIntAfterKey(list, data, key)`, `dinsertIntBeforeKey(list, data, key)` | Insert relative to the first matching key |
| `insertDNode(list, priorNode, node)` | Low-level link operation: insert a preallocated node after `priorNode` |
| `ddeleteHead(list)`, `ddeleteTail(list)` | Delete the head or tail node |
| `ddeleteAtPos(list, pos)` | Delete the node at a zero-based position |
| `ddeleteTheIntKey(list, key)`, `ddeleteAllIntKey(list, key)` | Delete the first or all nodes matching a key |
| `deleteDNode(list, node)` | Low-level unlink-and-free operation |

### Singly linked list (`singly/sllist.h`)

A pointer-to-head API (`SNode **headadd`) lets operations update the caller's head pointer. Allocation, lookup, insertion, and deletion functions return `opStatus`; lookup functions write results through output parameters. The API includes:

- Creation and cleanup: `createSNode`, `freeSNode`
- Link operations: `insertSNode`, `deleteSNode`
- Lookup: `getTail`, `getByPos`, `getByKey`, `getPriorToTail`, `getPriorToKey`
- Insertion: `appendSNode`, `sinsertAtHead`, `sinsertAtPos`, `sinsertAfterKey`, `sinsertBeforeKey`
- Deletion: `sdeleteAtHead`, `sdeleteAtTail`, `sdeleteAtPos`, `sdeleteTheKey`, `sdeleteAllKey`

Positions are zero-based. Check each returned `opStatus` for success or an error such as an invalid position, missing key, or allocation failure.

## Design notes

- **Sentinel nodes.** The doubly linked list keeps `preHead` and `postTail` inside the list struct. Because those two nodes always exist, inserting at the head or tail and deleting the first or last node need no special cases for an empty list or a `NULL` neighbour.
- **Constant-time head and tail inserts.** With sentinels, `appendIntDList` is just "insert after `postTail.prev`", so there is no walk to the end. The singly linked list has no tail pointer, so its `appendSNode` has to traverse the whole list first (`getTail`).
- **Links embedded in the node.** Every node type (`intDNode`, `NADNode`) starts with an `LNode`, so a pointer to the node and an `LNode *` point at the same address, and the link-manipulating code (`insertDNode`, `deleteDNode`) works on plain `LNode`s without knowing about the payload. This is what lets one list type hold either integers or search-result addresses.
- **Terminal input in raw mode.** `rawMode.c` switches the terminal out of line-buffered mode to read single keys with `readB`, which writes the key into a short string. Arrow-key escape sequences are translated into the UTF-8 strings `LEFT_ARROW_CHAR` and `RIGHT_ARROW_CHAR` from `convention.h`. On POSIX, a lone `ESC` is told apart from the start of an escape sequence by waiting 50 ms (`msTimeoutC`) for more bytes. The Windows and POSIX versions live side by side behind `#ifdef _WIN32`.
- **Terminal setup and cleanup.** `terminalconfig.c` enables UTF-8 output on Windows, detects the terminal width, enters the alternate screen, and restores the screen and cursor when the program exits.

## Wishlist
- [ ] Automated testing
- [ ] Cursor semantics and translation helper
- [ ] Centralized post-operation cleanup improvements
- [ ] Transaction-like operation design
- [ ] Linked-list integrity checker
- [ ] Terminal resizing support
- [ ] Animated timeout feedback for invalid input and other timed messages
- [ ] Multiple-list support, with a list selection menu 
- [ ] properties (`p`) option for lists
- [ ] saving a list to open another
- [ ] Undo support for more operations
- [ ] Redo support
- [ ] Rotate the list left
- [ ] Rotate the list right
- [ ] Split the list
- [ ] Merge lists
- [ ] Move nodes to another position
- [ ] Edit node in place
- [ ] Sorted inserted
- [ ] Clear and refill
- [ ] Remove duplicates
- [ ] holding prefix of command to stay longer in a submenu
