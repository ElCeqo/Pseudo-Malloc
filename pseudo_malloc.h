#pragma once
#include <stdlib.h>

//page size in bytes (checked with getconf PAGE_SIZE)
#define PAGE_SIZE 4096
#define MEMORY_SIZE (1024 * 1024) // 1MB

// functions to implement

// alloc bytes bytes
void * pseudo_malloc(size_t size);

// free
void pseudo_free(void * ptr);