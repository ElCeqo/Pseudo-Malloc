#pragma once
#include <stdlib.h>
#include "my_buddy_allocator.h"

//page size in bytes (checked with getconf PAGE_SIZE)
#define PAGE_SIZE 4096
#define MEMORY_SIZE (1024 * 1024) // 1MB
#define BUFFER_SIZE ((1 << MAX_LEVELS) +1) * (sizeof(MyBuddyItem) + sizeof(int))

// Structure to handle mmap requests
typedef struct mmapHandler{
    void *mem;
    size_t size;
} mmapHandler;


// alloc size bytes
void * pseudo_malloc(size_t size);

// free
void pseudo_free(void * ptr);