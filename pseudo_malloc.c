#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <math.h>

// Does this refer to the alloc defined in tests?
extern MyBuddyAllocator alloc;

void * pseudo_malloc(size_t size){
    if (size == 0) return NULL;
    
    /*from man:
        If size is 0 malloc returns NULL
        On error returns NULL*/
    
    // Use buddy_alloc when:
    if (size <= PAGE_SIZE/4){
        // Calculate the level for the buddy allocator
        int level = ceil(log2(size));
        void *mem = MyBuddyAllocator_malloc(&alloc, level);
        return mem;
    }
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