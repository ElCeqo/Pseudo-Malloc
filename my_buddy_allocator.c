#include "my_buddy_allocator.h"
#include <assert.h>
#include <math.h>
#include "pseudo_malloc.h"

void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory){
    
    int number_of_bits = (1 << MAX_LEVELS + 1) - 1;
    BitMap_init(&buddyAllocator->bitmap, number_of_bits, bitmap);

    buddyAllocator->memory = memory;
    buddyAllocator->num_levels = MAX_LEVELS;

    // initialize the number of blocks at each level
    for(int i = 0; i <= MAX_LEVELS; ++i){
        buddyAllocator->num_nodes[i] = (1 << i);
    }
}


void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int size){
    int level = MAX_LEVELS - ceil(log2(size));
    int num_nodes = buddyAllocator->num_nodes[level];

    // Find the first available block of the specified level
    size_t offset = 0;

    while (offset < num_nodes && BitMap_bit(&buddyAllocator->bitmap, (1 << level) + offset)){
        offset++;
    }

    // If no available block is found return NULL --insert some error message
    if (offset >= num_nodes){
        printf("[BuddyMalloc]: No more memory available, returning NULL\n");
        return NULL;
    }

    size_t available_bit = (1 << level) + offset;

    // Mark the block as allocated in the BitMap
    BitMap_setBit(&buddyAllocator->bitmap, available_bit, 1);

    // Set ALL the upper tree to allocated recursively
    BitMap_ParentSetBit(&buddyAllocator->bitmap, available_bit, 1);

    // Then set to allocated all the bits corresponding to subtree
    BitMap_SetSubTree(&buddyAllocator->bitmap, available_bit, 1);

    return (void *)(buddyAllocator->memory + (startIdx(available_bit) * 1 << (buddyAllocator->num_levels - levelIdx(available_bit)))); 
}

void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr){

   if (ptr == NULL) return;

   long int idx = ((char *)ptr - buddyAllocator->memory)+1;

   // Set the bit in the bitmap back to 0
   BitMap_setBit(&buddyAllocator->bitmap, idx, 0);

   // If the buddy is also 0 set the parent to 0 and do this recursively
   BitMap_ParentSetBit(&buddyAllocator->bitmap, idx, 0);

   // And set its subtree back to 0
   BitMap_SetSubTree(&buddyAllocator->bitmap, idx, 0);
   
   printf("Free Success!\n");
   
   return;
}