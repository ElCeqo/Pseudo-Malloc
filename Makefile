CC = gcc
CFLAGS = -g
LDFLAGS = -lm

SRCS = bit_map.c my_buddy_allocator.c pool_allocator.c pseudo_malloc.c pseudo_malloc_tests.c
OBJS = $(SRCS:.c=.o)
EXECUTABLE = pseudo_malloc_tests

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)
