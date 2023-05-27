#define _GNU_SOURCE // For map_anonymous
#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <sys/mman.h>
#include <assert.h>

extern MyBuddyAllocator alloc;
extern char memory[MEMORY_SIZE];

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
    void * mem = mmap(NULL, size, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    assert(mem != MAP_FAILED);

    // Create handler
    mmapHandler handler;
    handler.size = size;
    handler.mem = mem;

    mmapHandler **target = (mmapHandler **)(handler.mem);
    *target = &handler;

    return (void *)handler.mem;
}

void pseudo_free(void * ptr){
    if(ptr == NULL) return;

    // Use buddy if...
    for(int i = 0; i < MEMORY_SIZE; ++i){
        if(&memory[i] == ptr){
            MyBuddyAllocator_free(&alloc, ptr);
            return;
        }
    }

    // Here use munmap
    // Retrieve handler from system
    char *p = (char *)ptr;
    mmapHandler **handler_ptr = (mmapHandler **)p;
    mmapHandler *handler = (mmapHandler *)handler_ptr;
    handler = *handler_ptr;

    int ret = munmap(handler->mem, handler->size);
    printf("Success\n");
}