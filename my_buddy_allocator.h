#pragma once
#include "bit_map.h"
#include "pseudo_malloc.h"
#include "math.h"

//#define MAX_LEVELS log2(PAGE_SIZE/4) // Can store bytes from 2^(MAX_LEVELS)
#define MAX_LEVELS 10 //doesn't work with expression //TO BE FIXED
typedef struct MyBuddyAllocator {
    BitMap bitmap;                    // keeps track of the allocation status of the memory blocks
    size_t numblocks[MAX_LEVELS + 1]; //number of available blocks at each level

    int min_size;                     // minimum size in bytes to be allocated
} MyBuddyAllocator;

// initialize buddy allocator
void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *memory);

// Returns a block of memory of a specified level
//returns a void * and not a structure for now -> Should I implement it?
void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int level);

// releases block of memory
void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr);