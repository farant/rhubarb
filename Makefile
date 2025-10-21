CC = gcc
CFLAGS = -Wall -Wextra -std=c89 -Iinclude -Ilib/arena 
LDFLAGS = 

# Source files
ARENA_SRCS = lib/arena/arena.c

# Object files
ARENA_OBJS = $(ARENA_SRCS:.c=.o)

# Libraries (static archives)
LIBS = lib/arena/arena.a 

# Demos
DEMO_SRCS = $(wildcard demos/*.c)
DEMO_BINS = $(DEMO_SRCS:.c=)

# Default target 
all: $(LIBS) $(DEMO_BINS)

# Build static libraries
lib/arena/arena.a: $(ARENA_OBJS)
	ar rcs $@ $^

# Build demos
demo/arena_demo: demos/arena_demo.c $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

demos/%: demos/%.c $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile C to object files
%.o: %.c
	@$(CC) $(CFLAGS) -MM $< > $@

-include $(ARENA_SRCS:.c=.d)
-include $(STRING_SRCS:.c=.d)

# Clean
clean:
	rm -f $(ARENA_OBJS)
	rm -f lib/arena/arena.a
	rm -f $(DEMO_BINS)
	rm -f $(ARENA_SRCS:.c=.d) 

.PHONY: all clean
