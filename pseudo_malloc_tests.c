#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <stdio.h>

char memory[MEMORY_SIZE];

MyBuddyAllocator alloc;

int main(int argc, char *argv){

    // Initialize buddy allocator
    uint8_t *bitmap_arr;
    char *buffer;
    MyBuddyAllocator_init(&alloc, bitmap_arr, memory, buffer);

    char *ptr = (char*)pseudo_malloc(256);

    printf("Allocated pointer %p", ptr);
}