#pragma once
#include "bit_map.h"
#include <stdio.h>

#define MAX_LEVELS 20 // 1MB of memory


typedef struct MyBuddyAllocator {
    BitMap bitmap;                    // keeps track of the allocation status of the memory blocks
    size_t num_nodes[MAX_LEVELS + 1]; // number of available blocks at each level
    int num_levels;                   // number of levels
    char *memory;                     // memory area to manage
} MyBuddyAllocator;

// initialize buddy allocator
void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory);

// Returns a block of memory of a specified level
//returns a void * and not a structure for now -> Should I implement it?
void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int level);

// releases block of memory
void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr);

void printBitMap(BitMap *bitmap);
