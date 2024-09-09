CC := gcc
DFLAGS := -g -DDEBUG
FLAGS := -Wall -Wextra
LFLAGS := -lncurses

ifeq ($(DEBUG), true)
	FLAGS += $(DFLAGS)
endif

OBJS := $(patsubst %.c,%.o,$(wildcard *.c))

all: populaire

populaire: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

clean:
	rm -f populaire $(OBJS)

.PHONY: all clean
