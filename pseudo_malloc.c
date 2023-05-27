#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <math.h>
#include <sys/mman.h>

extern MyBuddyAllocator alloc;

void * pseudo_malloc(size_t size){

    if (size < 0){
        printf("[Malloc]: Error, size cannot be < 0, exiting\n");
        exit(EXIT_FAILURE);
    }

    if (size == 0) return NULL;
    
    // Use buddy_alloc when:
    if (size <= PAGE_SIZE/4){
        void *mem = MyBuddyAllocator_malloc(&alloc, size);
        return mem;
    }

    // Here use mmap
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

    MyBuddyAllocator_free(&alloc ,ptr);
}