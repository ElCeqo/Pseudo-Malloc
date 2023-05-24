#include "my_buddy_allocator.h"

void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory){
    
    int number_of_bits = 1 << MAX_LEVELS +1;
    BitMap_init(&buddyAllocator->bitmap, number_of_bits, bitmap);

    buddyAllocator->buffer = memory;
    buddyAllocator->min_size = 1; //minimum 1 byte of allocation

    // initialize the number of blocks at each level
    for(int i = 0; i < MAX_LEVELS; ++i){
        buddyAllocator->numblocks[i] = (1 << MAX_LEVELS - i);
    }

}

void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int level){
    int num_blocks = buddyAllocator->numblocks[level];

    // Find the first available block of the specified level
    // Check the bounds in the level, so from 2^level to (2^level+1)-1
    size_t start_index = 1 << level;
    while (start_index <= ((1 << (level +1)) -1) && BitMap_bit(&buddyAllocator->bitmap, start_index)){
        start_index++;
    }

    // If no available block is found return NULL
    if (start_index > ((1 << (level +1)) -1)) return NULL;

    // Mark the block as allocated in the BitMap
    // It has to allocate all the bits corresponding to the occupied memory
    for (int i = 0; i < num_blocks; ++i){
        BitMap_setBit(&buddyAllocator->bitmap, start_index + i, 1);
    }

    // Calculate and return the pointer to the allocated block
    return (void *)(start_index * 1 << level);
}

void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr){

}