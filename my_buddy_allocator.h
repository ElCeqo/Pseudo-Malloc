#pragma once
#include "bit_map.h"
#include "pseudo_malloc.h"
#include "math.h"
#include "pool_allocator.h"

#define MAX_LEVELS 20 // 1MB of memory

typedef enum {
    Succes=0x0,
    NotEnoughMemory=-1,
    UnalignedFree=-2,
    OutOfRange=-3,
    DoubleFree=-4
} MyBuddyAllocatorResult;

typedef struct MyBuddyItem{
    int idx;       // tree index
    int level;     //level of the tree
    char *start;   //pointer to start of memory
    int size;
} MyBuddyItem;

typedef struct MyBuddyAllocator {
    BitMap bitmap;                    // keeps track of the allocation status of the memory blocks
    size_t num_nodes[MAX_LEVELS + 1]; // number of available blocks at each level
    char *buffer;                     // memory area to manage
    PoolAllocator items;
} MyBuddyAllocator;

// initialize buddy allocator
void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory);

// Returns a block of memory of a specified level
//returns a void * and not a structure for now -> Should I implement it?
void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int level);

// releases block of memory
void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr);