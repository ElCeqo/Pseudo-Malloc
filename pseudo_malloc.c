#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"

#define MEMORY_SIZE (1024 * 1024) // 1MB

char memory[MEMORY_SIZE];

void * pseudo_malloc(size_t size){
    //TODO
    
    /*from man:
        If size is 0 malloc returns NULL
        On error returns NULL*/
    
    //Just a test
    uint8_t *bitmap;
    MyBuddyAllocator allocator;
    MyBuddyAllocator_init(&allocator, bitmap, &memory);
}

void pseudo_free(void * ptr){
    //TODO

    /*from man:
        frees memory space pointed by *ptr
        which must have been returned by a previous
        malloc().
        if free(ptr) has already been called -> undefined behaviour
        if ptr is NULL no operation occurs
        on error returns NULL*/
}