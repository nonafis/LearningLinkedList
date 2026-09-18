CC := gcc
CFLAGS := -Wall
INCLUDES := -Idoubly -Isingly -Iterminal
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    FIX_PATH = $(subst /,\,$1)
    EXEC = linkedList.exe
else
    RM = rm -f
    FIX_PATH = $1
    EXEC = linkedList
endif

SRCS := linkedList.c cli.c doubly/dllist.c singly/sllist.c terminal/rawMode.c terminal/timer.c terminal/terminalcontrol.c
OBJS := $(SRCS:.c=.o)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean

clean: 
	$(RM) $(call FIX_PATH,$(EXEC) $(OBJS))

run: $(EXEC)
	$(RUN)