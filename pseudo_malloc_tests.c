#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <stdio.h>

char memory[MEMORY_SIZE];
char buffer[BUFFER_SIZE];
uint8_t bitmap_arr[(1 << MAX_LEVELS) +1];

MyBuddyAllocator alloc;

int main(int argc, char *argv){

    // Initialize buddy allocator
    MyBuddyAllocator_init(&alloc, bitmap_arr, memory, buffer);

    printf("[MAIN]: Initialized BuddyAllocator:\n\tBitMap length in bits ---> %d \n", (&alloc)->bitmap.num_bits);

    char *ptr = (char*)pseudo_malloc(256);

    printf("[MAIN]: Allocated pointer %p", ptr);
}