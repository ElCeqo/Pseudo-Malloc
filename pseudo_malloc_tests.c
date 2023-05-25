#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"

char memory[MEMORY_SIZE];

MyBuddyAllocator alloc;

int main(int argc, char *argv){

    // Initialize buddy allocator
    uint8_t *bitmap_arr;
    MyBuddyAllocator_init(&alloc, bitmap_arr, memory);
}