#include "my_buddy_allocator.h"

void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *memory){
    
    int number_of_bits = (int)pow(2, MAX_LEVELS);
    BitMap_init(&buddyAllocator->bitmap, number_of_bits, memory);

    buddyAllocator->min_size = 1; //minimum 1 byte of allocation

    // initialize the number of blocks at each level
    for(int i = 0; i < MAX_LEVELS; ++i){
        buddyAllocator->numblocks[i] = (1 << MAX_LEVELS - i);
    }

}

void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int level){

}

void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr){

}