#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <stdio.h>

char memory[MEMORY_SIZE];
char buffer[BUFFER_SIZE];
uint8_t bitmap_arr[((1 << MAX_LEVELS) +1)];

MyBuddyAllocator alloc;

int main(int argc, char *argv){

    // Initialize buddy allocator
    MyBuddyAllocator_init(&alloc, bitmap_arr, memory, buffer);

    printf("[MAIN]: Initialized BuddyAllocator:\n\tBitMap length in bits ---> %d \n", (&alloc)->bitmap.num_bits);

    printf("[MAIN]: Beginning of memory: %p\n", memory);

    printf("[MAIN]: Allocating all memory with blocks of fixed size... and 5 more\n");

    void *ptrs[1029];

    for(int i = 0; i < 1029; ++i){
        ptrs[i] = pseudo_malloc(1024);
        printf("Allocated ptr %p\n", ptrs[i]);
    }

    printf("End of memory %p\n", memory + MEMORY_SIZE - 1024);

    printf("[MAIN]: Now freeing ptr0 %p\n", ptrs[0]);
    pseudo_free(ptrs[0]);

    printf("[MAIN]: Now reallocating 2 of half the sieze of the freed ptr\n");
    void *new1 = pseudo_malloc(512);
    void *new2 = pseudo_malloc(512);
    printf("[MAIN]: Allocated 2 new pointers:\n\tnew1: %p\n\tnew2: %p\n", new1, new2);
}