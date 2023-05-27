#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <math.h>
#include <sys/mman.h>

extern MyBuddyAllocator alloc;
extern char memory[MEMORY_SIZE]; // Used to check if ptr is in buddy

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
    if(ptr == NULL) return;

    // Use buddy if...
    if(memory < ptr < memory + MEMORY_SIZE){
        MyBuddyAllocator_free(&alloc, ptr);
    }

    // Here use mmap
}